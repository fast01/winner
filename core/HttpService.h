/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_SERVICE_H__
#define H_CORE_HTTP_SERVICE_H__

namespace core{
	/** HttpService **/
	class HttpService: public CoroutineService{
		SUPPORT_NEWABLE
		typedef CoroutineService Super;
		DECLARE_CLASS_INFO
	public:
		enum{
			SERVICE_ID =5,
			SERVICE_HTTP_PROCESS_REQUEST =1
		};
	protected:
		HttpService();
		virtual ~HttpService();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual void register_command();
	protected:
		virtual void on_request(HttpRequest* request, HttpRespond* respond);
		String* render_html(String* source, Hash* param);
	private:
		static void _process(Object* arg);
	protected:
		HtmlTemplate* m_html_template;
	};
}

#endif
