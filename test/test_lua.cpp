/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	static bool test_lua_internal(){
		// new state
		lua_State* L =luaL_newstate();
		if(!L){
			ERROR("fail to new lua state, lua language error");	
			return false;	
		}
		luaL_openlibs(L);
		lua_settop(L, 0);

		// set version
		lua_pushnumber(L, LUA_SCRIPT_VERSION);
		lua_setglobal(L, "LUA_SCRIPT_VERSION");

		// open core
		if(luaL_dofile(L, "../core/script/core.lua")){
			ERROR("fail to load core module, %s", lua_tostring(L, -1));	
			lua_close(L);
			return false;
		}
		lua_settop(L, 0);

		// call register
		if(!::core::ProcessLocal::Instance()->initLua(L)){
			ERROR("fail to register module");	
			lua_close(L);
			return false;
		}
		lua_settop(L, 0);

		// try test
		::core::get_lua_global_var(L, "Core.UnitTest.Test");
		if(lua_isfunction(L, -1)){
			if(lua_pcall(L, 0, 0, 0) != 0){
				ERROR("fail to call lua unit_test, %s", lua_tostring(L, -1));	
				lua_close(L);
				return false;
			}
		}

		// close lua
		lua_close(L);
		return true;
	}
	void test_lua(){
		OPH();
		DEBUG("testing lua ......");
		test_lua_internal();
	}
}

