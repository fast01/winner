/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BASE64_FOR_LUA_H__
#define H_CORE_BASE64_FOR_LUA_H__

namespace core{
	/*** Base64 ***/
	class Base64{
	public:
		static Bytes* Encode(const uint8_t* str, const int64_t len);
		static Bytes* Decode(const uint8_t* str, const int64_t len);
	private:
		static int _encode_by_lua(lua_State* L);
		static int _decode_by_lua(lua_State* L);
	public:
		static void Init();
		static bool RegisterToLua(lua_State* L);
	};
}

#endif
