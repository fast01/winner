/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Dispatcher ***/
	/** ctor & dtor **/
	Dispatcher::Dispatcher()
		: m_bytes(0){
	}
	Dispatcher::~Dispatcher(){
	}

	/** Object **/
	void Dispatcher::init(){
		Super::init();
		m_bytes =New<Bytes>();
		m_bytes->retain();
	}
	void Dispatcher::finalize(){
		CLEAN_POINTER(m_bytes);
		Super::finalize();
	}
	/** SELF **/
	char* Dispatcher::object_to_bytes(char* data, Object* obj, int64_t& len){
		if(!obj){
			len =0;
			return 0;
		}
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		// prepare bs
		Bytes* bs =dynamic_cast< Bytes* >(obj);
		if(!bs){
			m_bytes->clear();
			bs =obj->toBytes(m_bytes);
		}
		if(!bs){
			len =-1;
			return 0;
		}
		// make data
		if(bs->size() > len){
			len =bs->size();
			data =(char*)ALLOCATE(len);
			memcpy(data, bs->data(), static_cast<size_t>(len));
			return data;
		}
		else{
			len =bs->size();
			memcpy(data, bs->data(), static_cast<size_t>(len));
			return data;
		}
	}
}
