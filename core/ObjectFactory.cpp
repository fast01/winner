/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl ObjectFactory ***/
	/** process singleton **/
	DEFINE_PROCESS_LOCAL_SINGLETON(ObjectFactory)

	/** ctor & dtor **/
	ObjectFactory::ObjectFactory()
		: m_class_info_tb(0){
	}
	ObjectFactory::~ObjectFactory(){
	}

	/** Object **/
	void ObjectFactory::init(){
		Super::init();
		ASSIGN_POINTER(m_class_info_tb, SafeNew<Hash>());
	}
	void ObjectFactory::finalize(){
		CLEAN_POINTER(m_class_info_tb);
		Super::finalize();
	}
	/** SELF **/
	void ObjectFactory::registerClassInfo(ClassInfo* ci){
		if(ci){
			m_class_info_tb->set(ci->getName(), ci);
		}
		else{
			WARN("object factory register class failed, ci is null");
		}
	}
	ClassInfo* ObjectFactory::getClassInfo(String* class_name){
		return static_cast< ClassInfo* >(m_class_info_tb->get(class_name));
	}
	Object* ObjectFactory::create(String* class_name){
		if(ClassInfo* ci =getClassInfo(class_name)){
			return ci->create();
		}
		else{
			ERROR("object factory create class `%s` failed, class not registered", class_name ? class_name->c_str() : "null ptr");
			return 0;
		}
	}
	Object* ObjectFactory::Create(String* class_name){
		return ObjectFactory::Instance()->create(class_name);
	}
}
