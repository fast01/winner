/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_COOKIE_H__
#define H_CORE_HTTP_COOKIE_H__

namespace core{
	/** predecl **/
	class HttpConnection;
	class HttpListener;
	class HttpService;

	/** HttpCookie **/
	class HttpCookie: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		HttpCookie();
		virtual ~HttpCookie();
	public:
		virtual void init();
		virtual void finalize();
	public:
		DECLARE_PROPERTY(String*, Name)
		DECLARE_PROPERTY(String*, Value)
		DECLARE_PROPERTY(String*, Domain)
		DECLARE_PROPERTY(String*, Path)
		DECLARE_PROPERTY(int64_t, MaxAge)
		DECLARE_PROPERTY(String*, ExpireTime)
		DECLARE_PROPERTY(bool, Secure)
		DECLARE_PROPERTY(bool, HttpOnly)
	public:
		String* build(const bool client);
	public:
		static HttpCookie* Parse(const char* str);
	};
}

#endif
