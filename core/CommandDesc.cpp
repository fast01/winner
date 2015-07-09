/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** impl CommandDesc ***/
	/** ctor & dtor **/
	CommandDesc::CommandDesc()
		: m_respond_command(0)
		, m_is_use_protocol(false){
	}
	CommandDesc::CommandDesc(const int64_t res_cmd, const bool use_protocol)
		: m_respond_command(res_cmd)
		, m_is_use_protocol(use_protocol){
	}
	CommandDesc::~CommandDesc(){}
	/** Object **/
	void CommandDesc::finalize(){
		Super::finalize();
	}
	/** self **/
	void CommandDesc::setRespondCommand(const int64_t cmd){
		m_respond_command =cmd;
	}
	int64_t CommandDesc::getRespondCommand(){
		return m_respond_command;
	}
	bool CommandDesc::isUseProtocol(){
		return m_is_use_protocol;
	}
	void CommandDesc::setUseProtocol(const bool use_protocol){
		m_is_use_protocol =use_protocol;
	}
}
