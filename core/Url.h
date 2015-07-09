/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_URL_H__
#define H_CORE_URL_H__

namespace core{
	/** predecl **/
	class Hash;

	/** Url **/
	class Url: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_SUPPORT_SCRIPT
	private:
		Url();
		virtual ~Url();
	public:
		static Url* New(); 
	public:
		virtual void init();
		virtual void finalize();
	public:
		bool parse(String* url);
		bool build(String* protocol, String* auth, String* host, String* path, Hash* query, String* fragment);
		bool build(String* protocol, String* auth, String* host, String* path, String* query_string, String* fragment);
		void clean();
	public:
		static bool ParseQuery(String* query_str, Hash* tb);
	private:
		bool _build(String* protocol, String* auth, String* host, String* path, String* query_string, Hash* query, String* fragment);
	public:
		String* getUrl();
		String* getProtocol();
		String* getAuth();
		String* getHost();
		String* getPath();
		String* getQueryString();
		Hash* getQuery();
		String* getFragment();
	private:
		String* m_url;
		String* m_protocol;
		String* m_auth;
		String* m_host;
		String* m_path;
		String* m_query_string;
		Hash* m_query;
		String* m_fragment;
	};
}

#endif
