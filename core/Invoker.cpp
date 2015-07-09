/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** impl Invoker ***/
	/** ctor & dtor **/
	Invoker::Invoker()
		: m_pfn(0){
	}
	Invoker::~Invoker(){}
	/** Object **/
	void Invoker::finalize(){
		Super::finalize();
	}
	/** self **/
	int64_t Invoker::run(Object* arg){
		if(m_pfn){
			return m_pfn(arg);
		}
		return ErrorCode::NOT_CONFIG;
	}
	void Invoker::setFunc(PFN_FUNC pfn){
		m_pfn =pfn;
	}
	Invoker::PFN_FUNC Invoker::getFunc(){
		return m_pfn;
	}
}
