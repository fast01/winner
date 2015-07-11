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
	char* Dispatcher::object_to_bytes(Object* obj, char* data, int64_t& len){
		if(!obj){
			len =-1;
			return 0;
		}
		if(Bytes* bs =dynamic_cast< Bytes* >(obj)){
			return _bytes_to_pointer(bs, data, len);
		}
		else{
			std::lock_guard<LOCK_TYPE> guard(m_lock);
			m_bytes->clear();
			bs =obj->toBytes(m_bytes);
			return _bytes_to_pointer(bs, data, len);
		}
	}
	char* Dispatcher::_bytes_to_pointer(Bytes* bs, char* data, int64_t& len){
		if(bs == 0){
			len =-1;
			return 0;
		}
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
