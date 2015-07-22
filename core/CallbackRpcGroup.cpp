/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CallbackRpcGroup ***/
	/** ctor & dtor **/
	CallbackRpcGroup::CallbackRpcGroup(Object* context, PFN_CALLBACK callback)
		: m_context(context)
		, m_callback(callback)
		, m_state_array(0)
		, m_value_array(0)
		, m_success_count(0)
		, m_error_count(0)
		, m_service(0){
		RETAIN_POINTER(m_context);
	}
	CallbackRpcGroup::~CallbackRpcGroup(){
	}

	/** Object **/
	void CallbackRpcGroup::init(){
		Super::init();
		ASSIGN_POINTER(m_state_array, SafeNew<Int64Array>());
		ASSIGN_POINTER(m_value_array, SafeNew<Array>());
	}
	void CallbackRpcGroup::finalize(){
		CLEAN_POINTER(m_state_array);
		CLEAN_POINTER(m_value_array);
		RELEASE_POINTER(m_context);
		Super::finalize();
	}

	/** self **/
	int64_t CallbackRpcGroup::set(const int64_t idx, Object* param){
		// check
		if(idx<0 || idx>=getCount()){
			ERROR("CallbackRpcGroup set failed, index `%d` is out of range `[0, %d)`", (long long)idx, (long long)getCount());
			return Command::STATE_ERROR;
		}
		int64_t st =m_state_array->get(idx);
		if(st == STATE_PROCESSING){
			// set value and state
			m_value_array->set(idx, param);
			Error* err =dynamic_cast< Error* >(param);
			if(err && (err->getCode()!=ErrorCode::OK)){
				m_error_count +=1;
				m_state_array->set(idx, STATE_ERROR);
			}	
			else{	
				m_success_count +=1;
				m_state_array->set(idx, STATE_COMPLETED);
			}

			// settle
			if((m_error_count + m_success_count) >= m_state_array->size()){
				if(m_callback){
					return m_callback(this);
				}
				else{
					ERROR("CallbackRpcGroup set failed, callback is null");
					return Command::STATE_ERROR;
				}
			}
			else{
				return Command::STATE_PROCESSING;
			}
		}
		else if(st == STATE_COMPLETED){
			ERROR("CallbackRpcGroup set failed, duplicate rpc `%lld` respond", (long long)idx);
			return Command::STATE_PROCESSING;
		}
		else{
			ERROR("CallbackRpcGroup set failed, duplicate rpc `%lld` respond", (long long)idx);
			return Command::STATE_PROCESSING;
		}
	}

	/** callback **/
	void CallbackRpcGroup::setContext(Object* context){
		ASSIGN_POINTER(m_context, context);
	}
	Object* CallbackRpcGroup::getContext(){
		return m_context;
	}
	void CallbackRpcGroup::setCallback(PFN_CALLBACK pfn){
		m_callback =pfn;
	}
	CallbackRpcGroup::PFN_CALLBACK CallbackRpcGroup::getCallback(){
		return m_callback;
	}

	/** rpc **/
	bool CallbackRpcGroup::rpc(PACKET& packet, Object* req_param, CallbackItemRpcInfo* rpc_info){
		// check
		if(!rpc_info || rpc_info->getGroup()){
			return false;
		}

		// prepare
		if(!m_service){
			m_service =dynamic_cast< CallbackService* >(Service::Current());
		}
		ASSERT(m_service);

		// send and set local
		if(m_service->rpc(packet, req_param, rpc_info)){
			rpc_info->setGroup(this);
			rpc_info->setIndex(m_state_array->size());
			m_state_array->push_back(STATE_PROCESSING);
			m_value_array->push_back(0);
			return true;
		}
		else{
			return false;
		}
	}
	bool CallbackRpcGroup::rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, CallbackItemRpcInfo* rpc_info){
		PACKET packet;
		packet.size =0;
		packet.from =0;
		packet.sn =0;
		packet.to =to;
		packet.who =who;
		packet.command =cmd;
		packet.option =0;
		return rpc(packet, req_param, rpc_info);
	}
	/** init group **/
	bool CallbackRpcGroup::begin(){
		return true;
	}
	void CallbackRpcGroup::cancel(){
		// do nothing
	}
	void CallbackRpcGroup::end(){
		// do nothing
	}
	/** getter **/
	int64_t CallbackRpcGroup::getCount(){
		return m_state_array->size();
	}
	int64_t CallbackRpcGroup::getState(const int64_t idx){
		if(!(idx>=0 && idx<m_state_array->size())){
			return STATE_ERROR;
		}
		return m_state_array->get(idx);
	}
	Object* CallbackRpcGroup::getValue(const int64_t idx){
		if(!(idx>=0 && idx<m_value_array->size())){
			return 0;
		}
		return m_value_array->get(idx);
	}
	int64_t CallbackRpcGroup::getSuccessCount(){
		return m_success_count;
	}
	int64_t CallbackRpcGroup::getErrorCount(){
		return m_error_count;
	}
}
