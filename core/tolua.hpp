/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TO_LUA_H__
#define H_CORE_TO_LUA_H__

namespace core{
	/** primary template **/
	template<typename T>
	inline void ToLua(lua_State* L, T v){
		if(!L) return;
		if(v){
			v->toLua(L);
		}
		else{
			lua_checkstack(L, 1);
			lua_pushnil(L);
		}
	}

	/** bool **/
	template<>
	inline void ToLua(lua_State* L, bool v){
		if(!L) return;
		lua_checkstack(L, 1);
		lua_pushboolean(L, static_cast<int>(v));
	}

	/** number **/
#define _PUSH_LUA_NUMBER(t)	\
	template<>	\
	inline void ToLua(lua_State* L, t v){	\
		if(!L) return; \
		lua_checkstack(L, 1); \
		lua_pushnumber(L, static_cast< double >(v)); \
	}

	_PUSH_LUA_NUMBER(int8_t)
	_PUSH_LUA_NUMBER(int16_t)
	_PUSH_LUA_NUMBER(int32_t)
	_PUSH_LUA_NUMBER(int64_t)
	_PUSH_LUA_NUMBER(uint8_t)
	_PUSH_LUA_NUMBER(uint16_t)
	_PUSH_LUA_NUMBER(uint32_t)
	_PUSH_LUA_NUMBER(uint64_t)
	_PUSH_LUA_NUMBER(float32_t)
	_PUSH_LUA_NUMBER(float64_t)

#undef _PUSH_LUA_NUMBER

	/** string **/
	template<>
	inline void ToLua(lua_State* L, String* v){
		if(!L) return;
		lua_checkstack(L, 1);
		if(v){
			lua_pushstring(L, v->c_str());
		}
		else{
			lua_pushnil(L);
		}
	}

	/** bytes **/
	template<>
	inline void ToLua(lua_State* L, Bytes* v){
		if(!L) return;
		lua_checkstack(L, 1);
		if(v){
			lua_pushlstring(L, v->c_str(), (int)v->size());
		}
		else{
			lua_pushnil(L);
		}
	}
}

#endif
