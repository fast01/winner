/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CallbackService ***/
	/** ctor & dtor **/
	CallbackService::CallbackService()
		: CommandService()
		, m_command_desc_table(0){
	}
	CallbackService::~CallbackService(){}

	/** New **/
	Service* CallbackService::New(){
		return SafeNew<CallbackService>();
	}

	/** Service **/
	bool CallbackService::on_load(){
		if(!Super::on_load()) return false;
		ASSIGN_POINTER(m_command_desc_table, SafeNew<Hash>());
		register_command();
		return true;
	}
	void CallbackService::on_unload(){
		CLEAN_POINTER(m_command_desc_table);
		Super::on_unload();
	}
	int64_t CallbackService::on_start_command(Command* command){
		Super::on_start_command(command);

		PACKET packet =command->getPacket();
		const int64_t who =command->getWho();
		const int64_t req_cmd =command->getRequestCommand();
		if(CallbackCommandDesc* desc =static_cast< CallbackCommandDesc* >(m_command_desc_table->get(req_cmd))){
			//// prepare
			Bytes* body =command->getBody();
			const int64_t grp_id =m_protocol_group_id;
			const int64_t res_cmd =desc->getRespondCommand();
			CallbackCommandDesc::PFN_CALLBACK callback =desc->getCallback();
			
			//// try code as protocol
			if(desc->isUseProtocol()){
				ProtocolManager* pm =ProtocolManager::Instance();
				// request
				if(!(packet.option & OPT_BODY_IS_OBJECT_POINTER)){
					ProtocolBase* req =pm->createProtocol(grp_id, req_cmd);
					if(!req){
						ERROR("service %s(%lld) who %lld fail to start command %lld, create request group %lld protocol %lld error", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)grp_id, (long long)req_cmd);
						return Command::STATE_ERROR;
					}
					if(!req->fromBytes(body)){
						ERROR("service %s(%lld) who %lld fail to start command %lld, decode request group %lld protocol %lld error", name(), (long long)m_id, (long long)who, (long long)req_cmd, (long long)grp_id, (long long)req_cmd);
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

			//// callback
			return callback(command);
		}
		else{
			ERROR("service %s(%lld) who %lld fail to start command %lld, command not found", name(), (long long)m_id, (long long)who, (long long)req_cmd);
			return Command::STATE_ERROR;
		}
	}
	void CallbackService::on_update(const int64_t now){
	}
	/** rpc **/
	bool CallbackService::rpc(PACKET& packet, Object* req_param, RpcInfo* rpc_info){
		if(packet.to == static_cast<uint64_t>(m_id)){
			WARN("service %s(%lld) who %lld fail to rpc to %lld, can't request self", name(), (long long)m_id, (long long)packet.who, (long long)packet.to);
		}
		if(set_rpc(rpc_info) > 0){
			packet.sn =static_cast<uint64_t>(rpc_info->getId());
			if(DispatcherManager::RequestByObject(this, packet, req_param)){
				return true;
			}
			else{
				del_rpc(rpc_info->getId());
				WARN("service %s(%lld) who %lld fail to rpc to %lld, service not ready", name(), (long long)m_id, (long long)packet.who, (long long)packet.to);
			}
		}
		return false;
	}
	bool CallbackService::rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, RpcInfo* rpc_info){
		PACKET packet;
		packet.size =0;
		packet.from =m_id;
		packet.to =to;
		packet.who =who;
		packet.sn =0;
		packet.command =cmd;
		packet.option =0;
		return rpc(packet, req_param, rpc_info);
	}
	/** register command **/
	void CallbackService::on(const int64_t cmd, CallbackCommandDesc* desc){
		m_command_desc_table->set(cmd, desc);
	}
	void CallbackService::register_command(){
	}
}

