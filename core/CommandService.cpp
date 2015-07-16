/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** CommandService impl ***/
	/** ctor & dtor **/
	CommandService::CommandService()
		: m_queue_tb(0)
		, m_command_id(0)
		, m_processing_command(0)
		, m_rpc_tb(0)
		, m_rpc_id(0)
		, m_protocol_group_id(0)
		, m_process_timeout_last_time(0)
		, m_optimize_last_time(0){
	}
	CommandService::~CommandService(){
	}

	/* Object */
	void CommandService::init(){
		Super::init();
		ASSIGN_POINTER(m_queue_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_rpc_tb, SafeNew<Hash>());
	}
	void CommandService::finalize(){
		ASSERT(m_processing_command == 0);
		CLEAN_POINTER(m_queue_tb);
		CLEAN_POINTER(m_rpc_tb);
		Super::finalize();
	}

	/** Service **/
	void CommandService::on_update(const int64_t now){
		Super::on_update(now);
		if(now - m_process_timeout_last_time >= PROCESS_TIMEOUT_TIMER){
			m_process_timeout_last_time =now;
			_process_timeout(now);
		}
		if(now - m_optimize_last_time >= OPTIMIZE_TIMER){
			m_optimize_last_time =now;
			_optimize(now);
		}
	}
	void CommandService::on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
		Super::on_message(requestor, packet, body, body_len);
		// make command
		Command* command;
		GENERATE_ID(m_command_id);

		if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
			ASSERT(static_cast<size_t>(body_len) >= sizeof(Object*));
			Object* obj =*reinterpret_cast< Object** >(body);
			command =SafeNew<Command>(m_command_id, packet, static_cast< Bytes* >(0), requestor, this);
			command->setRequest(obj);
		}
		else{
			Bytes* bs =SafeNew< Bytes >();
			bs->set(body, body_len);
			command =SafeNew<Command>(m_command_id, packet, bs, requestor, this);
		}

		// dispatch
		if(packet.option & OPT_REQUEST){
			_append_request(command);
			_process_request(packet.who);
		}
		else if(packet.option & OPT_RESPOND){
			_process_respond(command);
		}
	}

	/* self */
	Command* CommandService::getProcessingCommand(){
		return m_processing_command;
	}
	void CommandService::setProtocolGroupId(const int64_t proto_grp_id){
		m_protocol_group_id =proto_grp_id;
	}
	int64_t CommandService::getProtocolGroupId(){
		return m_protocol_group_id;
	}
	/** event **/
	int64_t CommandService::on_start_command(Command* command){
		return Command::STATE_ERROR;
	}
	/** rpc **/
	int64_t CommandService::set_rpc(RpcInfo* rpc){
		ASSERT(rpc);
		if(rpc->getId() > 0){
			ERROR("RpcInfo object can't be reuse");
			return 0;
		}
		const int64_t expire_time =DateTime::Now() + RpcInfo::TTL;
		const int64_t rpc_id =_gen_rpc_id();
		rpc->set(rpc_id, m_processing_command, expire_time);
		m_rpc_tb->set(rpc_id, rpc);
		return rpc_id;
	}
	void CommandService::del_rpc(const int64_t rpc_id){
		m_rpc_tb->remove(rpc_id);
	}
	RpcInfo* CommandService::_get_rpc(const int64_t rpc_id){
		return static_cast< RpcInfo* >(m_rpc_tb->get(rpc_id));
	}
	int64_t CommandService::_gen_rpc_id(){
		GENERATE_ID(m_rpc_id);
		return m_rpc_id;
	}
	int64_t CommandService::GenRpcId(){
		CommandService* self =dynamic_cast< CommandService* >(Service::Current());
		if(self){
			return self->_gen_rpc_id();
		}
		else{
			return -1;
		}
	}
	/** private **/
	void CommandService::_append_request(Command* command){
		// get queue
		const int64_t who =command->getWho();
		Array* queue =static_cast< Array* >(m_queue_tb->get(who));
		if(queue == 0){
			queue =SafeNew<Array>();
			m_queue_tb->set(who, queue);
		}
		queue->push_back(command);
		queue->optimize(0);
	}
	void CommandService::_process_request(const int64_t who){
		// get queue
		Array* queue =static_cast< Array* >(m_queue_tb->get(who));
		if(!queue) return;

		// process
		int64_t last_pop_cmd =-1;
		while(queue->size() > 0){
			Command* front =static_cast< Command* >(queue->front());

			// cancel timeout
			if(front->isTimeout(m_now)){
				last_pop_cmd =front->getCommand();
				queue->pop_front();
				WARN("service %s(%lld) who %lld command %lld, timeout just remove it", name(), (long long)m_id, (long long)who, (long long)last_pop_cmd);
				continue;
			}

			// break when front is processing
			if(front->isProcessing()){
				break;
			}

			// must be init
			ASSERT(front->isInit());

			// process front
			m_processing_command =front;
			const int64_t result =on_start_command(front);
			m_processing_command =0;
			if(result == Command::STATE_COMPLETE){
				front->setState(Command::STATE_COMPLETE);
				last_pop_cmd =front->getCommand();
				queue->pop_front();
				INFO("service %s(%lld) who %lld command %lld complete", name(), (long long)m_id, (long long)who, (long long)last_pop_cmd);
			}
			else if(result > 0){
				front->setState(Command::STATE_PROCESSING);
				INFO("service %s(%lld) who %lld command %lld processing", name(), (long long)m_id, (long long)who, (long long)front->getCommand());
				break;
			}
			else{
				front->setState(Command::STATE_ERROR);
				last_pop_cmd =front->getCommand();
				queue->pop_front();
				ERROR("service %s(%lld) who %lld command %lld state %lld", name(), (long long)m_id, (long long)who, (long long)last_pop_cmd, (long long)result);
			}
		}
		// post process
		if(last_pop_cmd==LOGOUT_REQUEST && queue->empty()){
			m_queue_tb->remove(who);
		}
	}
	void CommandService::_process_respond(Command* rpc_res){
		const int64_t who =static_cast<int64_t>(rpc_res->getWho());
		const int64_t rpc_id =static_cast<int64_t>(rpc_res->getSn());
		bool done =true;
		do{
			RpcInfo* rpc =_get_rpc(rpc_id);
			if(rpc == 0){
				WARN("service %s(%lld) who %lld rpc %lld respond, not exist", name(), (long long)m_id, (long long)who, (long long)rpc_id);
				break;
			}
			if(Command* cmd=rpc->getCommand()){
				// check & prepare command
				Array* queue =static_cast< Array* >(m_queue_tb->get(who));
				if(queue == 0){
					WARN("service %s(%lld) who %lld rpc %lld respond, command not exist", name(), (long long)m_id, (long long)who, (long long)rpc_id);
					break;
				}
				Command* front =static_cast< Command* >(queue->front());
				if(!front){
					WARN("service %s(%lld) who %lld rpc %lld respond, command not exist", name(), (long long)m_id, (long long)who, (long long)rpc_id);
					break;
				}
				if(front != cmd){
					WARN("service %s(%lld) who %lld rpc %lld respond, command mismatch", name(), (long long)m_id, (long long)who, (long long)rpc_id);
					break;
				}

				// call rpc
				m_processing_command =front;
				const int64_t result =rpc->invoke(rpc_res);
				m_processing_command =0;

				// done
				if(rpc->isDone() == false){
					done =false;
				}

				// process result
				const int64_t cmd_id =front->getCommand();
				if(result == Command::STATE_COMPLETE){
					front->setState(Command::STATE_COMPLETE);
					queue->pop_front();
					INFO("service %s(%lld) who %lld rpc %lld respond, command %lld complete", name(), (long long)m_id, (long long)who, (long long)rpc_id, (long long)cmd_id);
				}
				else if(result > 0){
					front->setState(Command::STATE_PROCESSING);
					INFO("service %s(%lld) who %lld rpc %lld respond, command %lld processing", name(), (long long)m_id, (long long)who, (long long)rpc_id, (long long)cmd_id);
				}
				else{
					front->setState(Command::STATE_ERROR);
					queue->pop_front();
					ERROR("service %s(%lld) who %lld rpc %lld respond, command %lld error %lld", name(), (long long)m_id, (long long)who, (long long)rpc_id, (long long)cmd_id, (long long)result);
				}

				// post process
				if(cmd_id==LOGOUT_REQUEST && queue->empty()){
					m_queue_tb->remove(who);
				}
				else if(queue->size()){
					_process_request(who);
				}
			}
			else{
				const int64_t result =rpc->invoke(rpc_res);
				if(result == Command::STATE_COMPLETE){
					INFO("service %s(%lld) who %lld rpc %lld respond, complete", name(), (long long)m_id, (long long)who, (long long)rpc_id);
				}
				else if(result > 0){
					INFO("service %s(%lld) who %lld rpc %lld respond, processing", name(), (long long)m_id, (long long)who, (long long)rpc_id);
				}
				else{
					ERROR("service %s(%lld) who %lld rpc %lld respond, error %lld", name(), (long long)m_id, (long long)who, (long long)rpc_id, (long long)result);
				}
			}
		}while(0);
		// remove rpc info
		if(done){
			m_rpc_tb->remove(rpc_id);
		}
	}
	void CommandService::_process_timeout(const int64_t now){
		/* clear timeout command */
		if(m_queue_tb->size()>0){
			Int64Array* ls =0;
			HashIterator* it =static_cast< HashIterator* >(m_queue_tb->iterator());
			while(it->next()){
				const int64_t who =static_cast< Int64* >(it->getKey())->getValue();
				Array* queue =static_cast< Array* >(it->getValue());
				while(Command* cmd =dynamic_cast< Command* >(queue->front())){
					if(cmd->isTimeout(now)){
						if(cmd->isProcessing()){
							WARN("service %s(%lld) who %lld command %lld cancel", name(), (long long)m_id, (long long)cmd->getWho(), (long long)cmd->getCommand());
						}
						queue->pop_front();
					}
					else{
						break;
					}
				}
				if(Command* cmd =dynamic_cast< Command* >(queue->front())){
					if(cmd->isProcessing()){
						continue;
					}
					ASSERT(cmd->isInit());
					if(!ls){
						ls =SafeNew<Int64Array>();
					}
					ls->push_back(who);
				}
			}
			const int64_t n= ls ? ls->size() : 0;
			for(int64_t i=0; i<n; ++i){
				_process_request(ls->get(i));
			}
		}
		/* clear timeout rpc */
		if(m_rpc_tb->size()>0){
			Array* ls =0;
			HashIterator* it =static_cast< HashIterator* >(m_rpc_tb->iterator());
			while(it->next()){
				RpcInfo* ri =static_cast< RpcInfo* >(it->getValue());
				if(now >= ri->getExpireTime()){
					if(!ls){
						ls =SafeNew<Array>();
					}
					ls->push_back(ri);
					it->remove();
				}
			}
			const int64_t n =ls ? ls->size() : 0;
			for(int64_t i=0; i<n; ++i){
				RpcInfo* ri =static_cast< RpcInfo* >(ls->get(i));
				WARN("service %s(%lld) rpc %lld cancel", name(), (long long)m_id, (long long)ri->getId());
				ri->timeout();
			}
		}
	}

	void CommandService::_optimize(const int64_t now){
		if(m_queue_tb->size() > OPTIMIZE_THRESHOLD){
			HashIterator* it =static_cast< HashIterator* >(m_queue_tb->iterator());
			while(it->next()){
				Array* queue =static_cast< Array* >(it->getValue());
				if(queue->empty()){
					it->remove();
				}
			}
			m_queue_tb->optimize(0);
		}
	}
}
