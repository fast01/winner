/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** Md5 **/
	bool Md5::Sum(const void* bytes, const int64_t len, char* pbuffer){
		if(!bytes || len<=0 || !pbuffer){
			return false;
		}
		unsigned char digest[MD5_DIGEST_LENGTH] ={0};
		if(!MD5(reinterpret_cast< const unsigned char* >(bytes), len, digest)){
			return false;
		}
		for(int i=0; i<MD5_DIGEST_LENGTH; ++i){
			sprintf(pbuffer+i*2, "%02x", digest[i]);
		}
		return true;
	}
	int Md5::_Sum(lua_State* L){
		// check arg
		if((lua_gettop(L)<1) || (lua_isstring(L, 1)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t s;
		const char* data =lua_tolstring(L, -1, &s);
		ASSERT(data);
		// sum
		char result[BUFFER_LENGTH + 1] ={0};
		if(Sum(data, s, result)){
			lua_pushstring(L, result);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "Md5::Sum error");
			return 2;
		}
	}
	bool Md5::RegisterToLua(lua_State* L){
		CLASS_FUNC func[1] ={
			{ "Sum", &_Sum},
		};
		return register_class_to_lua(L, "Core", "Md5", 0, 0,
			1, func,
			0, 0,
			0, 0
		);
	}
}
