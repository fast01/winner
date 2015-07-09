/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_PROTOCOL_BASE_H__
#define H_PROTOCOL_BASE_H__

namespace core{
/*
	define ProtocolBase
*/
class ProtocolBase: public Object{
	DECLARE_SUPPORT_SCRIPT
public:
	virtual int64_t group() =0;
	virtual int64_t id() =0;
	virtual const char* name() =0;
	virtual void clean() =0;
	virtual bool check() =0;
	virtual bool toLua(lua_State* L) =0;
	virtual bool fromLua(lua_State* L, const int64_t idx) =0;
	virtual bool appendToString(Bytes* str, int64_t tab_count) =0;
public:
	static bool RegisterToLua(lua_State* L);
private:
	static int _Reply(lua_State* L);
	static int _ReplyByObject(lua_State* L);
};
}
#endif
