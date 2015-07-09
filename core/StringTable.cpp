/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	//*** StringTable Impl ***//
	DEFINE_PROCESS_LOCAL_SINGLETON(StringTable)

	/** ctor & dtor **/
	StringTable::StringTable()
		: m_pStrTable(0){
	}
	StringTable::~StringTable(){
	}

	/** init & finalize **/
	void StringTable::init(){
		Object::init();
		m_pStrTable =New<Hash>();
		m_pStrTable->retain();
	}
	void StringTable::finalize(){
		CLEAN_POINTER(m_pStrTable);
		Object::finalize();
	}

	/** self **/
	String* StringTable::get(const char* str){
		std::lock_guard<std::mutex> guard(m_mutex);
		return _get(str);
	}
	String* StringTable::get(String* str){
		std::lock_guard<std::mutex> guard(m_mutex);
		return _get(str);
	}
	void StringTable::clear(){
		std::lock_guard<std::mutex> guard(m_mutex);
		_clear();
	}

	/** thread not safe **/
	String* StringTable::_get(const char* str){
		if(!str) return 0;
		if(String* s =dynamic_cast<String*>(m_pStrTable->get(str))){
			return s;
		}
		else{
			String* o =String::NewString(str);
			if(o->length() < MAX_STRING_LENGTH){
				m_pStrTable->set(o, o);
			}
			return o;
		}
	}
	String* StringTable::_get(String* str){
		if(!str) return 0;
		if(String* s =dynamic_cast<String*>(m_pStrTable->get(str))){
			return s;
		}
		else{
			if(str->length() < MAX_STRING_LENGTH){
				m_pStrTable->set(str, str);
			}
			return str;
		}
	}
	void StringTable::_clear(){
		m_pStrTable->clear();
	}

	String* StringTable::Get(String* str){
		return Instance()->get(str);
	}
	String* StringTable::Get(const char* str){
		return Instance()->get(str);
	}

}
