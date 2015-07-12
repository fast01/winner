/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_RESPOND_H__
#define H_CORE_HTTP_RESPOND_H__

namespace core{
	/** HttpRespond **/
	class HttpRespond: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		HttpRespond(Requestor* requestor);
		virtual ~HttpRespond();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setVersion(String* version);
		String* getVersion();

		void setCode(const int64_t code);
		int64_t getCode();

		void setMsg(String* msg);
		String* getMsg();
	public:
		void setHeader(String* name, String* value);
		String* getHeader(String* name);
		Hash* getHeaderTable();

		void setCookie(String* name, String* value, const int64_t max_age);
		void setCookie(String* name, HttpCookie* cookie);
		HttpCookie* getCookie(String* name);
		Hash* getCookieTable();
	public:
		void write(const char* str);
		void write(const char* str, int64_t len);
		void write(String* str);
		Bytes* getContent();
	public:
		String* getPost(String* name);
		Hash* getPostTable();
	public:
		void enableAutoFlush(const bool yes);
		bool isEnableAutoFlush();
	public:
		Requestor* getRequestor();
		bool flush();
		void close();
	public:
		static HttpRespond* Parse(const char* str);
	private:
		const char* _code_to_msg(const int64_t code);
		bool _parse_mime();
	private:
		// head line
		String* m_version;
		int64_t m_code;
		String* m_msg;

		// header
		Hash* m_header_tb;
		Hash* m_cookie_tb;

		// content
		Bytes* m_content;

		// mime
		Hash* m_post_tb;

		// misc
		Requestor* m_requestor;
		bool m_auto_flush;
	};
}

#endif
