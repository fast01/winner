/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ENCODE_H__
#define H_CORE_ENCODE_H__

namespace core{
	/** encode decode bool **/
	int64_t encode_boolean(char* data, const int64_t len, const bool v);
	int64_t decode_boolean(const char* data, const int64_t len, bool& v);

	/** encode decode float **/
	int64_t encode_float32(char* data, const int64_t len, const float32_t v);
	int64_t decode_float32(const char* data, const int64_t len, float32_t& v);

	int64_t encode_float64(char* data, const int64_t len, const float64_t v);
	int64_t decode_float64(const char* data, const int64_t len, float64_t& v);

	/** encode decode int **/
	int64_t encode_int64(char* data, const int64_t len, const int64_t v);
	int64_t decode_int64(const char* data, const int64_t len, int64_t& v);

	int64_t encode_uint64(char* data, const int64_t len, const uint64_t v);
	int64_t decode_uint64(const char* data, const int64_t len, uint64_t& v);

	/** encode decode string **/
	int64_t encode_string(char* data, const int64_t len, String* str);
	int64_t decode_string(const char* data, const int64_t len, String*& str);

	/** encode, decode packet **/
	int64_t encode_packet(char* data, const int64_t s, const PACKET& packet);
	int64_t decode_packet(const char* data, const int64_t len, PACKET& packet);
}

#endif
