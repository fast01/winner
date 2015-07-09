/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TCP_CONNECTION_REQUESTOR_H__
#define H_CORE_TCP_CONNECTION_REQUESTOR_H__

namespace core{
	class TcpConnectionRequestor: public Requestor{
		SUPPORT_NEWABLE
		typedef Requestor Super;
	protected:
		TcpConnectionRequestor(const int64_t id, BytesChannel* conn_send_buffer);
		virtual ~TcpConnectionRequestor();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t getId();
		virtual bool reply(PACKET& packet, void* body, const int64_t body_len);
		virtual bool replyByObject(PACKET& packet, Object* obj);
	private:
		int64_t m_id;
		BytesChannel* m_conn_send_buffer;
	};
}
#endif
