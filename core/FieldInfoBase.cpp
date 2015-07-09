/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl FieldInfoBase ***/
	/** ctor & dtor **/
	FieldInfoBase::FieldInfoBase(String* name, const int64_t field_type, String* attribute)
		: m_name(name)
		, m_field_type(field_type)
		, m_attr_tb(0){
		ASSERT(m_name && m_field_type!=FIELD_TYPE_UNKNOWN);
		RETAIN_POINTER(m_name);

		// parse attribute
		if(attribute){
			Array* attr_ls =attribute->split(",");
			const int64_t n =attr_ls ? attr_ls->size() : 0;
			if(n > 0){
				ASSIGN_POINTER(m_attr_tb, SafeNew<Hash>());
			}
			for(int64_t i=0; i<n; i++){
				String* attr =static_cast< String* >(attr_ls->get(i));
				// const int64_t attr_len =attr->size();
				const int64_t pos =attr->indexOf("=");

				String* key =0;
				String* val =0;
				if(pos==-1){
					key =attr;
					val =STR("true");
				}
				else{
					key =attr->subString(0, pos);
					val =attr->subString(pos+1, -1);
				}

				key =key->trim()->replace("&comma;", ",", true)->replace("&amp;", "&", true)->replace("&eq;", "=", true);
				val =val->trim()->replace("&comma;", ",", true)->replace("&amp;", "&", true)->replace("&eq;", "=", true);
				if(key->size()){
					if(val->size() > 0){
						m_attr_tb->set(key, val);
					}
					else{
						m_attr_tb->set(key, STR("true"));
					}
					DEBUG("%s => %s", key->c_str(), static_cast< String* >(m_attr_tb->get(key))->c_str());
				}
				else{
					FATAL("`ClassInfo` \"%s\" attribute \"%s\" format invalid", m_name->c_str(), attribute->c_str());
				}
			}
		}
	}
	FieldInfoBase::~FieldInfoBase(){
	}

	/** Object **/
	void FieldInfoBase::init(){
		Super::init();
	}
	void FieldInfoBase::finalize(){
		RELEASE_POINTER(m_name);
		RELEASE_POINTER(m_attr_tb);
		Super::finalize();
	}
	/** SELF **/
	String* FieldInfoBase::getName(){
		return m_name;
	}
	int64_t FieldInfoBase::getFieldType(){
		return m_field_type;
	}
	String* FieldInfoBase::getAttribute(String* attr_name){
		return m_attr_tb ? static_cast< String* >(m_attr_tb->get(attr_name)) : 0;
	}
	/** is **/
	bool FieldInfoBase::isBoolean(){
		return m_field_type == FIELD_TYPE_BOOLEAN;
	}
	bool FieldInfoBase::isString(){
		return m_field_type == FIELD_TYPE_STRING;
	}
	bool FieldInfoBase::isObject(){
		return m_field_type == FIELD_TYPE_OBJECT;
	}

	bool FieldInfoBase::isInt8(){
		return m_field_type == FIELD_TYPE_INT8;
	}
	bool FieldInfoBase::isInt16(){
		return m_field_type == FIELD_TYPE_INT16;
	}
	bool FieldInfoBase::isInt32(){
		return m_field_type == FIELD_TYPE_INT32;
	}
	bool FieldInfoBase::isInt64(){
		return m_field_type == FIELD_TYPE_INT64;
	}

	bool FieldInfoBase::isUint8(){
		return m_field_type == FIELD_TYPE_UINT8;
	}
	bool FieldInfoBase::isUint16(){
		return m_field_type == FIELD_TYPE_UINT16;
	}
	bool FieldInfoBase::isUint32(){
		return m_field_type == FIELD_TYPE_UINT32;
	}
	bool FieldInfoBase::isUint64(){
		return m_field_type == FIELD_TYPE_UINT64;
	}

	bool FieldInfoBase::isFloat32(){
		return m_field_type == FIELD_TYPE_FLOAT32;
	}
	bool FieldInfoBase::isFloat64(){
		return m_field_type == FIELD_TYPE_FLOAT64;
	}

	/** as **/
	bool FieldInfoBase::asBoolean(Object* obj){
		MUST(isBoolean(), "field type is not a boolen");
		load(obj);
		return m_field_value.boolean_value;
	}
	String* FieldInfoBase::asString(Object* obj){
		MUST(isString(), "field type is not a string");
		load(obj);
		return m_field_value.string_value;
	}
	Object* FieldInfoBase::asObject(Object* obj){
		MUST(isObject(), "field type is not a object");
		load(obj);
		return m_field_value.object_value;
	}

	int8_t FieldInfoBase::asInt8(Object* obj){
		MUST(isInt8(), "field type is not a int8");
		load(obj);
		return m_field_value.int8_value;
	}
	int16_t FieldInfoBase::asInt16(Object* obj){
		MUST(isInt16(), "field type is not a int16");
		load(obj);
		return m_field_value.int16_value;
	}
	int32_t FieldInfoBase::asInt32(Object* obj){
		MUST(isInt32(), "field type is not a int32");
		load(obj);
		return m_field_value.int32_value;
	}
	int64_t FieldInfoBase::asInt64(Object* obj){
		MUST(isInt64(), "field type is not a int64");
		load(obj);
		return m_field_value.int64_value;
	}

	uint8_t FieldInfoBase::asUint8(Object* obj){
		MUST(isUint8(), "field type is not a uint8");
		load(obj);
		return m_field_value.uint8_value;
	}
	uint16_t FieldInfoBase::asUint16(Object* obj){
		MUST(isUint16(), "field type is not a uint16");
		load(obj);
		return m_field_value.uint16_value;
	}
	uint32_t FieldInfoBase::asUint32(Object* obj){
		MUST(isUint32(), "field type is not a uint32");
		load(obj);
		return m_field_value.uint32_value;
	}
	uint64_t FieldInfoBase::asUint64(Object* obj){
		MUST(isUint64(), "field type is not a uint64");
		load(obj);
		return m_field_value.uint64_value;
	}

	float32_t FieldInfoBase::asFloat32(Object* obj){
		MUST(isFloat32(), "field type is not a float32");
		load(obj);
		return m_field_value.float32_value;
	}
	float64_t FieldInfoBase::asFloat64(Object* obj){
		MUST(isFloat64(), "field type is not a float64");
		load(obj);
		return m_field_value.float64_value;
	}

	/** set **/
	void FieldInfoBase::setBoolean(Object* obj, const bool v){
		MUST(isBoolean(), "field type is not a boolen");
		m_field_value.boolean_value =v;
		store(obj);
	}
	void FieldInfoBase::setString(Object* obj, String* v){
		MUST(isString(), "field type is not a string");
		m_field_value.string_value =v;
		store(obj);
	}
	void FieldInfoBase::setObject(Object* obj, Object* v){
		MUST(isObject(), "field type is not a object");
		m_field_value.object_value =v;
		store(obj);
	}

	void FieldInfoBase::setInt8(Object* obj, const int8_t v){
		MUST(isInt8(), "field type is not a int8");
		m_field_value.int8_value =v;
		store(obj);
	}
	void FieldInfoBase::setInt16(Object* obj, const int16_t v){
		MUST(isInt16(), "field type is not a int16");
		m_field_value.int16_value =v;
		store(obj);
	}
	void FieldInfoBase::setInt32(Object* obj, const int32_t v){
		MUST(isInt32(), "field type is not a int32");
		m_field_value.int32_value =v;
		store(obj);
	}
	void FieldInfoBase::setInt64(Object* obj, const int64_t v){
		MUST(isInt64(), "field type is not a int64");
		m_field_value.int64_value =v;
		store(obj);
	}

	void FieldInfoBase::setUint8(Object* obj, const uint8_t v){
		MUST(isUint8(), "field type is not a uint8");
		m_field_value.uint8_value =v;
		store(obj);
	}
	void FieldInfoBase::setUint16(Object* obj, const uint16_t v){
		MUST(isUint16(), "field type is not a uint16");
		m_field_value.uint16_value =v;
		store(obj);
	}
	void FieldInfoBase::setUint32(Object* obj, const uint32_t v){
		MUST(isUint32(), "field type is not a uint32");
		m_field_value.uint32_value =v;
		store(obj);
	}
	void FieldInfoBase::setUint64(Object* obj, const uint64_t v){
		MUST(isUint64(), "field type is not a uint64");
		m_field_value.uint64_value =v;
		store(obj);
	}

	void FieldInfoBase::setFloat32(Object* obj, const float32_t v){
		MUST(isFloat32(), "field type is not a float32");
		m_field_value.float32_value =v;
		store(obj);
	}
	void FieldInfoBase::setFloat64(Object* obj, const float64_t v){
		MUST(isFloat64(), "field type is not a float64");
		m_field_value.float64_value =v;
		store(obj);
	}

}
