/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_URL_ENCODE_H__
#define H_CORE_URL_ENCODE_H__

namespace core{
	/** UrlEncode **/
	class UrlEncode: public Object{
	public:
		static String* Encode(const char* src, const int64_t len);
		static String* Decode(const char* code, const int64_t len);
		static String* Encode(String* src);
		static String* Decode(String* code);
	public:
		static int _Encode(lua_State* L);
		static int _Decode(lua_State* L);
		static bool RegisterToLua(lua_State* L);
	};

}

#endif
