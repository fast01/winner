#include "core.h"

namespace core{
	/*** impl CallbackGroupRpcInfo ***/
	/** ctor & dtor **/
	CallbackGroupRpcInfo::CallbackGroupRpcInfo(Object* context)
		: m_context(context)
		, m_callback(0)
		, m_rpc_array(0)
		, m_state_array(0)
		, m_value_array(0)
		, m_success_count(0)
		, m_error_count(0)
		, m_sn(0){
		RETAIN_POINTER(m_context);
	}
	CallbackGroupRpcInfo::CallbackGroupRpcInfo(Object* context, PFN_CALLBACK callback)
		: m_context(context)
		, m_callback(callback)
		, m_rpc_array(0)
		, m_state_array(0)
		, m_value_array(0)
		, m_success_count(0)
		, m_error_count(0)
		, m_sn(0){
		RETAIN_POINTER(m_context);
	}
	CallbackGroupRpcInfo::~CallbackGroupRpcInfo(){
	}

	/** Object **/
	void CallbackGroupRpcInfo::init(){
		Super::init();
		ASSIGN_POINTER(m_rpc_array, SafeNew<Array>());
		ASSIGN_POINTER(m_state_array, SafeNew<Int64Array>());
		ASSIGN_POINTER(m_value_array, SafeNew<Array>());
	}
	void CallbackGroupRpcInfo::finalize(){
		CLEAN_POINTER(m_rpc_array);
		CLEAN_POINTER(m_state_array);
		CLEAN_POINTER(m_value_array);
		RELEASE_POINTER(m_context);
		Super::finalize();
	}

	/** RpcInfo **/
	int64_t CallbackGroupRpcInfo::timeout(){
		if(m_callback){
			const int64_t ret =m_callback(this);
			done();
			return ret;
		}
		else{
			done();
			return Command::STATE_ERROR;
		}
	}
	int64_t CallbackGroupRpcInfo::invoke(Object* param){
		if(Command* res =dynamic_cast< Command* >(param)){
			const PACKET& packet =res->getPacket();
			const int64_t idx =static_cast< int64_t >(packet.sub_sn);
			if(!(idx>=0 && idx<m_state_array->size())){
				ERROR("CallbackGroupRpcInfo invoke sub sn is out of range");
				done();
				return Command::STATE_ERROR;
			}
			int64_t st =m_state_array->get(idx);
			CallbackRpcInfo* ri =static_cast< CallbackRpcInfo* >(m_rpc_array->get(idx));
			if(st == STATE_PROCESSING){
				if(Object* res =ri->parse(param)){
					m_success_count +=1;
					m_value_array->set(idx, res);
					m_state_array->set(idx, STATE_COMPLETED);
				}	
				else{	
					m_error_count +=1;
					m_value_array->set(idx, 0);
					m_state_array->set(idx, STATE_ERROR);
				}
				if(m_error_count + m_success_count >= m_state_array->size()){
					if(m_callback){
						const int64_t ret =m_callback(this);
						done();
						return ret;
					}
					else{
						done();
						return Command::STATE_ERROR;
					}
				}
				else{
					return Command::STATE_PROCESSING;
				}
			}
			else if(st == STATE_COMPLETED){
				WARN("duplicate recv msg");
				return Command::STATE_PROCESSING;
			}
			else{
				WARN("duplicate recv msg");
				return Command::STATE_PROCESSING;
			}
		}
		else{
			FATAL("wtf");
			done();
			return Command::STATE_ERROR;
		}
	}

	/** callback **/
	void CallbackGroupRpcInfo::setContext(Object* context){
		ASSIGN_POINTER(m_context, context);
	}
	Object* CallbackGroupRpcInfo::getContext(){
		return m_context;
	}
	void CallbackGroupRpcInfo::setCallback(PFN_CALLBACK pfn){
		m_callback =pfn;
	}
	CallbackGroupRpcInfo::PFN_CALLBACK CallbackGroupRpcInfo::getCallback(){
		return m_callback;
	}

	/** rpc **/
	bool CallbackGroupRpcInfo::rpc(PACKET& packet, Object* req_param, CallbackRpcInfo* rpc_info){
		if(m_sn <=0 || !rpc_info){
			return false;
		}
		// set sn & sub sn
		packet.sn =m_sn;
		packet.sub_sn =static_cast<uint64_t>(m_state_array->size() + 1);

		ASSERT(dynamic_cast< CallbackService* >(Service::Current()));

		// send
		if(DispatcherManager::RequestByObject(Service::Current(), packet, req_param)){
			m_rpc_array->push_back(rpc_info);
			m_state_array->push_back(STATE_PROCESSING);
			m_value_array->push_back(0);
			return true;
		}
		else{
			return false;
		}
	}
	bool CallbackGroupRpcInfo::rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, CallbackRpcInfo* rpc_info){
		PACKET packet;
		packet.size =0;
		packet.from =m_id;
		packet.to =to;
		packet.who =who;
		packet.command =cmd;
		packet.option =0;
		return rpc(packet, req_param, rpc_info);
	}
	/** init group **/
	bool CallbackGroupRpcInfo::begin(){
		CallbackService* service =dynamic_cast< CallbackService* >(Service::Current());
		ASSERT(service);
		m_sn =service->setRpcGroup(this);
		return m_sn > 0;
	}
	void CallbackGroupRpcInfo::cancel(){
		if(m_sn > 0){
			CallbackService* service =dynamic_cast< CallbackService* >(Service::Current());
			ASSERT(service);
			service->removeRpcGroup(m_sn);
		}
	}
	void CallbackGroupRpcInfo::end(){
		// do nothing
	}
	/** getter **/
	int64_t CallbackGroupRpcInfo::getCount(){
		return m_state_array->size();
	}
	int64_t CallbackGroupRpcInfo::getState(const int64_t idx){
		if(!(idx>=0 && idx<m_state_array->size())){
			return STATE_ERROR;
		}
		return m_state_array->get(idx);
	}
	Object* CallbackGroupRpcInfo::getValue(const int64_t idx){
		if(!(idx>=0 && idx<m_value_array->size())){
			return 0;
		}
		return m_value_array->get(idx);
	}
	CallbackRpcInfo* CallbackGroupRpcInfo::getRpcInfo(const int64_t idx){
		if(!(idx>=0 && idx<m_rpc_array->size())){
			return 0;
		}
		return static_cast< CallbackRpcInfo* >(m_rpc_array->get(idx));
	}
	int64_t CallbackGroupRpcInfo::getSuccessCount(){
		return m_success_count;
	}
	int64_t CallbackGroupRpcInfo::getErrorCount(){
		return m_error_count;
	}
}
