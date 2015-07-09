/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl LogService ***/
	DEFINE_CLASS_INFO(LogService)

	/** ctor & dtor **/
	LogService::LogService()
		: m_logger_table(0){
		m_id =SERVICE_ID;
	}
	LogService::~LogService(){}

	/** Service **/
	bool LogService::on_load(){
		if(!Super::on_load()) return false;
		ASSIGN_POINTER(m_name, STR("LogService"));
		ASSIGN_POINTER(m_logger_table, SafeNew<Hash>());
		return _load("../data/startup/log_service.lua");
	}
	void LogService::on_unload(){
		CLEAN_POINTER(m_logger_table);
		Super::on_unload();
		DEBUG("unload log service");
	}
	void LogService::on_update(const int64_t now){
		Super::on_update(now);
	}
	void LogService::on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
		Super::on_message(requestor, packet, body, body_len);
		// prepare
		if(packet.command == SERVICE_LOG_NOTIFY){
			if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
				WARN("fail to call %s, OPT_BODY_IS_OBJECT_POINTER is not support", __func__);
				return;
			}
			BinaryCoder<1024> coder(body, body_len);
			// decode param
			int64_t type =0;
			if(!coder.decodeInt64(type)){
				WARN("fail to call %s, decode type error", __func__);
				return;
			}
			int64_t flag =0;
			if(!coder.decodeInt64(flag)){
				WARN("fail to call %s, decode flag error", __func__);
				return;
			}
			String* content =coder.decodeString();
			if(!content){
				WARN("fail to call %s, decode content error", __func__);
				return;
			}

			// log
			_log(type, flag, content);
		}
		else{
			WARN("fail to call %s, command %lld is not support", __func__, (long long)packet.command);
		}
	}
	void LogService::_log(const int64_t type, const int64_t flag, String* content){
		Logger* lg =static_cast< Logger* >(m_logger_table->get(type));
		if(!lg){
			WARN("fail to call %s, type %lld is not support", __func__, (long long)type);
			return;
		}
		// set flush mode
		if(flag == FLAG_SYNC){
			lg->setFlushMode(Logger::FLUSH_MODE_SYNC);
		}
		else if(flag == FLAG_FLUSH){
			lg->setFlushMode(Logger::FLUSH_MODE_FLUSH);
		}

		// log
		lg->printf(content->c_str());
	}
	bool LogService::_load(const char* path){
		// load config
		protocol::LogServiceConfig* cfg =(protocol::LogServiceConfig*)protocol::create_protocol_from_lua_file(protocol::PROTOCOL_LOG_SERVICE_CONFIG, path);
		if(!cfg){
			DEBUG("load log service fail, cfg load error");
			return false;
		}

		// dump config
		Bytes* str =SafeNew<Bytes>();
		cfg->appendToString(str, 0);
		DEBUG(str->toString()->c_str());

		// add target
		const int64_t n =cfg->getTargetCount();
		for(int64_t i=0; i<n; ++i){
			protocol::LogTargetConfig* ltc =cfg->getTarget(i);
			Logger* lg =SafeNew<Logger>();
			lg->setName(ltc->getFileName());
			lg->SetFileLifeTime(ltc->getFileLifeTime());
			lg->setFlushMode(ltc->getFlushMode());
			m_logger_table->set(ltc->getType(), lg);
		}
		return true;
	}
	bool LogService::Log(const int64_t type, const int64_t flag, String* content){
		Service* cur_service =Service::Current();
		//packet
		PACKET packet;
		packet.from =cur_service ? cur_service->getId() : 0;
		packet.to =SERVICE_ID;
		packet.who =0;
		packet.sn =0;
		packet.command =SERVICE_LOG_NOTIFY;
		packet.option =OPT_REQUEST;

		// body
		Bytes* body =SafeNew<Bytes>();
		body->encodeInt64(type);
		body->encodeInt64(flag);
		body->encodeString(content);

		return DispatcherManager::NotifyByObject(packet, body);
	}
	bool LogService::Log(const int64_t type, const int64_t flag, const char* fmt, ...){
		String* content =0;
		va_list vl;
		va_start(vl, fmt);
		content =String::FormatV(fmt, vl);
		va_end(vl);
		return Log(type, flag, content);
	}
	/** register for lua **/
	int LogService::_Log(lua_State* L){
		// check
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		if(lua_isnumber(L, 1) == 0){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg #1, invalid type");
			return 2;
		}
		if(lua_isnumber(L, 2) == 0){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg #2, invalid flag");
			return 2;
		}
		if(lua_isstring(L, 3) == 0){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg #3, invalid content");
			return 2;
		}
		// prepare args
		const int64_t type =lua_tonumber(L, 1);
		const int64_t flag =lua_tonumber(L, 2);
		const char * content =lua_tostring(L, 3);

		// call
		if(Log(type, flag, STR(content))){
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "call LogService::Log failed");
			return 2;
		}
	}
	bool LogService::RegisterToLua(lua_State* L){
		CLASS_FUNC func[1] ={
			{ "Log", &_Log},
		};
		CLASS_ENUM enumerator[2] ={
			{ "SERVICE_ID", SERVICE_ID},
			{ "SERVICE_LOG_NOTIFY", SERVICE_LOG_NOTIFY},
		};
		return register_class_to_lua(L, "Core", "LogService", 0, 0,
			1, func,
			0, 0,
			2, enumerator
		);
	}
}
