/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CoroutineRpcInfo ***/
	/** ctor & dtor **/
	CoroutineRpcInfo::CoroutineRpcInfo(const int64_t proto_grp_id, const int64_t cr_id, CoroutineService* srv)
		: m_protocol_group_id(proto_grp_id)
		, m_coroutine_id(cr_id)
		, m_coroutine_service(srv){
		ASSERT(m_coroutine_service);
	}
	CoroutineRpcInfo::~CoroutineRpcInfo(){
	}

	/** Object **/
	void CoroutineRpcInfo::init(){
		Super::init();
	}
	void CoroutineRpcInfo::finalize(){
		if(m_coroutine_id){
			m_coroutine_service->resume_coroutine(m_coroutine_id, 0);
		}
		Super::finalize();
	}
	/** RpcInfo **/
	int64_t CoroutineRpcInfo::invoke(Object* param){
		const int64_t result =m_coroutine_service->resume_coroutine(m_coroutine_id, param);
		m_coroutine_id =0;
		return result;
	}
	/** SELF **/
	int64_t CoroutineRpcInfo::getProtocolGroupId(){
		return m_protocol_group_id;
	}
	int64_t CoroutineRpcInfo::getCoroutineId(){
		return m_coroutine_id;
	}
}
