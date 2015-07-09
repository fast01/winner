/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LUA_COMPATIBLE_H__
#define H_CORE_LUA_COMPATIBLE_H__

#if LUA_SCRIPT_VERSION >= 520
	inline size_t lua_objlen(lua_State *L, int idx){
		return lua_rawlen(L, idx);
	}
#endif

// EOF
#endif
