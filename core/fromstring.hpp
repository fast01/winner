/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FROM_STRING_H__
#define H_CORE_FROM_STRING_H__

namespace core{
	/** primary **/
	template<typename T>
	bool FromString(String* str,T& v);

	//** bool **//
	template<>
	inline bool FromString(String* str, bool& v){
		if(!str){
			return false;
		}
		if(str->is("1") || str->is("true") || str->is("True") || str->is("TRUE")){
			v =true;
			return true;
		}
		else if(str->is("0") || str->is("false") || str->is("False") || str->is("FALSE")){
			v =false;
			return true;
		}
		else{
			return false;
		}
	}

	//** int **//
#define _DEFINE_SIGNED_INTEGER_FROM_STRING(type)	\
	template<>	\
	inline bool FromString< type >(String* str, type& v){	\
		if(!str || str->size()==0){ \
			return false; \
		} \
		long long val =0; \
		if(1 == sscanf(str->c_str(), "%lld", &val)){ \
			v =static_cast<type>(val); \
			return true; \
		} \
		else{ \
			return false; \
		} \
	}

	_DEFINE_SIGNED_INTEGER_FROM_STRING(int8_t)
	_DEFINE_SIGNED_INTEGER_FROM_STRING(int16_t)
	_DEFINE_SIGNED_INTEGER_FROM_STRING(int32_t)
	_DEFINE_SIGNED_INTEGER_FROM_STRING(int64_t)
	#undef _DEFINE_SIGNED_INTEGER_FROM_STRING

	//** uint **//
	#define _DEFINE_UNSIGNED_INTEGER_FROM_STRING(type)	\
	template<>	\
	inline bool FromString< type >(String* str, type& v){	\
		if(!str || str->size()==0){ \
			return false; \
		} \
		unsigned long long val =0; \
		if(1 == sscanf(str->c_str(), "%llu", &val)){ \
			v =static_cast<type>(val); \
			return true; \
		} \
		else{ \
			return false; \
		} \
	}

	_DEFINE_UNSIGNED_INTEGER_FROM_STRING(uint8_t)
	_DEFINE_UNSIGNED_INTEGER_FROM_STRING(uint16_t)
	_DEFINE_UNSIGNED_INTEGER_FROM_STRING(uint32_t)
	_DEFINE_UNSIGNED_INTEGER_FROM_STRING(uint64_t)
	#undef _DEFINE_UNSIGNED_INTEGER_FROM_STRING

	//** float **//
	#define _DEFINE_FLOAT_FROM_STRING(type)	\
	template<>	\
	inline bool FromString< type >(String* str, type& v){	\
		if(!str || str->size()==0){ \
			return false; \
		} \
		double val =0; \
		if(1 == sscanf(str->c_str(), "%lf", &val)){ \
			v =static_cast<type>(val); \
			return true; \
		} \
		else{ \
			return false; \
		} \
	}

	_DEFINE_FLOAT_FROM_STRING(float32_t)
	_DEFINE_FLOAT_FROM_STRING(float64_t)
	#undef _DEFINE_FLOAT_FROM_STRING

	//** string **//
	template<>	
	inline bool FromString< String* >(String* str, String* &v){
		if(!str){
			return false;
		}
		v =str;
		return true;
	}
}
#endif
