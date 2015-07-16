/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CallbackRpcInfo ***/
	/** ctor & dtor **/
	CallbackRpcInfo::CallbackRpcInfo(const int64_t group_id)
		: m_protocol_group_id(group_id)
		, m_context(0)
		, m_callback(0){
	}
	CallbackRpcInfo::CallbackRpcInfo(const int64_t group_id, Object* context)
		: m_protocol_group_id(group_id)
		, m_context(context)
		, m_callback(0){
		RETAIN_POINTER(m_context);
	}
	CallbackRpcInfo::CallbackRpcInfo(const int64_t group_id, Object* context, PFN_CALLBACK callback)
		: m_protocol_group_id(group_id)
		, m_context(context)
		, m_callback(callback){
		RETAIN_POINTER(m_context);
	}
	CallbackRpcInfo::~CallbackRpcInfo(){
	}

	/** impl Object **/
	void CallbackRpcInfo::init(){
		Super::init();
	}
	void CallbackRpcInfo::finalize(){
		RELEASE_POINTER(m_context);
		Super::finalize();
	}

	/** callback **/
	void CallbackRpcInfo::setContext(Object* context){
		ASSIGN_POINTER(m_context, context);
	}
	Object* CallbackRpcInfo::getContext(){
		return m_context;
	}

	void CallbackRpcInfo::setCallback(PFN_CALLBACK pfn){
		m_callback =pfn;
	}
	CallbackRpcInfo::PFN_CALLBACK CallbackRpcInfo::getCallback(){
		return m_callback;
	}
	/** invoke **/
	int64_t CallbackRpcInfo::timeout(){
		const int64_t ret =invoke(SafeNew<Error>(ErrorCode::TIMEOUT));
		done();
		return ret;
	}
	Object* CallbackRpcInfo::parse(Object* param){
		// check
		if(Command* respond =dynamic_cast< Command* >(param)){
			// prepare
			const PACKET res_packet =respond->getPacket();

			// body is object pointer
			if(res_packet.option & OPT_BODY_IS_OBJECT_POINTER){
				return respond;
			}

			// body is protocol
			if(m_protocol_group_id > 0){
				Bytes* body =respond->getBody();
				if(!body){
					ERROR("fail to invoke rpc, body is null");
					return 0; 
				}
				const int64_t who =static_cast<int64_t>(res_packet.who);
				const int64_t group_id =m_protocol_group_id;
				const int64_t protocol_id =static_cast<int64_t>(res_packet.command);
				ProtocolBase* param = ProtocolManager::CreateProtocol(group_id, protocol_id);
				if(!param){
					ERROR("%lld fail to invoke rpc, create protocol %lld %lld error", (long long)who, (long long)group_id, (long long)protocol_id);
					return 0; 
				}
				if(!param->fromBytes(body)){
					ERROR("%lld fail to invoke rpc, unmarshal protocol %s %lld %lld error", (long long)who, param->name(), (long long)group_id, (long long)protocol_id);
					return 0; 
				}
				respond->setRequest(param);
			}
			return respond;
		}
		else{
			return param;
		}
	}
	int64_t CallbackRpcInfo::invoke(Object* param){
		Object* res =parse(param);
		if(m_callback){
			const int64_t ret =m_callback(res, m_context);
			done();
			return ret;
		}
		else{
			done();
			ERROR("CallbackRpcInfo not have callback");
			return Command::STATE_ERROR;
		}
	}
}
