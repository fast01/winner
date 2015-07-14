/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl TcpConnectionRequestor ***/
	/** ctor & dtor **/
	TcpConnectionRequestor::TcpConnectionRequestor(const int64_t id, BytesChannel* conn_send_buffer)
		: m_id(id)
		, m_conn_send_buffer(conn_send_buffer){
		RETAIN_POINTER(m_conn_send_buffer);
	}
	TcpConnectionRequestor::~TcpConnectionRequestor(){
	}
	/** Object **/
	void TcpConnectionRequestor::init(){
		Super::init();
	}
	void TcpConnectionRequestor::finalize(){
		CLEAN_POINTER(m_conn_send_buffer);
		Super::finalize();
	}

	/** Requestor **/
	int64_t TcpConnectionRequestor::getId(){
		return m_id;
	}
	bool TcpConnectionRequestor::reply(PACKET& packet, void* body, const int64_t body_len){
		// make packet
		packet.size =body_len;
		packet.option &= (~OPT_REQUEST);
		packet.option &= (~OPT_BODY_IS_OBJECT_POINTER);
		packet.option |= OPT_RESPOND;
		char sz[PACKET_ENCODE_MAX_SIZE] ={0};
		const int64_t n =encode_packet(sz, sizeof(sz), packet);
		ASSERT(n > 0);
		// push
		MEMORY_SLICE mv[2] ={
			{ sz, n },
			{ reinterpret_cast< char* >(body), body_len }
		};
		if(m_conn_send_buffer->pushv(mv, 2)){
			// m_conn_send_buffer->signal();
			return true;
		}
		else{
			return false;
		}
	}
	bool TcpConnectionRequestor::replyByObject(PACKET& packet, Object* obj){
		Bytes* bs =obj ? obj->toBytes(0) : 0;
		void* body =bs ? bs->data() : 0;
		const int64_t body_len =bs ? bs->size() : 0;
		return reply(packet, body, body_len);
	}
	bool TcpConnectionRequestor::sendv(const MEMORY_SLICE* slice, const int64_t n){
		if(m_conn_send_buffer->pushv(slice, n)){
			// m_conn_send_buffer->signal();
			return true;
		}
		else{
			return false;
		}
	}
}

