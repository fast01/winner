/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpConnection ***/
	/** ctor & dtor **/
	HttpConnection::HttpConnection(const int64_t conn_id, const int64_t service_id)
		: TcpConnection(conn_id)
		, m_header_length(0)
		, m_content_length(0)
		, m_packet_length(0)
		, m_service_id(service_id){
		if(m_service_id <= 0){
			m_service_id =HttpService::SERVICE_ID;
		}
		setHeartBeatTimer(HTTP_CONNECTION_KEEP_ALIVE_TIME);
	}
	HttpConnection::~HttpConnection(){
	}

	/** Object **/
	void HttpConnection::init(){
		Super::init();
	}
	void HttpConnection::finalize(){
		Super::finalize();
	}
	/** TcpConnection **/
	int64_t HttpConnection::on_auth(char* data, const int64_t s){
		setAuthed();
		return on_recv(data, s);
	}
	int64_t HttpConnection::on_recv(char* data, const int64_t s){
		// calc http request package length
		if(m_packet_length == 0){
			BinaryCoder<2048> coder;
			coder.append(data, s);
			coder.appendNull();

			char* sz =coder.c_str();
			const char* s1 =strstr(sz, "\r\nContent-Length: ");
			const char* s2 =strstr(sz, "\r\n\r\n");
			if(s2){
				if(s1 && s1<s2){
					long long len =0;
					if(1 != sscanf(s1+18, "%lld", &len)){
						return -1;
					}
					m_content_length =len;
				}
				else{
					m_content_length =0;
				}
				m_header_length =s2+4 - sz;
				m_packet_length =m_header_length + m_content_length;
			}
		}

		// dispatch
		if(s >= m_packet_length){
			PACKET packet;
			memset(&packet, 0, sizeof(PACKET));
			packet.size =sizeof(PACKET) + m_packet_length;
			packet.from =0;
			packet.to =getServiceId();
			packet.who =0;
			packet.option =OPT_REQUEST;
			packet.sn =0;
			packet.command =HttpService::SERVICE_HTTP_PROCESS_REQUEST;
			DispatcherManager::Request(toRequestor(), packet, data, m_packet_length);
			
			// clean
			const int64_t ret =m_packet_length;
			m_header_length =0;
			m_content_length =0;
			m_packet_length =0;
			return ret;
		}
		return 0;
	}
	/** self **/
	int64_t HttpConnection::getServiceId(){
		return m_service_id;
	}
	void HttpConnection::setServiceId(const int64_t id){
		m_service_id =id;
	}
}
