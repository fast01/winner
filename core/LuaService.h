/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LUA_SERVICE_H__
#define H_CORE_LUA_SERVICE_H__

namespace core{
	/** LuaService **/
	class LuaService: public Service{
		typedef Service Super;
		SUPPORT_NEWABLE
	private:
		LuaService();
		virtual ~LuaService();
	protected:
		virtual bool load_module(const int64_t id, const char* path);
		virtual void unload_module();
	protected:
		virtual bool on_load();
		virtual void on_update(const int64_t now);
		virtual void on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
		virtual void on_unload();
	private:
		void _on_http_request(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
		void _on_logic_request(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
	private:
		lua_State* m_L;
	};
}

#endif
