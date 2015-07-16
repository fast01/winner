/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** UrlEncode **/
	String* UrlEncode::Encode(const char* src, const int64_t len){
		if(!src){
			return 0;
		}
		// prepare
		BinaryCoder<1024> coder;
		int64_t real_len =len;
		if(len < 0){
			real_len =strlen(src);
		}
		// encode
		for(int64_t i=0; i<real_len; ++i){
			const char ch =src[i];
			if(isalnum(ch) || ch=='.' || ch=='-' || ch=='_' || ch=='~'){
				coder.append(&ch, 1);
			}
			else{
				char sz[3];
				sprintf(sz, "%%%02X", (unsigned char)ch);
				coder.append(sz, sizeof(sz));
			}
		}
		// return
		if(coder.size() == 0){
			return String::New();
		}
		else{
			return String::New(coder.c_str(), coder.size());
		}
	}
	String* UrlEncode::Decode(const char* code, const int64_t len){
		if(!code){
			return 0;
		}
		// prepare
		BinaryCoder<1024> coder;
		int64_t real_len =len;
		if(len < 0){
			real_len =strlen(code);
		}
		// encode
		for(int64_t i=0; i<real_len; ++i){
			int ch =code[i];
			if(isalnum(ch) || ch=='.' || ch=='-' || ch=='_' || ch=='~'){
				const uint8_t x =static_cast<uint8_t>(ch);
				coder.append(&x, 1);
			}
			else if(ch == '%'){
				if(1 != sscanf(code+i, "%%%2X", &ch)){
					ERROR("invalid url code");
					return 0;
				}
				const uint8_t x =static_cast<uint8_t>(ch);
				coder.append(&x, sizeof(x));
				i+=2;
			}
			else{
					puts(code);
				ERROR("invalid url code");
				return 0;
			}
		}
		// return
		if(coder.size() == 0){
			return String::New();
		}
		else{
			return String::New(coder.c_str(), coder.size());
		}
	}
	String* UrlEncode::Encode(String* src){
		if(!src){
			return 0;
		}
		return Encode(src->c_str(), src->size());
	}
	String* UrlEncode::Decode(String* code){
		if(!code){
			return 0;
		}
		return Decode(code->c_str(), code->size());
	}

	/** for lua **/
	int UrlEncode::_Encode(lua_State* L){
		// check arg
		if((lua_gettop(L)<1) || (lua_isstring(L, 1)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t s;
		const char* data =lua_tolstring(L, 1, &s);
		ASSERT(data);
		// encode
		if(String* code =Encode(data, s)){
			lua_pushstring(L, code->c_str());
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "UrlEncode::Encode error");
			return 2;
		}
	}
	int UrlEncode::_Decode(lua_State* L){
		// check arg
		if((lua_gettop(L)<1) || (lua_isstring(L, 1)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t s;
		const char* data =lua_tolstring(L, 1, &s);
		ASSERT(data);
		// decode
		if(String* src =Decode(data, s)){
			lua_pushstring(L, src->c_str());
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "UrlEncode::Decode error");
			return 2;
		}
	}
	bool UrlEncode::RegisterToLua(lua_State* L){
		CLASS_FUNC func[2] ={
			{ "Encode", &_Encode },
			{ "Decode", &_Decode },
		};
		return register_class_to_lua(L, "Core", "UrlEncode", 0, 0,
			2, func,
			0, 0,
			0, 0
		);
	}
}
