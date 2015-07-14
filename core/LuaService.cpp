/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** LuaService impl ***/
	/** ctor & dtor **/
	LuaService::LuaService()
		: m_L(0){
	}
	LuaService::~LuaService(){
	}

	/* Service */
	bool LuaService::load_module(const int64_t id, const char* path){
		if(!Super::load_module(id, path)) return false;
		if(!path) return false;
		// object pool manager
		OPH();
		
		// new state
		m_L =luaL_newstate();
		if(!m_L){
			ERROR("fail to load lua module <%s>, lua language error", path);	
			return false;	
		}
		luaL_openlibs(m_L);
		lua_settop(m_L, 0);

		// set version
		lua_pushnumber(m_L, LUA_SCRIPT_VERSION);
		lua_setglobal(m_L, "LUA_SCRIPT_VERSION");

		// open core
		if(luaL_dofile(m_L, "../core/script/core.lua")){
			ERROR("fail to load lua module <%s>, %s", path, lua_tostring(m_L, -1));	
			unload_module();
			return false;
		}
		lua_settop(m_L, 0);

		// call register
		if(!ProcessLocal::Instance()->initLua(m_L)){
			ERROR("fail to load lua module <%s>, Register error", path, lua_tostring(m_L, -1));	
			unload_module();
			return false;
		}
		lua_settop(m_L, 0);

		// set package path
		String* dir =FileSystem::ParseDir(String::New(path));
		String* str_script =String::Format("package.path =package.path .. ';../service/script/?.lua;%s/?.lua';\npackage.cpath =package.cpath .. ';../service/script/?.so;%s/?.so';", dir->c_str(), dir->c_str());
		if(luaL_dostring(m_L, str_script->c_str())){
			ERROR("fail to load lua module <%s>, %s", path, lua_tostring(m_L, -1));	
			unload_module();
			return false;
		}
		lua_settop(m_L, 0);

		// set instance
		push_object_to_lua<Requestor>(m_L, this);
		lua_setglobal(m_L, "SERVICE_INSTANCE");

		// load service command script
		str_script =String::Format("require 'service'");
		if(luaL_dostring(m_L, str_script->c_str())){
			ERROR("fail to load lua module <%s>, %s", path, lua_tostring(m_L, -1));	
			unload_module();
			return false;
		}
		lua_settop(m_L, 0);

		// load service logic script
		if(luaL_dofile(m_L, path)){
			ERROR("fail to load lua module <%s>, %s", path, lua_tostring(m_L, -1));	
			unload_module();
			return false;
		}
		lua_settop(m_L, 0);

		// set path
		ASSIGN_POINTER(m_path, String::NewString(path));

		// set id
		if(id > 0){
			lua_pushnumber(m_L, id);
			lua_setglobal(m_L, "SERVICE_ID");
		}

		// get name
		{
			lua_getglobal(m_L, "SERVICE_NAME");
			if(lua_isstring(m_L, -1)){
				CLEAN_POINTER(m_name);
				m_name =String::NewString(lua_tostring(m_L, -1));
				m_name->retain();
				lua_pop(m_L, 1);
			}
			else{
				ERROR("fail to load lua module <%s> SERVICE_NAME is invalid", path);	
				unload_module();
				return false;
			}
		}

		// get desc
		{
			lua_getglobal(m_L, "SERVICE_DESC");
			if(lua_isstring(m_L, -1)){
				CLEAN_POINTER(m_desc);
				m_desc =String::NewString(lua_tostring(m_L, -1));
				m_desc->retain();
				lua_pop(m_L, 1);
			}
			else{
				ERROR("fail to load lua module <%s> SERVICE_DESC is invalid", path);	
				unload_module();
				return false;
			}
		}

		// get id
		{
			lua_getglobal(m_L, "SERVICE_ID");
			if(lua_isnumber(m_L, -1)){
				m_id =lua_tonumber(m_L, -1);
				lua_pop(m_L, 1);
			}
			else{
				ERROR("fail to load lua module <%s> SERVICE_ID is invalid", path);	
				unload_module();
				return false;
			}
		}
		return true;
	}
	void LuaService::unload_module(){
		if(m_L){
			lua_close(m_L);
			m_L =0;
		}
		Super::unload_module();
	}
	bool LuaService::on_load(){
		if(!Super::on_load()) return false;
		if(!m_L) return false;
		LuaTopHelper lth(m_L);
		lua_getglobal(m_L, "on_load");
		if(lua_isfunction(m_L, -1)){
			lua_pushstring(m_L, m_path ? m_path->c_str() : "");
			if(lua_pcall(m_L, 1, 0, 0) != 0){
				const char* err =lua_tostring(m_L, -1);	
				ERROR("fail to call lua module on_load, %s", err);
				return false;
			}
		}
		return true;
	}
	void LuaService::on_update(const int64_t now){
		Super::on_update(now);
		if(!m_L) return;
		LuaTopHelper lth(m_L);
		lua_getglobal(m_L, "on_update");
		if(lua_isfunction(m_L, -1)){
			lua_pushnumber(m_L, now);
			if(lua_pcall(m_L, 1, 0, 0) != 0){
				const char* err =lua_tostring(m_L, -1);	
				ERROR("fail to call lua module on_update, %s", err);
			}
		}
	}
	void LuaService::on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
		Super::on_message(requestor, packet, body, body_len);
		if(!m_L) return;
		if(packet.command == HttpService::SERVICE_HTTP_PROCESS_REQUEST){
			return _on_http_request(requestor, packet, body, body_len);
		}
		else{
			return _on_logic_request(requestor, packet, body, body_len);
		}
	}
	void LuaService::on_unload(){
		if(m_L){
			LuaTopHelper lth(m_L);
			lua_getglobal(m_L, "on_unload");
			if(lua_isfunction(m_L, -1)){
				if(lua_pcall(m_L, 0, 0, 0) != 0){
					const char* err =lua_tostring(m_L, -1);	
					ERROR("fail to call lua module on_unload %s", err);
				}
			}
		}
		Super::on_unload();
	}
	void LuaService::_on_http_request(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
		LuaTopHelper lth(m_L);
		//// check requestor
		if(!requestor){
			ERROR("lua service process http request failed, requestor is null");
			return;
		}

		//// push function
		lua_getglobal(m_L, "on_http_message");
		if(!lua_isfunction(m_L, -1)){
			ERROR("lua service process http request failed, on_http_message is not a lua function");
			return;
		}

		//// build HttpRequest
		BinaryCoder<> coder;
		coder.append(reinterpret_cast< char* >(body), body_len);
		coder.appendNull();
		HttpRequest* req =HttpRequest::Parse(coder.c_str());
		if(!req){
			ERROR("lua service process http request failed, parse http `%s` failed", coder.c_str());
			return;
		}
		
		//// push request
		lua_createtable(m_L, 0, 10);
		
		// requestor
		if(!push_object_to_lua< Requestor >(m_L, requestor)){
			ERROR("lua service process http request failed, push requestor to lua error");
			return;
		}
		lua_setfield(m_L, -2, "requestor");

		// method
		if(String* method =req->getMethod()){
			lua_pushstring(m_L, method->c_str());
			lua_setfield(m_L, -2, "method");
		}
		else{
			ERROR("lua service process http request failed, method is null");
			return;
		}

		// path
		if(String* path =req->getPath()){
			lua_pushstring(m_L, path->c_str());
			lua_setfield(m_L, -2, "path");
		}
		else{
			ERROR("lua service process http request failed, path is null");
			return;
		}

		// version
		if(String* version =req->getVersion()){
			lua_pushstring(m_L, version->c_str());
			lua_setfield(m_L, -2, "version");
		}
		else{
			ERROR("lua service process http request failed, version is null");
			return;
		}

		// header
		if(Hash* tb =req->getHeaderTable()){
			const int count =static_cast<int>(tb->size());
			lua_createtable(m_L, count, 0);
			HashIterator* it =static_cast< HashIterator* >(tb->iterator());
			while(it->next()){
				String* name =dynamic_cast< String* >(it->getKey());
				String* value =dynamic_cast< String* >(it->getValue());
				if(name && value){
					lua_pushstring(m_L, value->c_str());
					lua_setfield(m_L, -2, name->c_str());
				}
			}
			lua_setfield(m_L, -2, "header");
		}

		// cookie
		if(Hash* tb =req->getCookieTable()){
			const int count =static_cast<int>(tb->size());
			lua_createtable(m_L, count, 0);
			HashIterator* it =static_cast< HashIterator* >(tb->iterator());
			while(it->next()){
				String* name =dynamic_cast< String* >(it->getKey());
				HttpCookie* cookie =dynamic_cast< HttpCookie* >(it->getValue());
				if(name && cookie){
					if(String* value =cookie->getValue()){
						lua_pushstring(m_L, value->c_str());
						lua_setfield(m_L, -2, name->c_str());
					}
				}
			}
			lua_setfield(m_L, -2, "cookie");
		}

		// post
		if(Hash* tb =req->getPostTable()){
			const int count =static_cast<int>(tb->size());
			lua_createtable(m_L, count, 0);
			HashIterator* it =static_cast< HashIterator* >(tb->iterator());
			while(it->next()){
				String* name =dynamic_cast< String* >(it->getKey());
				String* value =dynamic_cast< String* >(it->getValue());
				if(name && value){
					lua_pushstring(m_L, value->c_str());
					lua_setfield(m_L, -2, name->c_str());
				}
			}
			lua_setfield(m_L, -2, "post");
		}

		// url
		if(Url* url =req->getUrl()){
			// get
			if(Hash* tb =url->getQueryTable()){
				const int count =static_cast<int>(tb->size());
				lua_createtable(m_L, count, 0);
				HashIterator* it =static_cast< HashIterator* >(tb->iterator());
				while(it->next()){
					String* name =dynamic_cast< String* >(it->getKey());
					String* value =dynamic_cast< String* >(it->getValue());
					if(name && value){
						lua_pushstring(m_L, value->c_str());
						lua_setfield(m_L, -2, name->c_str());
					}
				}
				lua_setfield(m_L, -2, "get");
			}

			// query string
			if(String* query =url->getQueryString()){
				lua_pushstring(m_L, query->c_str());
				lua_setfield(m_L, -2, "query_string");
			}
			else{
				ERROR("lua service process http request failed, query string is null");
				return;
			}
		}
		else{
			ERROR("lua service process http request failed, url is null");
			return;
		}

		// content
		if(Bytes* content =req->getContent()){
			lua_pushlstring(m_L, content->c_str(), static_cast<int>(content->size()));
			lua_setfield(m_L, -2, "content");
		}

		// call
		if(lua_pcall(m_L, 1, 0, 0) != 0){
			const char* err =lua_tostring(m_L, -1);	
			ERROR("fail to call lua module on_http_message, %s", err);
		}
	}
	void LuaService::_on_logic_request(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
		LuaTopHelper lth(m_L);

		// prepare bytes
		Bytes* bs =SafeNew<Bytes>(body, body_len);

		// on message
		lua_getglobal(m_L, "on_logic_message");
		if(lua_isfunction(m_L, -1)){
			ERROR("lua service process logic request failed, on_logic_message is not a lua function");
			return;
		}

		// object
		Object* obj =0;
		if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
			ASSERT(static_cast<size_t>(body_len) >= sizeof(Object*));
			obj =*reinterpret_cast< Object** >(body);
		}
		// push arg
		if(!push_object_to_lua< Requestor >(m_L, requestor)){
			ERROR("lua service process logic request failed, push requestor to lua error");
			return;
		}
		if(!push_packet_to_lua(m_L, packet)){
			ERROR("lua service process logic request failed, push packet to lua error");
			return;
		}
		if(!push_object_to_lua< Bytes >(m_L, bs)){
			ERROR("lua service process logic request failed, push body to lua error");
			return;
		}
		if(!push_object_to_lua< Object >(m_L, obj)){
			ERROR("lua service process logic request failed, push request to lua error");
			return;
		}

		// call
		if(lua_pcall(m_L, 4, 0, 0) != 0){
			const char* err =lua_tostring(m_L, -1);	
			ERROR("lua service process logic request failed, %s", err);
		}
	}
}
