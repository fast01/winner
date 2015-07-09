/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** Pointer impl ***/
	/** ctor & dtor **/
	Pointer::Pointer()
		: m_value(0){
	}
	Pointer::Pointer(void* v)
		: m_value(v){
	}
	Pointer::~Pointer(){
	}

	/** Object **/
	Object* Pointer::clone(){
		Pointer* o =SafeNew<Pointer>();
		o->m_value =m_value;
		return o;
	}
	bool Pointer::equals(const Object* obj){
		if(const Pointer* o =dynamic_cast<const Pointer*>(obj)){
			return o->m_value == m_value;
		}
		else{
			return false;
		}
	}
	int64_t Pointer::getHashCode(){
		return static_cast<int64_t>(reinterpret_cast< uint64_t >(m_value) / 2);
	}
	String* Pointer::toString(){
		return String::Format("Pointer[0X%X]", m_value);
	}

	/** self **/
	void* Pointer::getValue(){
		return m_value;
	}
	void Pointer::setValue(void* v){
		m_value =v;
	}
}
