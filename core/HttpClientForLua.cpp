/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpClientForLua ***/
	DEFINE_SUPPORT_SCRIPT(HttpClientForLua, "core::HttpClientForLua")

	/** ctor & dtor **/
	HttpClientForLua::HttpClientForLua()
		: TcpConnection(0)
		, m_header_length(0)
		, m_content_length(0)
		, m_packet_length(0)
		, m_lua_cr_id(0){
		setHeartBeatTimer(HTTP_CONNECTION_KEEP_ALIVE_TIME);
	}
	HttpClientForLua::~HttpClientForLua(){
	}

	/** Object **/
	void HttpClientForLua::init(){
		Super::init();
	}
	void HttpClientForLua::finalize(){
		_resume_coroutine(0);
		Super::finalize();
	}
	/** MonitorTarget **/
	bool HttpClientForLua::reborn(){
		return false;
	}
	bool HttpClientForLua::canReborn(){
		return false;
	}
	void HttpClientForLua::onDetachEvent(){
		Super::onDetachEvent();
		_resume_coroutine(0);
	}
	/** TcpConnection **/
	int64_t HttpClientForLua::on_auth(char* data, const int64_t s){
		setAuthed();
		return on_recv(data, s);
	}
	int64_t HttpClientForLua::on_recv(char* data, const int64_t s){
		// build coder
		BinaryCoder<192> coder;
		coder.append(data, s);
		coder.appendNull();

		// calc http request package length
		if(m_packet_length == 0){
			char* sz =coder.c_str();
			const char* s1 =strstr(sz, "\r\nContent-Length: ");
			const char* s2 =strstr(sz, "\r\n\r\n");
			if(s2){
				if(s1 && s1<s2){
					long long len =0;
					if(1 != sscanf(s1+18, "%lld", &len)){
						return -1;
					}
					m_content_length =len;
				}
				else{
					m_content_length =0;
				}
				m_header_length =s2+4 - sz;
				m_packet_length =m_header_length + m_content_length;
			}
		}

		// resume coroutine
		if(s >= m_packet_length){
			HttpRespond* res =HttpRespond::Parse(coder.c_str());
			_resume_coroutine(res);

			// clean
			const int64_t ret =m_packet_length;
			m_header_length =0;
			m_content_length =0;
			m_packet_length =0;
			return ret;
		}
		return 0;
	}
	/** lua interface **/
	int HttpClientForLua::_Send(lua_State* L){
		// check arg
		if((lua_gettop(L)<3) || (lua_isstring(L, 1)==0) || (lua_isstring(L, 2)==0) || (lua_isnumber(L, 3)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t content_len;
		const char* host =lua_tostring(L, 1);
		const char* content =lua_tolstring(L, 2, &content_len);
		const int64_t cr_id =static_cast<int64_t>(lua_tonumber(L, 3));
		ASSERT(host && content && content_len>=0 && cr_id>0);

		// send
		HttpClientForLua* client =SafeNew<HttpClientForLua>();
		if(client->_send(host, content, static_cast<int64_t>(content_len)) && push_object_to_lua<HttpClientForLua>(L, client)){
			client->m_lua_cr_id =cr_id;
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "send data error");
			return 2;
		}
	}
	int HttpClientForLua::_gc(lua_State* L){
		// check arg
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		HttpClientForLua* client =0;
		if(!get_object_from_lua<HttpClientForLua>(L, 1, client) || !client){
			lua_pushfstring(L, "fail call %s, invalid HttpClientForLua", __func__);
			lua_error(L);
			return 0;
		}
		// clean
		Monitor* monitor =Monitor::Instance();
		ASSERT(monitor);
		monitor->demonitor(client);

		client->_resume_coroutine(0);
		client->release();
		return 0;
	}

	/** private **/
	bool HttpClientForLua::_send(const char* host, const char* content, const int64_t content_len){
		//// connect
		char szip[32] ={0};
		int32_t port =0;
		if(!ParseIpPort(String::New(host), szip, port)){
			ERROR("HttpClientForLua request failed, parse ip:port error");
			return false;
		}
		if(!connect(szip, port)){
			ERROR("HttpClientForLua request failed, connect error");
			return false;
		}

		//// monitor
		Monitor* monitor =Monitor::Instance();
		ASSERT(monitor);
		if(!monitor->monitor(this)){
			ERROR("HttpClientForLua send failed, monitor error");
			return false;
		}

		//// send
		if(send(content, content_len)){
			return true;
		}
		else{
			ERROR("HttpClientForLua send failed, tcp send error");
			monitor->demonitor(this);
			return false;
		}
	}
	void HttpClientForLua::_resume_coroutine(HttpRespond* res){
		LuaService* service =dynamic_cast< LuaService* >(Service::Current());
		while(service && m_lua_cr_id>0){
			//// prepare
			lua_State* L =service->getLuaState();
			ASSERT(L);

			//// protect lua stack
			LuaTopHelper lth(L);

			//// check respond
			if(!res){
				lua_pushnil(L);
				lua_pushstring(L, "unexpected resume");
				if(lua_pcall(L, 2, 0, 0) != 0){
					const char* err =lua_tostring(L, -1);	
					ERROR("fail to call lua module on_http_message, %s", err);
				}
				break;
			}
			
			//// push function
			lua_getglobal(L, "on_http_message");
			if(!lua_isfunction(L, -1)){
				ERROR("lua service process http respond failed, on_http_message is not a lua function");
				break;
			}

			//// push respond
			lua_createtable(L, 0, 12);

			// type
			lua_pushstring(L, "respond");
			lua_setfield(L, -2, "type");

			// rpc id
			lua_pushnumber(L, static_cast<double>(m_lua_cr_id));
			lua_setfield(L, -2, "rpc_id");
			
			// version
			if(String* version =res->getVersion()){
				lua_pushstring(L, version->c_str());
				lua_setfield(L, -2, "version");
			}
			else{
				ERROR("lua service process http respond failed, version is null");
				break;
			}

			// code
			lua_pushnumber(L, static_cast<double>(res->getCode()));
			lua_setfield(L, -2, "code");

			// msg
			if(String* msg =res->getMsg()){
				lua_pushstring(L, msg->c_str());
				lua_setfield(L, -2, "msg");
			}
			else{
				ERROR("lua service process http respond failed, msg is null");
				break;
			}

			// header
			if(Hash* tb =res->getHeaderTable()){
				const int count =static_cast<int>(tb->size());
				lua_createtable(L, count, 0);
				HashIterator* it =static_cast< HashIterator* >(tb->iterator());
				while(it->next()){
					String* name =dynamic_cast< String* >(it->getKey());
					String* value =dynamic_cast< String* >(it->getValue());
					if(name && value){
						lua_pushstring(L, value->c_str());
						lua_setfield(L, -2, name->c_str());
					}
				}
				lua_setfield(L, -2, "header");
			}

			// cookie
			if(Hash* tb =res->getCookieTable()){
				const int count =static_cast<int>(tb->size());
				lua_createtable(L, count, 0);
				HashIterator* it =static_cast< HashIterator* >(tb->iterator());
				while(it->next()){
					String* name =dynamic_cast< String* >(it->getKey());
					HttpCookie* cookie =dynamic_cast< HttpCookie* >(it->getValue());
					if(name && cookie){
						if(String* value =cookie->getValue()){
							// table
							lua_newtable(L);

							// value
							lua_pushstring(L, value->c_str());
							lua_setfield(L, -2, "value");

							// domain
							if(String* domain =cookie->getDomain()){
								lua_pushstring(L, domain->c_str());
								lua_setfield(L, -2, "domain");
							}

							// path
							if(String* path =cookie->getPath()){
								lua_pushstring(L, path->c_str());
								lua_setfield(L, -2, "path");
							}

							// secure
							if(cookie->getSecure()){
								lua_pushboolean(L, 1);
								lua_setfield(L, -2, "secure");
							}

							// HttpOnly
							if(cookie->getHttpOnly()){
								lua_pushboolean(L, 1);
								lua_setfield(L, -2, "HttpOnly");
							}

							// max-age
							if(cookie->getMaxAge() > 0){
								lua_pushnumber(L, static_cast<double>(cookie->getMaxAge()));
								lua_setfield(L, -2, "max_age");
							}

							// expires
							if(String* expires =cookie->getExpireTime()){
								lua_pushstring(L, expires->c_str());
								lua_setfield(L, -2, "expires");
							}

							// set
							lua_setfield(L, -2, name->c_str());
						}
					}
				}
				lua_setfield(L, -2, "cookie");
			}

			// content
			if(Bytes* content =res->getContent()){
				lua_pushlstring(L, content->c_str(), static_cast<int>(content->size()));
				lua_setfield(L, -2, "content");
			}

			// call
			if(lua_pcall(L, 1, 0, 0) != 0){
				const char* err =lua_tostring(L, -1);	
				ERROR("fail to call lua module on_http_message, %s", err);
			}
			break;
		};
		m_lua_cr_id =0;
	}
	bool HttpClientForLua::RegisterToLua(lua_State* L){
		CLASS_FUNC static_func[1] ={
			{ "Send", &_Send},
		};
		return register_class_to_lua(L, "Core", "HttpClient", MAKE_LUA_METATABLE_NAME(core::HttpClientForLua), &HttpClientForLua::_gc,
			1, static_func,
			0, 0,
			0, 0
		);
	}
}
