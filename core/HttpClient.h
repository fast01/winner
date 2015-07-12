/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_CLIENT_H__
#define H_CORE_HTTP_CLIENT_H__

namespace core{
	/** HttpClient **/
	class HttpClient: public TcpConnection{
		SUPPORT_NEWABLE
		typedef TcpConnection Super;
	private:
		HttpClient();
		virtual ~HttpClient();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual bool reborn();
		virtual bool canReborn();
		virtual void onDetachEvent();
	protected:
		virtual int64_t on_auth(char* data, const int64_t s);
		virtual int64_t on_recv(char* data, const int64_t s);
	public:
		static HttpRespond* Get(String* szurl);
		static HttpRespond* Post(String* szurl, Hash* header, Hash* param);
		static HttpRespond* Request(String* szmethod, String* szurl, String* szversion, Hash* header, Bytes* content);
	private:
		HttpRespond* _get(String* szurl);
		HttpRespond* _post(String* szurl, Hash* header, Hash* param);
		HttpRespond* _request(String* szmethod, String* szurl, String* szversion, Hash* header, Bytes* content);
	private:
		HttpRespond* _yield_coroutine();
		void _resume_coroutine(HttpRespond* res);
		bool _parse_ip_port(String* host, char szip[32], int32_t& port);
	private:
		int64_t m_header_length;
		int64_t m_content_length;
		int64_t m_packet_length;
		Coroutine* m_coroutine;
	};
}

#endif
