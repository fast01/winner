/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FROM_LUA_H__
#define H_CORE_FROM_LUA_H__

namespace core{
	/** primary template **/
	template<typename T>
	inline bool FromLua(lua_State* L, int idx, T& v){
		if(!L) return false;
		if(lua_istable(L, idx)){
			v =SafeNew< typename std::remove_pointer<T>::type >();
			return v->fromLua(L, idx);
		}
		else if(lua_isnil(L, idx)){
			v =0;
			return true;
		}
		else{
			return false;
		}
	}

	/** bool **/
	template<>
	inline bool FromLua(lua_State* L, int idx, bool& v){
		if(!L || !lua_isboolean(L, idx)){
			return false;
		}
		v =(lua_toboolean(L, idx) != 0);
		return true;
	}

	/** number **/
#define _FETCH_LUA_NUMBER(t)	\
	template<>	\
	inline bool FromLua(lua_State* L, int idx, t& v){	\
		if(!L || !lua_isnumber(L, idx)){ \
			return false; \
		} \
		v =static_cast<t>(lua_tonumber(L, idx)); \
		return true; \
	}

	_FETCH_LUA_NUMBER(int8_t)
	_FETCH_LUA_NUMBER(int16_t)
	_FETCH_LUA_NUMBER(int32_t)
	_FETCH_LUA_NUMBER(int64_t)
	_FETCH_LUA_NUMBER(uint8_t)
	_FETCH_LUA_NUMBER(uint16_t)
	_FETCH_LUA_NUMBER(uint32_t)
	_FETCH_LUA_NUMBER(uint64_t)
	_FETCH_LUA_NUMBER(float32_t)
	_FETCH_LUA_NUMBER(float64_t)

#undef _FETCH_LUA_NUMBER

	/** string **/
	template<>
	inline bool FromLua(lua_State* L, int idx, String*& v){
		if(!L){
			return false;
		}
		if(lua_isstring(L, idx)){
			size_t len =0;
			const char* data =lua_tolstring(L, idx, &len);
			v =String::NewString(data, len);
			return true;
		}
		else if(lua_isnil(L, idx)){
			v =0;
			return true;
		}
		else{
			return false;
		}
	}

	/** bytes **/
	template<>
	inline bool FromLua(lua_State* L, int idx, Bytes*& v){
		if(!L){
			return false;
		}
		if(lua_isstring(L, idx)){
			size_t len =0;
			const char* data =lua_tolstring(L, idx, &len);
			Bytes* bs =SafeNew< Bytes >();
			bs->set(data, len);
			return true;
		}
		else if(lua_isnil(L, idx)){
			v =0;
			return true;
		}
		else{
			return false;
		}
	}
}
#endif
