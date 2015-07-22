/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Command ***/
	/** ctor & dtor **/
	Command::Command(const int64_t id, const PACKET& packet, Bytes* body, Requestor* requestor, Service* service)
		: m_id(id)
		, m_requestor(requestor)
		, m_packet(packet)
		, m_body(body)
		, m_state(STATE_INIT)
		, m_timestamp(DateTime::Now())
		, m_request(0)
		, m_respond(0)
		, m_respond_command(0)
		, m_service(service){
		RETAIN_POINTER(m_requestor);
		RETAIN_POINTER(m_body);
		ASSERT(m_service);
	}
	Command::~Command(){
	}

	/** impl Object **/
	void Command::init(){
		Super::init();
	}
	void Command::finalize(){
		CLEAN_POINTER(m_requestor);
		CLEAN_POINTER(m_body);
		CLEAN_POINTER(m_request);
		CLEAN_POINTER(m_respond);
		Super::finalize();
	}

	/** basic **/
	int64_t Command::getId(){
		return m_id;
	}
	Requestor* Command::getRequestor(){
		return m_requestor;
	}
	Bytes* Command::getBody(){
		return m_body;
	}
	Service* Command::getService(){
		return m_service;
	}
	int64_t Command::getTimestamp(){
		return m_timestamp;
	}
	bool Command::isTimeout(const int64_t now){
		return now-m_timestamp > TIMEOUT_SECS;
	}

	/** packet **/
	PACKET Command::getPacket(){
		return m_packet;
	}
	int64_t Command::getFrom(){
		return m_packet.from;
	}
	int64_t Command::getTo(){
		return m_packet.to;
	}
	int64_t Command::getWho(){
		return m_packet.who;	
	}
	int64_t Command::getCommand(){
		return m_packet.command;	
	}
	int64_t Command::getOption(){
		return m_packet.option;
	}
	int64_t Command::getSn(){
		return m_packet.sn;
	}

	/** state **/
	int64_t Command::getState(){
		return m_state;
	}
	void Command::setState(const int64_t state){
		m_state =state;
	}
	bool Command::isInit(){
		return m_state == STATE_INIT;
	}
	bool Command::isProcessing(){
		return m_state == STATE_PROCESSING;	
	}
	bool Command::isCompleted(){
		return m_state == STATE_COMPLETE;	
	}

	/** request & respond **/
	void Command::setRequest(Object* request){
		ASSIGN_POINTER(m_request, request);
	}
	Object* Command::getRequest(){
		return m_request;
	}
	int64_t Command::getRequestCommand(){
		return m_packet.command;
	}

	void Command::setRespond(Object* respond){
		ASSIGN_POINTER(m_respond, respond);
	}
	Object* Command::getRespond(){
		return m_respond;
	}
	void Command::setRespondCommand(const int64_t cmd){
		m_respond_command =cmd;
	}
	int64_t Command::getRespondCommand(){
		return m_respond_command;
	}
	/** reply **/
	bool Command::reply(){
		// calc res_cmd
		int64_t res_cmd =0;
		if(ProtocolBase* proto =dynamic_cast< ProtocolBase* >(m_respond)){
			res_cmd =proto->id();
		}
		// reply
		return reply(res_cmd);
	}
	bool Command::reply(const int64_t res_cmd){
		// check arg
		if(!m_requestor){
			ERROR("command fail to reply, m_requestor is null");
			return false;
		}
		// reply
		if(m_respond){
			if(Bytes* bs =dynamic_cast< Bytes* >(m_respond)){
				return reply(res_cmd, bs->data(), bs->size());
			}
			else{
				// make packet
				PACKET res_packet;
				res_packet.from    =m_packet.to;
				res_packet.to      =m_packet.from;
				res_packet.who     =m_packet.who;
				res_packet.sn      =m_packet.sn;
				res_packet.command =res_cmd;
				res_packet.option  =static_cast<uint64_t>(OPT_BODY_IS_OBJECT_POINTER);
				// reply
				return m_requestor->replyByObject(res_packet, m_respond);
			}
		}
		else{
			return reply(res_cmd, 0, 0);
		}
	}
	bool Command::reply(const int64_t res_cmd, void* body, const int64_t body_len){
		// check arg
		if(res_cmd <= 0){
			ERROR("command fail to reply, res_cmd invalid");
			return false;
		}
		if(!m_requestor){
			ERROR("command fail to reply, m_requestor is null");
			return false;
		}
		// make packet
		PACKET res_packet;
		res_packet.from    =m_packet.to;
		res_packet.to      =m_packet.from;
		res_packet.who     =m_packet.who;
		res_packet.sn      =m_packet.sn;
		res_packet.command =res_cmd;
		res_packet.option  =0;
		// reply
		return m_requestor->reply(res_packet, body, body_len);
	}
}
