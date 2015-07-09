/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Sha ***/
	bool Sha::Sum(const int64_t type, const void* bytes, const int64_t len, char* pbuffer){
		if(!(type>=TYPE_MIN && type<=TYPE_MAX) || !bytes || len<=0 || !pbuffer){
			return false;
		}
		bool ok =false;
		unsigned char digest[SHA512_DIGEST_LENGTH] ={0};
		int digest_len =0;
		if(type == TYPE_SHA1){
			digest_len =SHA_DIGEST_LENGTH;
			ok =SHA1(reinterpret_cast< const unsigned char* >(bytes), len, digest);
		}
		else if(type == TYPE_SHA224){
			digest_len =SHA224_DIGEST_LENGTH;
			ok =SHA224(reinterpret_cast< const unsigned char* >(bytes), len, digest);
		}
		else if(type == TYPE_SHA256){
			digest_len =SHA256_DIGEST_LENGTH;
			ok =SHA256(reinterpret_cast< const unsigned char* >(bytes), len, digest);
		}
		else if(type == TYPE_SHA384){
			digest_len =SHA384_DIGEST_LENGTH;
			ok =SHA384(reinterpret_cast< const unsigned char* >(bytes), len, digest);
		}
		else if(type == TYPE_SHA512){
			digest_len =SHA512_DIGEST_LENGTH;
			ok =SHA512(reinterpret_cast< const unsigned char* >(bytes), len, digest);
		}
		if(ok){
			for(int i=0; i<digest_len; ++i){
				sprintf(pbuffer+i*2, "%02x", digest[i]);
			}
			return true;
		}
		else{
			return false;
		}
	}
	int Sha::_Sum(lua_State* L){
		// check arg
		if((lua_gettop(L)<2) || (lua_isnumber(L, 1)==0) || (lua_isstring(L, 2)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		const int64_t type =static_cast<int64_t>(lua_tonumber(L, 1));
		if(!(type>=TYPE_MIN && type<=TYPE_MAX)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg, type is out of range");
			return 2;
		}
		size_t s;
		const char* data =lua_tolstring(L, 2, &s);
		ASSERT(data);

		// sum
		char result[SHA512_BUFFER_LENGTH + 1] ={0};
		if(Sum(type, data, s, result)){
			lua_pushstring(L, result);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "Sha::Sum error");
			return 2;
		}
	}
	bool Sha::RegisterToLua(lua_State* L){
		CLASS_FUNC func[1] ={
			{ "Sum", &_Sum},
		};
		CLASS_ENUM enumerator[5] ={
			{ "SHA1", TYPE_SHA1},
			{ "SHA224", TYPE_SHA224},
			{ "SHA256", TYPE_SHA256},
			{ "SHA384", TYPE_SHA384},
			{ "SHA512", TYPE_SHA512},
		};
		return register_class_to_lua(L, "Core", "Sha", 0, 0,
			1, func,
			0, 0,
			5, enumerator
		);
	}
}
