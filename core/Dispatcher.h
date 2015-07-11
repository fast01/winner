/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_DISPATCHER_H__
#define H_CORE_DISPATCHER_H__

namespace core{
	/** Dispatcher **/
	class Dispatcher: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		enum{
			SENDV_FLAG_REQUEST,
			SENDV_FLAG_NOTIFY,
			SENDV_FLAG_REPLY,
		};
	protected:
		enum{
			CACHE_SIZE =1024 * 10
		};
	protected:
		Dispatcher();
		virtual ~Dispatcher();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual bool request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len) =0;
		virtual bool requestByObject(Requestor* requestor, PACKET& packet, Object* obj) =0;
	public:
		virtual bool notify(PACKET& packet, void* body, const int64_t body_len) =0;
		virtual bool notifyByObject(PACKET& packet, Object* obj) =0;
	public:
		virtual bool reply(PACKET& packet, void* body, const int64_t body_len) =0;
		virtual bool replyByObject(PACKET& packet, Object* obj) =0;
	public:
		virtual bool isMatch(const int64_t id) =0;
	protected:
		char* object_to_bytes(Object* obj, char* data, int64_t& len);
		char* _bytes_to_pointer(Bytes* bs, char* data, int64_t& len);
	private:
		Bytes* m_bytes;
	protected:
		typedef DEFAULT_LOCK_TYPE LOCK_TYPE;
		LOCK_TYPE m_lock;
	};
}

#endif
