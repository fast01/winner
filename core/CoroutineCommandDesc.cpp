/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** impl CoroutineCommandDesc ***/
	/** ctor & dtor **/
	CoroutineCommandDesc::CoroutineCommandDesc()
		: m_callback(0){
	}
	CoroutineCommandDesc::CoroutineCommandDesc(const int64_t res_cmd, Coroutine::PFN_COROUTINE_TASK pfn)
		: CommandDesc(res_cmd, true)
		, m_callback(pfn){
	}
	CoroutineCommandDesc::CoroutineCommandDesc(const int64_t res_cmd, const bool use_protocol, Coroutine::PFN_COROUTINE_TASK pfn)
		: CommandDesc(res_cmd, use_protocol)
		, m_callback(pfn){
	}
	CoroutineCommandDesc::~CoroutineCommandDesc(){}
	/** Object **/
	void CoroutineCommandDesc::finalize(){
		Super::finalize();
	}
	/** self **/
	Coroutine::PFN_COROUTINE_TASK CoroutineCommandDesc::getCallback(){
		return m_callback;
	}
	void CoroutineCommandDesc::setCallback(Coroutine::PFN_COROUTINE_TASK pfn){
		m_callback =pfn;
	}
}
