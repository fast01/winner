/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl ClassInfo ***/
	/** ctor & dtor **/
	ClassInfo::ClassInfo(String* name, PFN_CREATOR creator)
		: m_name(name)
		, m_creator(creator)
		, m_super_name(0){
		RETAIN_POINTER(m_name);
	}
	ClassInfo::~ClassInfo(){
	}

	/** Object **/
	void ClassInfo::init(){
		Super::init();
		ASSIGN_POINTER(m_FieldInfo, SafeNew<Array>());
	}
	void ClassInfo::finalize(){
		CLEAN_POINTER(m_name);
		CLEAN_POINTER(m_super_name);
		CLEAN_POINTER(m_FieldInfo);
		Super::finalize();
	}
	/** SELF **/
	ClassInfo* ClassInfo::New(String* name, PFN_CREATOR creator){
		return SafeNew<ClassInfo>(name, creator);
	}
	ClassInfo::PFN_CREATOR ClassInfo::getCreator(){
		return m_creator;
	}
	Object* ClassInfo::create(){
		return m_creator ? m_creator() : 0;
	}
	/** name & field **/
	void ClassInfo::setName(String* name){
		ASSIGN_POINTER(m_name, name);
	}
	String* ClassInfo::getName(){
		return m_name;
	}
	String* ClassInfo::getSuperName(){
		return m_super_name;
	}
	ClassInfo* ClassInfo::getSuper(){
		if(m_super_name){
			return ObjectFactory::Instance()->getClassInfo(m_super_name);
		}
		else{
			return 0;
		}
	}
	DEFINE_PROPERTY_DYNAMIC_ARRAY(ClassInfo, Array*, FieldInfoBase*, FieldInfo)
}
