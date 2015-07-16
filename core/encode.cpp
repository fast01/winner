/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** encode decode bool **/
	int64_t encode_boolean(char* data, const int64_t len, const bool v){
		if(len < (int64_t)sizeof(uint8_t)) return -1;
		*reinterpret_cast< uint8_t* >(data) =static_cast<uint8_t>(v);	
		return sizeof(uint8_t);
	}
	int64_t decode_boolean(const char* data, const int64_t len, bool& v){
		if(len < (int64_t)sizeof(uint8_t)) return -1;
		v =(*reinterpret_cast< const uint8_t* >(data) != 0);
		return sizeof(uint8_t);
	}

	/** encode decode float **/
	int64_t encode_float32(char* data, const int64_t len, const float32_t v){
		if(len < (int64_t)sizeof(v)) return -1;
		*reinterpret_cast< float32_t* >(data) =v;	
		return sizeof(v);
	}
	int64_t decode_float32(const char* data, const int64_t len, float32_t& v){
		if(len < (int64_t)sizeof(v)) return -1;
		v =*reinterpret_cast< const float32_t* >(data);
		return sizeof(v);
	}

	int64_t encode_float64(char* data, const int64_t len, const float64_t v){
		if(len < (int64_t)sizeof(v)) return -1;
		*reinterpret_cast< float64_t* >(data) =v;	
		return sizeof(v);
	}
	int64_t decode_float64(const char* data, const int64_t len, float64_t& v){
		if(len < (int64_t)sizeof(v)) return -1;
		v =*reinterpret_cast< const float64_t* >(data);
		return sizeof(v);
	}

	/** encode decode int **/
	int64_t encode_int64(char* data, const int64_t len, const int64_t num){
		uint8_t* v =reinterpret_cast< uint8_t* >(data);
		register uint64_t val =num<0 ? (-num) : num;
		register uint8_t sign = num<0 ? 0x40 : 0;
		if(val <= 0x3F){ // 6 bits
			if(len < 1) return -1;
			v[0] =static_cast<uint8_t>(val) | sign;
			return 1;
		}
		else if(val <= 0x1FFF){ // 13 bits
			if(len < 2) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>(val >> 6);
			return 2;
		}
		else if(val <= 0xFFFFF){ // 20 bits
			if(len < 3) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>(val >> 13);
			return 3;
		}
		else if(val <= 0x7FFFFFF){ // 27 bits
			if(len < 4) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>(val >> 20);
			return 4;
		}
		else if(val <= 0x3FFFFFFFF){ // 34 bits
			if(len < 5) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>(val >> 27);
			return 5;
		}
		else if(val <= 0x1FFFFFFFFFF){ // 41 bits
			if(len < 6) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 27) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>(val >> 34);
			return 6;
		}
		else if(val <= 0xFFFFFFFFFFFF){ // 48 bits
			if(len < 7) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 27) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 34) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>(val >> 41);
			return 7;
		}
		else if(val <= 0x7FFFFFFFFFFFFF){ // 55 bits
			if(len < 8) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 27) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 34) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 41) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>(val >> 48);
			return 8;
		}
		else if(val <= 0x3FFFFFFFFFFFFFFF){ // 62 bits
			if(len < 9) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 27) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 34) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 41) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>((val >> 48) & 0x7F) | 0x80;
			v[8] =static_cast<uint8_t>(val >> 55);
			return 9;
		}
		else{ // 64 bits
			if(len < 10) return -1;
			v[0] =static_cast<uint8_t>(val & 0x3F) | 0x80 | sign;
			v[1] =static_cast<uint8_t>((val >> 6) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 13) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 20) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 27) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 34) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 41) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>((val >> 48) & 0x7F) | 0x80;
			v[8] =static_cast<uint8_t>((val >> 55) & 0x7F) | 0x80;
			v[9] =static_cast<uint8_t>(val >> 62);
			return 10;
		}
	}
	int64_t decode_int64(const char* data, const int64_t len, int64_t& v){
		if(len <= 0){
			return -1;
		}
		register uint64_t offset =0;
		register const uint8_t* p =reinterpret_cast< const uint8_t* >(data + offset);
		register uint8_t sign =*p & 0x40;
		register uint64_t x =*p & 0x3F;
		register uint64_t cnt =1;
		while((*p & 0x80)){
			if(cnt >= (uint64_t)len || cnt>=10){
				return -1;
			}
			++p;
			x |= ((((uint64_t)(*p)) & 0x7Fllu) << (6llu + (cnt-1) * 7llu));
			++cnt;
		}
		v =(int64_t)x;
		v =sign ? -v : v;
		return (int64_t)cnt;
	}

	int64_t encode_uint64(char* data, const int64_t len, const uint64_t num){
		uint8_t* v =reinterpret_cast< uint8_t* >(data);
		register uint64_t val =num;
		if(val <= 0x7F){ // 7 bits
			if(len < 1) return -1;
			v[0] =static_cast<uint8_t>(val);
			return 1;
		}
		else if(val <= 0x3FFF){ // 14 bits
			if(len < 2) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>(val >> 7);
			return 2;
		}
		else if(val <= 0x1FFFFF){ // 21 bits
			if(len < 3) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>(val >> 14);
			return 3;
		}
		else if(val <= 0xFFFFFFF){ // 28 bits
			if(len < 4) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>(val >> 21);
			return 4;
		}
		else if(val <= 0x7FFFFFFFF){ // 35 bits
			if(len < 5) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>(val >> 28);
			return 5;
		}
		else if(val <= 0x3FFFFFFFFFF){ // 42 bits
			if(len < 6) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 28) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>(val >> 35);
			return 6;
		}
		else if(val <= 0x1FFFFFFFFFFFF){ // 49 bits
			if(len < 7) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 28) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 35) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>(val >> 42);
			return 7;
		}
		else if(val <= 0xFFFFFFFFFFFFFF){ // 56 bits
			if(len < 8) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 28) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 35) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 42) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>(val >> 49);
			return 8;
		}
		else if(val <= 0x7FFFFFFFFFFFFFFF){ // 63 bits
			if(len < 9) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 28) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 35) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 42) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>((val >> 49) & 0x7F) | 0x80;
			v[8] =static_cast<uint8_t>(val >> 56);
			return 9;
		}
		else{ // 64 bits
			if(len < 10) return -1;
			v[0] =static_cast<uint8_t>(val & 0x7F) | 0x80;
			v[1] =static_cast<uint8_t>((val >> 7) & 0x7F) | 0x80;
			v[2] =static_cast<uint8_t>((val >> 14) & 0x7F) | 0x80;
			v[3] =static_cast<uint8_t>((val >> 21) & 0x7F) | 0x80;
			v[4] =static_cast<uint8_t>((val >> 28) & 0x7F) | 0x80;
			v[5] =static_cast<uint8_t>((val >> 35) & 0x7F) | 0x80;
			v[6] =static_cast<uint8_t>((val >> 42) & 0x7F) | 0x80;
			v[7] =static_cast<uint8_t>((val >> 49) & 0x7F) | 0x80;
			v[8] =static_cast<uint8_t>((val >> 56) & 0x7F) | 0x80;
			v[9] =static_cast<uint8_t>(val >> 63);
			return 10;
		}
	}
	int64_t decode_uint64(const char* data, const int64_t len, uint64_t& v){
		if(len <= 0){
			return -1;
		}
		register uint64_t offset =0;
		register const uint8_t* p =reinterpret_cast< const uint8_t* >(data + offset);
		v =*p & 0x7F;
		register uint64_t cnt =1;
		while((*p & 0x80)){
			if(cnt >= (uint64_t)len || cnt>=10){
				return -1;
			}
			++p;
			v |= ((((uint64_t)(*p)) & 0x7Fllu) << (cnt * 7llu));
			++cnt;
		}
		return (int64_t)cnt;
	}

	/** encode decode string **/
	int64_t encode_string(char* data, const int64_t len, String* str){
		if(!str){
			// null object
			return encode_int64(data, len, -1);
		}
		else{
			const int64_t str_len =str->size();
			const int64_t offset =encode_int64(data, len, str_len);
			if((offset<0) || (len-offset < str_len)) return -1;
			if(str_len > 0){
				memcpy(data+offset, str->c_str(), str_len);
			}
			return offset + str_len;
		}
	}
	int64_t decode_string(const char* data, const int64_t len, String*& str){
		int64_t cnt =0;
		const int64_t offset =decode_int64(data, len, cnt);
		if(offset < 0){
			return -1;
		}
		if(cnt < 0){ // null object
			str =0;
			return offset;
		}
		else if(0 == cnt){
			str =String::NewString();
			return offset;
		}

		if(len-offset < cnt){
			return -1;
		}
		str =String::NewString(data+offset, cnt);
		return offset + cnt;
	}

	/** encode, decode packet **/
	int64_t encode_packet(char* data, const int64_t s, const PACKET& packet){
		int64_t len =s;
		if(len < (int64_t)sizeof(packet_size_t)) return -1;
		int64_t offset =sizeof(packet_size_t);
		len -=sizeof(packet_size_t);

		// encode from
		int64_t n =encode_uint64(data+offset, len, packet.from);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode to
		n =encode_uint64(data+offset, len, packet.to);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode who
		n =encode_uint64(data+offset, len, packet.who);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode sn
		n =encode_uint64(data+offset, len, packet.sn);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode sub sn
		n =encode_uint64(data+offset, len, packet.sub_sn);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode command
		n =encode_uint64(data+offset, len, packet.command);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode option
		n =encode_uint64(data+offset, len, packet.option);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// encode size
		*reinterpret_cast< packet_size_t* >(data) =host_to_net_packet_size(packet.size + offset);
		ASSERT(packet.size+offset == ntohl(*reinterpret_cast< packet_size_t* >(data)));
		return offset;
	}
	int64_t decode_packet(const char* data, const int64_t s, PACKET& packet){
		int64_t len =s;
		if(len < (int64_t)sizeof(packet_size_t)) return -1;
		// decode size
		packet.size =*reinterpret_cast< const packet_size_t* >(data);
		packet.size =net_to_host_packet_size(packet.size);
		if(s < (int64_t)(packet.size)) return -1;
		int64_t offset =sizeof(packet_size_t);
		len -=sizeof(packet_size_t);

		// decode from
		int32_t n =decode_uint64(data+offset, len, packet.from);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode to
		n =decode_uint64(data+offset, len, packet.to);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode who
		n =decode_uint64(data+offset, len, packet.who);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode sn
		n =decode_uint64(data+offset, len, packet.sn);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode sub sn
		n =decode_uint64(data+offset, len, packet.sub_sn);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode command
		n =decode_uint64(data+offset, len, packet.command);
		if(n < 0) return -1;
		offset +=n;
		len -=n;

		// decode option
		n =decode_uint64(data+offset, len, packet.option);
		if(n < 0) return -1;
		offset +=n;
		len -=n;
	
		return offset;
	}
}
