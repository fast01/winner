/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTTP_CLIENT_FOR_LUA_H__
#define H_CORE_HTTP_CLIENT_FOR_LUA_H__

namespace core{
	/** HttpClientForLua **/
	class HttpClientForLua: public TcpConnection{
		SUPPORT_NEWABLE
		DECLARE_SUPPORT_SCRIPT
		typedef TcpConnection Super;
	private:
		HttpClientForLua();
		virtual ~HttpClientForLua();
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
	private:
		static int _Send(lua_State* L);
		static int _gc(lua_State* L);
	private:
		bool _send(const char* host, const char* content, const int64_t content_len);
		void _resume_coroutine(HttpRespond* res);
	public:
		static bool RegisterToLua(lua_State* L);
	private:
		int64_t m_header_length;
		int64_t m_content_length;
		int64_t m_packet_length;
		int64_t m_lua_cr_id;
	};
}

#endif
