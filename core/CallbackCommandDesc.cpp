/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** impl CallbackCommandDesc ***/
	/** ctor & dtor **/
	CallbackCommandDesc::CallbackCommandDesc()
		: m_callback(0){
	}
	CallbackCommandDesc::CallbackCommandDesc(const int64_t res_cmd, PFN_CALLBACK pfn)
		: CommandDesc(res_cmd, true)
		, m_callback(pfn){
	}
	CallbackCommandDesc::CallbackCommandDesc(const int64_t res_cmd, const bool use_protocol, PFN_CALLBACK pfn)
		: CommandDesc(res_cmd, use_protocol)
		, m_callback(pfn){
	}
	CallbackCommandDesc::~CallbackCommandDesc(){}
	/** Object **/
	void CallbackCommandDesc::finalize(){
		Super::finalize();
	}
	/** self **/
	void CallbackCommandDesc::setCallback(PFN_CALLBACK pfn){
		m_callback =pfn;
	}
	CallbackCommandDesc::PFN_CALLBACK CallbackCommandDesc::getCallback(){
		return m_callback;
	}
}
