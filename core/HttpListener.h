/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_LISTENER_H__
#define H_CORE_HTTP_LISTENER_H__

namespace core{
	/** HttpListener **/
	class HttpListener: public TcpListener{
		SUPPORT_NEWABLE
		typedef TcpListener Super;
		DECLARE_CLASS_INFO
	private:
		HttpListener();
		virtual ~HttpListener();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual TcpConnection* create_connection();
	public:
		virtual bool setExtraParam(String* param);
	public:
		int64_t getServiceId();
		void setServiceId(const int64_t id);
	private:
		int64_t m_service_id;
	};
}

#endif
