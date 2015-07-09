/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl RpcInfo ***/
	/** ctor & dtor **/
	RpcInfo::RpcInfo()
		: m_id(0)
		, m_command(0)
		, m_expire_time(0){
	}
	RpcInfo::~RpcInfo(){
	}

	/** Object **/
	void RpcInfo::init(){
		Super::init();
	}
	void RpcInfo::finalize(){
		CLEAN_POINTER(m_command);
		Super::finalize();
	}
	/** SELF **/
	void RpcInfo::setId(const int64_t id){
		ASSERT(m_id == 0);
		m_id =id;
	}
	int64_t RpcInfo::getId(){
		return m_id;
	}

	/** associate command **/
	void RpcInfo::setCommand(Command* cmd){
		ASSIGN_POINTER(m_command, cmd);
	}
	Command* RpcInfo::getCommand(){
		return m_command;
	}

	/** expire time **/
	void RpcInfo::setExpireTime(const int64_t expire_time){
		m_expire_time =expire_time;
	}
	int64_t RpcInfo::getExpireTime(){
		return m_expire_time;
	}

	/** set **/
	void RpcInfo::set(const int64_t id, Command* cmd, const int64_t expire_time){
		ASSERT(m_id == 0);
		m_id =id;
		ASSIGN_POINTER(m_command, cmd);
		m_expire_time =expire_time;
	}
}
