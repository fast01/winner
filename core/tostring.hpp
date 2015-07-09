/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TO_STRING_H__
#define H_CORE_TO_STRING_H__

namespace core{
	/** primary **/
	template<typename T>
	String* ToString(T v){
		static_assert(std::is_base_of<Object, typename std::remove_pointer<T>::type >::value, "ToString arg error, not a Object");
		return v ? v->toString() : StringTable::Get("Object(null)");
	}

	//** void* **//
	template<>
	inline String* ToString< void* >(void* v){
		return String::Format("0X%X", v);
	}
	template<>
	inline String* ToString< const void* >(const void* v){
		return String::Format("0X%X", v);
	}

	//** bool **//
	template<>
	inline String* ToString< bool >(bool v){
		return StringTable::Get(v ? "true" : "false");
	}

	//** int **//
#define _DEFINE_SIGNED_INTEGER_TO_STRING(type)	\
	template<>	\
	inline String* ToString< type >(type v){	\
		return String::Format("%lld", static_cast<long long>(v));	\
	}

	_DEFINE_SIGNED_INTEGER_TO_STRING(int8_t)
	_DEFINE_SIGNED_INTEGER_TO_STRING(int16_t)
	_DEFINE_SIGNED_INTEGER_TO_STRING(int32_t)
	_DEFINE_SIGNED_INTEGER_TO_STRING(int64_t)
	#undef _DEFINE_SIGNED_INTEGER_TO_STRING

	//** uint **//
	#define _DEFINE_UNSIGNED_INTEGER_TO_STRING(type)	\
	template<>	\
	inline String* ToString< type >(type v){	\
		return String::Format("%llu", static_cast<long long>(v));	\
	}

	_DEFINE_UNSIGNED_INTEGER_TO_STRING(uint8_t)
	_DEFINE_UNSIGNED_INTEGER_TO_STRING(uint16_t)
	_DEFINE_UNSIGNED_INTEGER_TO_STRING(uint32_t)
	_DEFINE_UNSIGNED_INTEGER_TO_STRING(uint64_t)
	#undef _DEFINE_UNSIGNED_INTEGER_TO_STRING

	//** float **//
	#define _DEFINE_FLOAT_TO_STRING(type)	\
	template<>	\
	inline String* ToString< type >(type v){	\
		return String::Format("%f", v);	\
	}

	_DEFINE_FLOAT_TO_STRING(float32_t)
	_DEFINE_FLOAT_TO_STRING(float64_t)
	#undef _DEFINE_FLOAT_TO_STRING

	//** string **//
	template<>
	inline String* ToString< String* >(String* v){
		if(!v) return STR("Object(null)");
		return v;
	}
}
#endif
