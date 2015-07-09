/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Boolean impl ***/
	/** ctor & dtor **/
	Boolean::Boolean()
		: m_value(false){
	}
	Boolean::Boolean(bool v)
		: m_value(v){
	}
	Boolean::~Boolean(){
	}

	/** Object **/
	Object* Boolean::clone(){
		Boolean* o =SafeNew<Boolean, bool>(m_value);
		return o;
	}
	bool Boolean::equals(Object* obj){
		if(Boolean* o =dynamic_cast<Boolean*>(obj)){
			return o->m_value == m_value;
		}
		else{
			return false;
		}
	}

	int64_t Boolean::getHashCode(){
		return (int64_t)m_value;
	}
	String* Boolean::toString(){
		const char* sz[2] ={
			"false", "true"
		};
		const int64_t idx =static_cast< int64_t >(m_value);
		return StringTable::Get(sz[idx]);
	}

	/** self **/
	bool Boolean::getValue(){
		return m_value;
	}
	void Boolean::setValue(const bool v){
		m_value =v;
	}
}
