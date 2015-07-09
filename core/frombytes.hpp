/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FROM_BYTES_H__
#define H_CORE_FROM_BYTES_H__

namespace core{
	/** primary template **/
	template<typename T>
	inline bool FromBytes(Bytes* bytes, T& v){
		if(!bytes || bytes->getRemainCount()<sizeof(packet_size_t)) return false;
		if(0 == *reinterpret_cast< packet_size_t* >(bytes->c_str()+bytes->getReadCursor())){
			bytes->skip(sizeof(packet_size_t));
			v =0;
			return true;
		}
		else{
			v =SafeNew< typename std::remove_pointer<T>::type >();
			return v->fromBytes(bytes);
		}
	}

	/** bool **/
	template<>
	inline bool FromBytes(Bytes* bytes, bool& v){
		return bytes && bytes->decodeBoolean(v);
	}

	/** int **/
#define _READ_BYTES_INT(t)	\
	template<>	\
	inline bool FromBytes(Bytes* bytes, t& v){	\
		if(!bytes) return false; \
		int64_t val =0;	\
		const bool ok =bytes->decodeInt64(val); \
		v =static_cast<t>(val);	\
		return ok;	\
	}

	_READ_BYTES_INT(int8_t)
	_READ_BYTES_INT(int16_t)
	_READ_BYTES_INT(int32_t)
	_READ_BYTES_INT(int64_t)

#undef _READ_BYTES_INT

	/** uint **/
#define _READ_BYTES_UINT(t)	\
	template<>	\
	inline bool FromBytes(Bytes* bytes, t& v){	\
		if(!bytes) return false; \
		uint64_t val =0;	\
		const bool ok =bytes->decodeUint64(val); \
		v =static_cast<t>(val);	\
		return ok;	\
	}

	_READ_BYTES_UINT(uint8_t)
	_READ_BYTES_UINT(uint16_t)
	_READ_BYTES_UINT(uint32_t)
	_READ_BYTES_UINT(uint64_t)

#undef _READ_BYTES_UINT
	
	/** float **/
	template<>
	inline bool FromBytes(Bytes* bytes, float32_t& v){
		return bytes && bytes->decodeFloat32(v);
	}
	template<>
	inline bool FromBytes(Bytes* bytes, float64_t& v){
		return bytes && bytes->decodeFloat64(v);
	}

	/** string **/
	template<>
	inline bool FromBytes(Bytes* bytes, String*& v){
		return bytes && bytes->decodeString(v);
	}
}

#endif
