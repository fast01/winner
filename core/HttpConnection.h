/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_CONNECTION_H__
#define H_CORE_HTTP_CONNECTION_H__

namespace core{
	/** HttpConnection **/
	class HttpConnection: public TcpConnection{
		SUPPORT_NEWABLE
		typedef TcpConnection Super;
	private:
		HttpConnection(const int64_t conn_id, const int64_t service_id);
		virtual ~HttpConnection();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual int64_t on_auth(char* data, const int64_t s);
		virtual int64_t on_recv(char* data, const int64_t s);
	public:
		int64_t getServiceId();
		void setServiceId(const int64_t id);
	private:
		int64_t m_header_length;
		int64_t m_content_length;
		int64_t m_packet_length;
		int64_t m_service_id;
	};
}

#endif
