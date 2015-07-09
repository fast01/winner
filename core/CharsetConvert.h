/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CHARSET_CONVERT_H__
#define H_CORE_CHARSET_CONVERT_H__

namespace core{
	/*** CharsetConvert ***/
	class CharsetConvert{
	public:
		static Bytes* Convert(const char* src, const char* from_code, const char* to_code);
	private:
		static int _Convert(lua_State* L);
	public:
		static bool RegisterToLua(lua_State* L);
	};
}

#endif
