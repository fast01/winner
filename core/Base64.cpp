/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** prepare ***/
	static const char base64_alpha[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static uint8_t base64_alpha_idx[255] ={0};

	/*** Base64 ***/
	void Base64::Init(){
		memset(base64_alpha_idx, 0xFF, sizeof(base64_alpha_idx));
		for(uint8_t i=0; i<64; ++i){
			base64_alpha_idx[static_cast<uint8_t>(base64_alpha[i])] =i;
		}
	}
	Bytes* Base64::Encode(const uint8_t* str, const int64_t len){
		// check arg
		if(str == 0 || len<0){
			return 0;
		}
		// pre
		Bytes* bs =SafeNew<Bytes>();
		bs->resize((len / 3 + 1) * 4);
		const int64_t n =len - len%3;
		uint8_t buf[4] ={0};
		uint32_t bits =0;
		// calc
		for(int64_t i=0; i<n; i+=3){
			uint8_t* data =(uint8_t*)str + i;
			bits = data[0] << 16u;
			bits |= (data[1] << 8u);
			bits |= data[2];
			buf[0] =base64_alpha[ (bits >> 18) & 0x3F ];
			buf[1] =base64_alpha[ (bits >> 12) & 0x3F ];
			buf[2] =base64_alpha[ (bits >> 6) & 0x3F ];
			buf[3] =base64_alpha[ bits & 0x3F ];
			bs->write(buf, 4);
		}
		const int64_t remain_n =len % 3;
		if(1 == remain_n){
			bits =str[len - 1] << 16u;
			buf[0] =base64_alpha[ (bits >> 18) & 0x3F ];
			buf[1] =base64_alpha[ (bits >> 12) & 0x3F ];
			buf[2] ='=';
			buf[3] ='=';
			bs->write(buf, 4);
		}
		else if(2 == remain_n){
			bits = (str[len - 2] << 16u);
			bits |= (str[len - 1] << 8u);
			buf[0] =base64_alpha[ (bits >> 18) & 0x3F ];
			buf[1] =base64_alpha[ (bits >> 12) & 0x3F ];
			buf[2] =base64_alpha[ (bits >> 6) & 0x3F ];
			buf[3] ='=';
			bs->write(buf, 4);
		}
		return bs;
	}
	Bytes* Base64::Decode(const uint8_t* str, const int64_t len){
		// check arg
		if(!str || len%4){
			ERROR("invalid base64 code");
			return 0;
		}
		// pre
		Bytes* bs =SafeNew<Bytes>();
		bs->resize((len / 4) * 3);
		uint8_t buf[3] ={0};
		uint32_t bits =0;
		int64_t ch_count =0;
		// calc
		for(int64_t i=0; i<len; ++i){
			uint8_t ch =str[i];
			const uint8_t idx =base64_alpha_idx[ch];
			if(ch == '='){
				break;
			}
			if(idx == 0xFF){
				ERROR("invalid base64 code");
				return 0;
			}
			bits |= idx & 0x3F;
			ch_count +=1;
			if(ch_count == 4){
				buf[0] =(bits >> 16) & 0xFF;
				buf[1] =(bits >> 8) & 0xFF;
				buf[2] =bits & 0xFF;
				bs->write(buf, 3);
				ch_count =0;
				bits =0;
			}
			else{
				bits <<= 6;
			}
		}
		if(ch_count == 2){
			bits <<= 6;
			buf[0] =(bits >> 16) & 0xFF;
			bs->write(buf, 1);
		}
		else if(ch_count == 3){
			buf[0] =(bits >> 16) & 0xFF;
			buf[1] =(bits >> 8) & 0xFF;
			bs->write(buf, 2);
		}
		else if(ch_count != 0){
			ERROR("invalid base64 code");
			return 0;
		}
		return bs;
	}
	int Base64::_encode_by_lua(lua_State* L){
		if(!lua_isstring(L, 1)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t s =0;
		const char* src =lua_tolstring(L, 1, &s);
		if(Bytes* bs=Encode(reinterpret_cast< const uint8_t* >(src), (int64_t)s)){
			lua_pushlstring(L, bs->c_str(), bs->getWriteCursor());
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "base64 encode error");
			return 2;
		}
	}
	int Base64::_decode_by_lua(lua_State* L){
		if(!lua_isstring(L, 1)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		size_t s =0;
		const char* src =lua_tolstring(L, 1, &s);
		if(Bytes* bs=Decode(reinterpret_cast< const uint8_t* >(src), s)){
			lua_pushlstring(L, bs->c_str(), bs->getWriteCursor());
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "base64 decode error");
			return 2;
		}
	}
	bool Base64::RegisterToLua(lua_State* L){
		CLASS_FUNC func[2] ={
			{ "Encode", &_encode_by_lua },
			{ "Decode", &_decode_by_lua },
		};
		return register_class_to_lua(L, "Core", "Base64", 0, 0,
			2, func,
			0, 0,
			0, 0
		);
	}
}
