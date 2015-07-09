/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** CharsetConvert ***/
	Bytes* CharsetConvert::Convert(const char* src, const char* from_code, const char* to_code){
		// open
		iconv_t ic =iconv_open(to_code, from_code);
		if((iconv_t)-1 == ic){
			ERROR("fail to %s, %s", __FUNCTION__, get_last_error_desc());
			return 0;
		}

		// conv
		Bytes* bs =SafeNew<Bytes>();
		char buf[64] ={0};
		size_t os =64;
		char* ob =0;
		size_t is =strlen(src);
		char* ib =const_cast< char* >(src);

		while(is > 0){
			os =64;
			ob =buf;
			const int err =iconv(ic, &ib, &is, &ob, &os);
			if(err == -1){
				if(get_last_error() != E2BIG){
					ERROR("fail to %s, %s", __FUNCTION__, get_last_error_desc());
					iconv_close(ic);
					return 0;
				}
			}
			bs->write(buf, ob-buf);
		}

		// close
		iconv_close(ic);
		return bs;
	}
	int CharsetConvert::_Convert(lua_State* L){
		if((lua_gettop(L)<3) || (lua_isstring(L, 1)==0) || (lua_isstring(L, 2)==0) || (lua_isstring(L, 3)==0)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		const char* src =lua_tostring(L, 1);
		const char* from_code =lua_tostring(L, 2);
		const char* to_code =lua_tostring(L, 3);
		if(Bytes* bs =Convert(src, from_code, to_code)){
			lua_pushlstring(L, bs->c_str(), bs->getWriteCursor());
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "Convert error, invalid input sequence");
			return 2;
		}
	}
	bool CharsetConvert::RegisterToLua(lua_State* L){
		CLASS_FUNC func[1] ={
			{ "Convert", &_Convert},
		};
		return register_class_to_lua(L, "Core", "CharsetConvert", 0, 0,
			1, func,
			0, 0,
			0, 0
		);
	}
}
