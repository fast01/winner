/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_S6_HTTP_SERVICE_H__
#define H_S6_HTTP_SERVICE_H__

namespace service{
	/** S6HttpService **/
	class S6HttpService: public core::HttpService{
		typedef core::HttpService Super;
		SUPPORT_NEWABLE
		DECLARE_CLASS_INFO
	protected:
		S6HttpService();
		virtual ~S6HttpService();
	public:
		static Service* New();
	protected:
		virtual bool on_load();
		virtual void on_unload();
		virtual void update(const int64_t now);
		virtual void on_request(HttpRequest* request, HttpRespond* respond);
	};
}
#endif
