/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CoroutineRpcInfo ***/
	/** ctor & dtor **/
	CoroutineRpcInfo::CoroutineRpcInfo(const int64_t proto_grp_id, const int64_t cr_id)
		: m_protocol_group_id(proto_grp_id)
		, m_coroutine_id(cr_id)
		, m_rpc_id(0)
		, m_coroutine_service(0){
	}
	CoroutineRpcInfo::~CoroutineRpcInfo(){
	}

	/** Object **/
	void CoroutineRpcInfo::init(){
		Super::init();
	}
	void CoroutineRpcInfo::finalize(){
		if(m_coroutine_service && m_rpc_id && m_coroutine_id){
			m_coroutine_service->resume_coroutine(m_coroutine_id, 0, 0);
		}
		Super::finalize();
	}
	/** RpcInfo **/
	int64_t CoroutineRpcInfo::timeout(){
		done();
		return 0;
	}
	int64_t CoroutineRpcInfo::invoke(Object* param){
		const int64_t result =m_coroutine_service->resume_coroutine(m_coroutine_id, param, m_rpc_id);
		m_coroutine_id =0;
		m_coroutine_service =0;
		m_rpc_id =0;
		done();
		return result;
	}
	/** SELF **/
	int64_t CoroutineRpcInfo::getProtocolGroupId(){
		return m_protocol_group_id;
	}
	int64_t CoroutineRpcInfo::getCoroutineId(){
		return m_coroutine_id;
	}
	void CoroutineRpcInfo::set(const int64_t rpc_id, CoroutineService* srv){
		m_rpc_id =rpc_id;
		m_coroutine_service =srv;
	}
}
