/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FIELD_INFO_BASE_H__
#define H_CORE_FIELD_INFO_BASE_H__


/*
 *
 * support field type: boolean_t, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float32_t, float64_t, String*, Object* and its derive class.
 * field type constraints: 
 *   1. can't qualified by const or volatile.
 */

namespace core{
	/** FieldInfoBase **/
	class FieldInfoBase: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		union FIELD_VALUE{
			bool boolean_value;
			String* string_value;
			Object* object_value;

			int8_t int8_value;
			int16_t int16_value;
			int32_t int32_value;
			int64_t int64_value;

			uint8_t uint8_value;
			uint16_t uint16_value;
			uint32_t uint32_value;
			uint64_t uint64_value;

			float32_t float32_value;
			float64_t float64_value;
		};
	public:
		enum{
			FIELD_TYPE_UNKNOWN,

			FIELD_TYPE_BOOLEAN,
			FIELD_TYPE_STRING,
			FIELD_TYPE_OBJECT,

			FIELD_TYPE_INT8,
			FIELD_TYPE_INT16,
			FIELD_TYPE_INT32,
			FIELD_TYPE_INT64,

			FIELD_TYPE_UINT8,
			FIELD_TYPE_UINT16,
			FIELD_TYPE_UINT32,
			FIELD_TYPE_UINT64,

			FIELD_TYPE_FLOAT32,
			FIELD_TYPE_FLOAT64,
		};
	protected:
		FieldInfoBase(String* name, const int64_t field_type, String* attribute);
		virtual ~FieldInfoBase();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual void load(Object* obj) =0;
		virtual void store(Object* obj) =0;
	public:
		String* getName();
		int64_t getFieldType();
		String* getAttribute(String* attr_name);
	public:
		bool isBoolean();
		bool isString();
		bool isObject();

		bool isInt8();
		bool isInt16();
		bool isInt32();
		bool isInt64();

		bool isUint8();
		bool isUint16();
		bool isUint32();
		bool isUint64();

		bool isFloat32();
		bool isFloat64();
	public:
		bool asBoolean(Object* obj);
		String* asString(Object* obj);
		Object* asObject(Object* obj);

		int8_t asInt8(Object* obj);
		int16_t asInt16(Object* obj);
		int32_t asInt32(Object* obj);
		int64_t asInt64(Object* obj);

		uint8_t asUint8(Object* obj);
		uint16_t asUint16(Object* obj);
		uint32_t asUint32(Object* obj);
		uint64_t asUint64(Object* obj);

		float32_t asFloat32(Object* obj);
		float64_t asFloat64(Object* obj);
	public:
		void setBoolean(Object* obj, const bool v);
		void setString(Object* obj, String* v);
		void setObject(Object* obj, Object* v);

		void setInt8(Object* obj, const int8_t v);
		void setInt16(Object* obj, const int16_t v);
		void setInt32(Object* obj, const int32_t v);
		void setInt64(Object* obj, const int64_t v);

		void setUint8(Object* obj, const uint8_t v);
		void setUint16(Object* obj, const uint16_t v);
		void setUint32(Object* obj, const uint32_t v);
		void setUint64(Object* obj, const uint64_t v);

		void setFloat32(Object* obj, const float32_t v);
		void setFloat64(Object* obj, const float64_t v);
	protected:
		String* const m_name;
		const int64_t m_field_type;
		Hash* m_attr_tb;
		FIELD_VALUE m_field_value;
	};
	/** field info type **/
	template< typename T >
	struct field_info_type{
		enum{
			value =std::is_base_of< Object, typename std::remove_cv< typename std::remove_pointer< typename std::remove_reference< T >::type >::type >::type >::value ? FieldInfoBase::FIELD_TYPE_OBJECT : FieldInfoBase::FIELD_TYPE_UNKNOWN
		};
	};
	template<>
	struct field_info_type<boolean_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_BOOLEAN
		};
	};
	template<>
	struct field_info_type< String* >{
		enum{
			value =FieldInfoBase::FIELD_TYPE_STRING
		};
	};
	template<>
	struct field_info_type<int8_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_INT8
		};
	};
	template<>
	struct field_info_type<int16_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_INT16
		};
	};
	template<>
	struct field_info_type<int32_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_INT32
		};
	};
	template<>
	struct field_info_type<int64_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_INT64
		};
	};
	template<>
	struct field_info_type<uint8_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_UINT8
		};
	};
	template<>
	struct field_info_type<uint16_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_UINT16
		};
	};
	template<>
	struct field_info_type<uint32_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_UINT32
		};
	};
	template<>
	struct field_info_type<uint64_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_UINT64
		};
	};
	template<>
	struct field_info_type<float32_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_FLOAT32
		};
	};
	template<>
	struct field_info_type<float64_t>{
		enum{
			value =FieldInfoBase::FIELD_TYPE_FLOAT64
		};
	};
	/** load field value **/
	template<typename T>
	void load_field_value(T& val, FieldInfoBase::FIELD_VALUE& field_value){
		ASSIGN_POINTER(field_value.object_value, val);
	}
	template<>
	inline void load_field_value< String* >(String*& val, FieldInfoBase::FIELD_VALUE& field_value){
		ASSIGN_POINTER(field_value.string_value, val);
	}
#define _LOAD_FIELD(t) \
	template<> \
	inline void load_field_value<t##_t>(t##_t& val, FieldInfoBase::FIELD_VALUE& field_value){ \
		field_value.t ## _value =val; \
	}
	_LOAD_FIELD(boolean)
	_LOAD_FIELD(int8)
	_LOAD_FIELD(int16)
	_LOAD_FIELD(int32)
	_LOAD_FIELD(int64)
	_LOAD_FIELD(uint8)
	_LOAD_FIELD(uint16)
	_LOAD_FIELD(uint32)
	_LOAD_FIELD(uint64)
	_LOAD_FIELD(float32)
	_LOAD_FIELD(float64)
#undef _LOAD_FIELD

	/** store field value **/
	template<typename T>
	void store_field_value(T& val, FieldInfoBase::FIELD_VALUE& field_value){
		ASSIGN_POINTER(val, field_value.object_value);
	}
	template<>
	inline void store_field_value< String* >(String*& val, FieldInfoBase::FIELD_VALUE& field_value){
		ASSIGN_POINTER(val, field_value.string_value);
	}
#define _STORE_FIELD(t) \
	template<> \
	inline void store_field_value<t##_t>(t##_t& val, FieldInfoBase::FIELD_VALUE& field_value){ \
		val =field_value.t ## _value; \
	}
	_STORE_FIELD(boolean)
	_STORE_FIELD(int8)
	_STORE_FIELD(int16)
	_STORE_FIELD(int32)
	_STORE_FIELD(int64)
	_STORE_FIELD(uint8)
	_STORE_FIELD(uint16)
	_STORE_FIELD(uint32)
	_STORE_FIELD(uint64)
	_STORE_FIELD(float32)
	_STORE_FIELD(float64)
#undef _STORE_FIELD
}

#endif
