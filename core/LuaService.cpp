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
		// prepare bytes
		Bytes* bs =SafeNew<Bytes>(body, body_len);

		// on message
		LuaTopHelper lth(m_L);
		lua_getglobal(m_L, "on_message");
		if(lua_isfunction(m_L, -1)){
			// object
			Object* obj =0;
			if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
				ASSERT(static_cast<size_t>(body_len) >= sizeof(Object*));
				obj =*reinterpret_cast< Object** >(body);
			}
			// push arg
			if(!push_object_to_lua< Requestor >(m_L, requestor)){
				ERROR("fail to call lua module on_message, push requestor error");
				return;
			}
			if(!push_packet_to_lua(m_L, packet)){
				ERROR("fail to call lua module on_message, push packet error");
				return;
			}
			if(!push_object_to_lua< Bytes >(m_L, bs)){
				ERROR("fail to call lua module on_message, push body error");
				return;
			}
			if(!push_object_to_lua< Object >(m_L, obj)){
				ERROR("fail to call lua module on_message, push object error");
				return;
			}

			// call
			if(lua_pcall(m_L, 4, 0, 0) != 0){
				const char* err =lua_tostring(m_L, -1);	
				ERROR("fail to call lua module on_message, %s", err);
			}
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
}
