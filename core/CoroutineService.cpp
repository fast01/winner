/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CoroutineService ***/
	/** ctor & dtor **/
	CoroutineService::CoroutineService()
		: m_command_desc_table(0)
		, m_sleeper_table(0)
		, m_cr_pool(0){
	}
	CoroutineService::~CoroutineService(){}

	/** New **/
	Service* CoroutineService::New(){
		return SafeNew<CoroutineService>();
	}

	/** Service **/
	bool CoroutineService::on_load(){
		if(!Super::on_load()) return false;
		OPH();
		ASSIGN_POINTER(m_name, STR("CoroutineService"));
		ASSIGN_POINTER(m_command_desc_table, SafeNew<Hash>());
		ASSIGN_POINTER(m_sleeper_table, SafeNew<Hash>());
		ASSIGN_POINTER(m_cr_pool, SafeNew<CoroutinePool>());
		register_command();
		return true;
	}
	void CoroutineService::on_unload(){
		CLEAN_POINTER(m_command_desc_table);
		CLEAN_POINTER(m_sleeper_table);
		CLEAN_POINTER(m_cr_pool);
		Super::on_unload();
	}
	int64_t CoroutineService::on_start_command(Command* command){
		Super::on_start_command(command);
		PACKET packet =command->getPacket();
		const int64_t grp_id =m_protocol_group_id;
		const int64_t who =command->getWho();
		const int64_t req_cmd =command->getRequestCommand();
		if(CoroutineCommandDesc* desc =static_cast< CoroutineCommandDesc* >(m_command_desc_table->get(req_cmd))){
			//// preprocess command
			const int64_t res_cmd =desc->getRespondCommand();
			Bytes* body =command->getBody();
			ProtocolManager* pm =ProtocolManager::Instance();

			//// try code as protocol
			if(desc->isUseProtocol()){
				// request
				if(!(packet.option & OPT_BODY_IS_OBJECT_POINTER)){
					ProtocolBase* req =pm->createProtocol(grp_id, req_cmd);
					if(!req){
						ERROR("service %s(%lld) who %lld fail to start command %lld, create request group %lld protocol %lld error", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)grp_id, (long long)req_cmd);
						return Command::STATE_ERROR;
					}
					if(!req->fromBytes(body)){
						ERROR("service %s(%lld) who %lld fail to start command %lld, unmarsh request group %lld protocol %lld error", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)grp_id, (long long)req_cmd);
						return Command::STATE_ERROR;
					}
					command->setRequest(req);
				}

				// respond
				if(res_cmd > 0){
					ProtocolBase* res =pm->createProtocol(grp_id, res_cmd);
					if(!res){
						ERROR("service %s(%lld) who %lld fail to start command %lld, create respond group %lld protocol %lld error", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)grp_id, (long long)res_cmd);
						return Command::STATE_ERROR;
					}
					command->setRespond(res);
					command->setRespondCommand(res->id());
				}
			}

			//// go coroutine
			int64_t cr_new_id=0;
			const int64_t result =m_cr_pool->go(desc->getCallback(), command, cr_new_id);
			if(result >= 0){
				if(result == Coroutine::STATUS_IDLE){
					return Command::STATE_COMPLETE;
				}
				else if(result == Coroutine::STATUS_WAITING){
					return cr_new_id;
				}
				else{
					return Command::STATE_ERROR;
				}
			}
			else{
				ERROR("service %s(%lld) who %lld fail to start command %lld, error code %lld", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)-result);
				return Command::STATE_ERROR;
			}
		}
		else{
			ERROR("service %s(%lld) who %lld fail to start command %lld, not config", name(), (long long)m_id, (long long)packet.who, (long long)req_cmd);
			return core::Command::STATE_ERROR;
		}
	}
	void CoroutineService::on_update(const int64_t now){
		Super::on_update(now);
		// wakeup sleeper
		if(m_sleeper_table->size() > 0){
			// prepare wake up list
			Int64Array* wake_list =0;
			HashIterator* it =static_cast< HashIterator* >(m_sleeper_table->iterator());
			while(it->next()){
				Int64* cr_id =static_cast< Int64* >(it->getKey());
				Int64* expire_time =static_cast< Int64* >(it->getValue());
				if(expire_time->getValue() > now){
					if(!wake_list){
						wake_list =SafeNew<Int64Array>();
					}
					wake_list->push_back(cr_id->getValue());
					it->remove();
				}
			}

			// wake up
			const int64_t n =wake_list ? wake_list->size() : 0;
			for(int64_t i=0; i<n; ++i){
				_resume_coroutine(wake_list->get(i), SafeNew<Error>(ErrorCode::TIMEOUT), 0);
			}
		}

		// update
		int64_t cr_new_id=0;
		m_cr_pool->go(_update, 0, cr_new_id);
	}
	/** register command **/
	void CoroutineService::register_command(){
	}
	void CoroutineService::on(const int64_t cmd, CoroutineCommandDesc* desc){
		m_command_desc_table->set(cmd, desc);
	}
	void CoroutineService::on(const int64_t cmd, const bool is_use_protocol, Coroutine::PFN_COROUTINE_TASK pfn){
		CoroutineCommandDesc* desc =SafeNew<CoroutineCommandDesc>();
		desc->setCallback(pfn);
		desc->setUseProtocol(is_use_protocol);
		on(cmd, desc);
	}
	void CoroutineService::on(const int64_t cmd, Coroutine::PFN_COROUTINE_TASK pfn){
		on(cmd, true, pfn);
	}
	/** rpc **/
	Object* CoroutineService::rpc(const int64_t who, const int64_t to, const int64_t cmd, const int64_t res_proto_grp_id, Object* req_param){
		if(to == getId()){
			ERROR("service %s(%lld) can't request self", name(), (long long)m_id);
			return 0;
		}
		// prepare
		Coroutine* cr =Coroutine::Running();
		if(!cr){
			WARN("service %s(%lld) %lld fail to rpc to %lld, in main thread", name(), (long long)m_id, (long long)who, (long long)to);
			return 0;
		}
		ASSERT(cr->canYield());
		const int64_t cr_id =cr->getId();

		// set rpc info
		CoroutineRpcInfo* rpc_info =SafeNew<CoroutineRpcInfo>(res_proto_grp_id, cr_id);
		if(!set_rpc(rpc_info)){
			return 0;
		}
		const int64_t rpc_id =rpc_info->getId();

		// prepare packet
		PACKET packet;
		packet.from =getId();
		packet.to =to;
		packet.who =who;
		packet.sn =rpc_id;
		packet.sub_sn =0;
		packet.command =cmd;
		packet.option =0;

		// request
		if(!DispatcherManager::RequestByObject(this, packet, req_param)){
			WARN("service %s(%lld) %lld fail to rpc to %lld, service not ready", name(), (long long)m_id, (long long)who, (long long)to);
			return 0;
		}
		rpc_info->set(rpc_id, this);
		ENSURE(cr->yield(0, rpc_id));

		// process respond
		if(Command* respond =dynamic_cast< Command* >(cr->getResumeParam())){
			// prepare
			const PACKET res_packet =respond->getPacket();
			if(res_packet.sn != static_cast<uint64_t>(rpc_id)){
				WARN("service %s(%lld) %lld fail to rpc to %lld, rpc id mismatch", name(), (long long)m_id, (long long)who, (long long)to);
				return 0;
			}
			// body is object pointer
			if(res_packet.option & OPT_BODY_IS_OBJECT_POINTER){
				return respond;
			}
			// body is protocol
			if(res_proto_grp_id>0){
				Bytes* res_bs =respond->getBody();
				const int64_t res_cmd =static_cast<int64_t>(res_packet.command);
				const int64_t group_id =res_proto_grp_id;
				const int64_t protocol_id =res_cmd;
				ProtocolBase* res_proto = ProtocolManager::CreateProtocol(group_id, protocol_id);
				if(!res_proto){
					ERROR("service %s(%lld) %lld fail to rpc to %lld, create protocol group %lld id %lld error", name(), (long long)m_id, (long long)who, (long long)to, (long long)group_id, (long long)protocol_id);
					return 0;
				}
				if(!res_proto->fromBytes(res_bs)){
					ERROR("service %s(%lld) %lld fail to rpc to %lld, unmarshal protocol name %s group %lld id %lld error", name(), (long long)m_id, (long long)who, (long long)to, res_proto->name(), (long long)group_id, (long long)protocol_id);
					return 0;
				}
				respond->setRequest(res_proto);
			}
			// body is not special
			return respond;
		}
		else{
			return cr->getResumeParam();
		}
	}
	bool CoroutineService::sleep(const int64_t secs){
		Coroutine* cr =Coroutine::Running();
		if(!cr){
			WARN("service %s(%lld) fail to sleep, in main thread", name(), (long long)m_id);
			return false;
		}
		if(m_processing_command){
			WARN("service %s(%lld) fail to sleep, in processing command", name(), (long long)m_id);
			return false;
		}
		const int64_t cr_id =cr->getId();
		m_sleeper_table->set(cr_id, SafeNew<Int64>(DateTime::Now() + secs));
		ASSERT(cr->yield(0, -1));
		m_sleeper_table->remove(cr_id);
		return true;
	}
	void CoroutineService::update(const int64_t now){
		if(m_cr_pool){
			m_cr_pool->update(now);
		}
	}
	int64_t CoroutineService::resume_coroutine(const int64_t cr_id, Object* param, const int64_t sign){
		return _resume_coroutine(cr_id, param, sign);
	}
	/** crp **/
	CoroutinePool* CoroutineService::getCoroutinePool(){
		return m_cr_pool;
	}
	/** helper **/
	int64_t CoroutineService::_resume_coroutine(const int64_t cr_id, Object* param, const int64_t sign){
		const int64_t result =m_cr_pool->resume(cr_id, param, sign);
		if(result >= 0){
			if(result == Coroutine::STATUS_IDLE){
				return Command::STATE_COMPLETE;
			}
			else if(result == Coroutine::STATUS_WAITING){
				return 1;
			}
			else{
				return Command::STATE_ERROR;
			}
		}
		else{
			ERROR("service %s(%lld) fail to resume rpc %lld, unknown error", name(), (long long)m_id, (long long)cr_id);
			return Command::STATE_ERROR;
		}
	}
	void CoroutineService::_update(Object* arg){
		CoroutineService* self =static_cast< CoroutineService* >(Current());
		self->update(DateTime::Now());
	}
}
