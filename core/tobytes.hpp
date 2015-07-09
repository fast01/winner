/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TO_BYTES_H__
#define H_CORE_TO_BYTES_H__

namespace core{
	/** primary template **/
	template<typename T>
	inline void ToBytes(Bytes* bytes, T v){
		if(v){
			v->toBytes(bytes);
		}
		else{
			// null object
			packet_size_t s =0;
			bytes->write(&s, sizeof(s));
		}
	}

	/** bool **/
	template<>
	inline void ToBytes(Bytes* bytes, bool v){
		bytes->encodeBoolean(v);
	}

	/** int **/
#define _WRITE_BYTES_INT(t)	\
	template<>	\
	inline void ToBytes(Bytes* bytes, t v){	\
		bytes->encodeInt64((int64_t)v); \
	}

	_WRITE_BYTES_INT(int8_t)
	_WRITE_BYTES_INT(int16_t)
	_WRITE_BYTES_INT(int32_t)
	_WRITE_BYTES_INT(int64_t)

#undef _WRITE_BYTES_INT

	/** uint **/
#define _WRITE_BYTES_UINT(t)	\
	template<>	\
	inline void ToBytes(Bytes* bytes, t v){	\
		bytes->encodeUint64((uint64_t)v); \
	}

	_WRITE_BYTES_UINT(uint8_t)
	_WRITE_BYTES_UINT(uint16_t)
	_WRITE_BYTES_UINT(uint32_t)
	_WRITE_BYTES_UINT(uint64_t)

#undef _WRITE_BYTES_UINT
	
	/** float **/
	template<>
	inline void ToBytes(Bytes* bytes, float32_t v){
		bytes->encodeFloat32(v);
	}
	template<>
	inline void ToBytes(Bytes* bytes, float64_t v){
		bytes->encodeFloat64(v);
	}

	/** string **/
	template<>
	inline void ToBytes(Bytes* bytes, String* v){
		bytes->encodeString(v);
	}
}

#endif
