/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_REQUEST_H__
#define H_CORE_HTTP_REQUEST_H__

namespace core{
	/** HttpRequest **/
	class HttpRequest: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		HttpRequest();
		virtual ~HttpRequest();
	public:
		virtual void init();
		virtual void finalize();
	public:
		String* getMethod();
		String* getPath();
		String* getVersion();
	public:
		Url* getUrl();
	public:
		String* getHeader(String* name);
		Hash* getHeaderTable();

		HttpCookie* getCookie(String* name);
		Hash* getCookieTable();
	public:
		Bytes* getContent();
	public:
		String* getPost(String* name);
		Hash* getPostTable();
	public:
		String* getGet(String* name);
		String* getRequest(String* name);
	public:
		static HttpRequest* Parse(const char* str);
	private:
		bool _parse_mime();
	private:
		// head line
		String* m_method;
		String* m_path;
		String* m_version;

		// url
		Url* m_url;

		// header & cookie
		Hash* m_header_tb;
		Hash* m_cookie_tb;

		// content
		Bytes* m_content;

		// mime
		Hash* m_post_tb;
	};
}

#endif
