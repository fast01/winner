/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_REQUESTOR_H__
#define H_CORE_REQUESTOR_H__

namespace core{
	/** Requestor **/
	class Requestor : public Object{
		DECLARE_SUPPORT_SCRIPT
	public:
		virtual int64_t getId() =0;
		virtual bool reply(PACKET& packet, void* body, const int64_t body_len) =0;
		virtual bool replyByObject(PACKET& packet, Object* obj);
		virtual bool send(const char* data, const int64_t data_len);
		virtual bool sendv(const MEMORY_SLICE* slice, const int64_t n) =0;
	public:
		static bool RegisterToLua(lua_State* L);
	private:
		static int _Send(lua_State* L);
		static int _Reply(lua_State* L);
		static int _ReplyByObject(lua_State* L);
	};
}

#endif
