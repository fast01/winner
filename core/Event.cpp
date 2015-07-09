/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Event impl ***/
	/** ctor & dtor**/
	Event::Event()
		: m_id(0)
		, m_timestamp(DateTime::Now()){
	}
	Event::~Event(){}

	/** self **/
	void Event::setId(const int64_t id){
		m_id =id;
	}
	int64_t Event::getId(){
		return m_id;
	}

	void Event::setTimestamp(const int64_t timestamp){
		m_timestamp =timestamp;
	}
	int64_t Event::getTimestamp(){
		return m_timestamp;
	}
}


