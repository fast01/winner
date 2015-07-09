/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Error impl ***/
	/** ctor & dtor **/
	Error::Error()
		: m_desc(0)
		, m_code(0){
	}
	Error::Error(const int64_t code)
		: m_desc(0)
		, m_code(code){
		ASSIGN_POINTER(m_desc, ErrorCode::Code2Desc(code));
	}
	Error::Error(String* desc, const int64_t code)
		: m_desc(0)
		, m_code(code){
		ASSIGN_POINTER(m_desc, desc);
	}
	Error::~Error(){
	}

	/** Object **/
	void Error::init(){
		Super::init();
		if(m_code == 0){
			ASSIGN_POINTER(m_desc, STR("OK"));
		}
	}
	void Error::finalize(){
		CLEAN_POINTER(m_desc);
		Super::finalize();
	}
	/** self **/
	void Error::setDesc(String* desc){
		ASSIGN_POINTER(m_desc, desc);
	}
	String* Error::getDesc(){
		return m_desc;
	}
	void Error::setCode(const int64_t code){
		m_code =code;
	}
	int64_t Error::getCode(){
		return m_code;
	}
}
