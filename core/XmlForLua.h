/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_XML_FOR_LUA_H__
#define H_CORE_XML_FOR_LUA_H__

namespace core{
	/*** XmlForLua ***/
	class XmlForLua{
	public:
		static int _Encode(lua_State* L);
		static int _Decode(lua_State* L);
	public:
		static bool RegisterToLua(lua_State* L);
	};
}

#endif
