/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** Bytes impl ***/
	/** Script **/
	DEFINE_SUPPORT_SCRIPT(Bytes, "core::Bytes")

	/** ctor & dtor **/
	Bytes::Bytes(){
	}
	Bytes::Bytes(const void* data, const int64_t len)
		: m_coder(data, len){
	}
	Bytes::~Bytes(){
	}

	/** new **/
	Bytes* Bytes::New(){
		return SafeNew<Bytes>();
	}

	/** Object **/
	Object* Bytes::clone(){
		Bytes* o =core::SafeNew<Bytes>();
		o->set(data(), length());
		return o;
	}
	bool Bytes::equals(Object* obj){
		if(const Bytes* o =dynamic_cast<Bytes*>(obj)){
			return m_coder == o->m_coder;
		}
		else{
			return false;
		}
	}
	int64_t Bytes::getHashCode(){
		const int64_t x =reinterpret_cast<int64_t>(data());
		return x<0 ? (-(x/2)) : x;
	}
	String* Bytes::toString(){
		return String::New(c_str(), length());
	}
	bool Bytes::fromBytes(Bytes* bytes){
		if(!bytes){
			set(bytes->c_str(), bytes->size());
		}
		else{
			clear();
		}
		return true;
	}
	Bytes* Bytes::toBytes(Bytes* bytes){
		return this;
	}


	/** getter **/
	void* Bytes::data(){
		return m_coder.data();
	}
	char* Bytes::c_str(){
		return m_coder.c_str();
	}
	int64_t Bytes::capacity(){
		return m_coder.capacity();
	}
	int64_t Bytes::length(){
		return m_coder.length();
	}
	int64_t Bytes::size(){
		return m_coder.size();
	}
	int64_t Bytes::getWriteCursor(){
		return m_coder.getWriteCursor();
	}
	int64_t Bytes::getReadCursor(){
		return m_coder.getReadCursor();
	}
	int64_t Bytes::getRemainCount(){
		return m_coder.getRemainCount();
	}
	bool Bytes::good(){
		return m_coder.good();
	}
	bool Bytes::bad(){
		return m_coder.bad();
	}

	/** basic ctrl **/
	void Bytes::clearState(){
		m_coder.clearState();
	}
	void Bytes::set(const void* data, const int64_t len){
		m_coder.set(data, len);
	}
	void Bytes::clear(){
		m_coder.clear();
	}
	void Bytes::reserve(const int64_t len){
		m_coder.reserve(len);
	}
	void Bytes::resize(const int64_t len){
		m_coder.resize(len);
	}
	void Bytes::zero(){
		m_coder.zero();
	}
	void Bytes::one(){
		m_coder.one();
	}
	void Bytes::appendNull(){
		m_coder.appendNull();
	}
	void Bytes::fill(const int32_t val){
		m_coder.fill(val);
	}
	void Bytes::write(const void* data, const int64_t len){
		m_coder.write(data, len);
	}
	bool Bytes::pick(void* data, const int64_t len){
		return m_coder.pick(data, len);
	}
	bool Bytes::read(void* data, const int64_t len){
		return m_coder.read(data, len);
	}
	bool Bytes::skip(const int64_t len){
		return m_coder.skip(len);
	}
	void Bytes::rewind(){
		m_coder.rewind();
	}
	void Bytes::setReadCursor(const int64_t cursor){
		m_coder.setReadCursor(cursor);
	}
	void Bytes::setWriteCursor(const int64_t cursor){
		m_coder.setWriteCursor(cursor);
	}
	Bytes* Bytes::appendString(const char* str){
		m_coder.append(str);
		return this;
	}
	Bytes* Bytes::appendString(String* str){
		m_coder.append(str);
		return this;
	}
	Bytes* Bytes::append(const void* data, const int64_t len){
		m_coder.append(static_cast< const uint8_t* >(data), len);
		return this;
	}
	Bytes* Bytes::append(Bytes* bs){
		if(bs){
			m_coder.append(bs->c_str(), bs->length());
		}
		return this;
	}
	Bytes* Bytes::appendStringNull(){
		char ch =0;
		write(&ch, 1);
		return this;
	}

	/*** encode decode bool ***/
	/** encode decode bool **/
	void Bytes::encodeBoolean(const bool v){
		m_coder.encodeBoolean(v);
	}
	bool Bytes::decodeBoolean(bool& v){
		return m_coder.decodeBoolean(v);
	}
	bool Bytes::decodeBoolean(){
		return m_coder.decodeBoolean();
	}

	/** encode decode float **/
	void Bytes::encodeFloat32(const float32_t v){
		m_coder.encodeFloat32(v);
	}
	bool Bytes::decodeFloat32(float32_t & v){
		return m_coder.decodeFloat32(v);
	}
	float32_t Bytes::decodeFloat32(){
		return m_coder.decodeFloat32();
	}

	void Bytes::encodeFloat64(const float64_t v){
		m_coder.encodeFloat64(v);
	}
	bool Bytes::decodeFloat64(float64_t & v){
		return m_coder.decodeFloat64(v);
	}
	float64_t Bytes::decodeFloat64(){
		return m_coder.decodeFloat64();
	}
	/** encode decode int **/
	void Bytes::encodeInt64(const int64_t v){
		m_coder.encodeInt64(v);
	}
	void Bytes::encodeUint64(const uint64_t v){
		m_coder.encodeUint64(v);
	}
	bool Bytes::decodeInt64(int64_t& v){
		return m_coder.decodeInt64(v);
	}
	int64_t Bytes::decodeInt64(){
		return m_coder.decodeInt64();
	}
	bool Bytes::decodeUint64(uint64_t& v){
		return m_coder.decodeUint64(v);
	}
	uint64_t Bytes::decodeUint64(){
		return m_coder.decodeUint64();
	}

	/** encode decode string **/
	void Bytes::encodeString(const char* str, int64_t len){
		m_coder.encodeString(str, len);
	}
	void Bytes::encodeString(String* str){
		m_coder.encodeString(str);
	}
	bool Bytes::decodeString(String*& v){
		return m_coder.decodeString(v);
	}
	String* Bytes::decodeString(){
		return m_coder.decodeString();
	}
	/** encode decode bytes **/
	void Bytes::encodeBytes(const void* data, const int64_t s){
		if(data && s>=0){
			encodeInt64(s);
			append(data, s);
		}
		else{
			encodeInt64(-1); // null object
		}
	}
	void Bytes::encodeBytes(Bytes* bs){
		if(bs){
			encodeInt64(bs->size());
			append(bs);
		}
		else{
			encodeInt64(-1); // null object
		}
	}
	bool Bytes::decodeBytes(Bytes*& v){
		int64_t cnt =0;
		if(!decodeInt64(cnt)){
			return false;
		}
		if(cnt == -1){ // null object
			v =0;
			return true;
		}
		if(getRemainCount() < cnt){
			return false;
		}
		const int64_t cursor =getReadCursor();
		v =SafeNew<Bytes>();
		v->write(c_str() + cursor, cnt);
		skip(cnt);
		return true;
	}
	Bytes* Bytes::decodeBytes(){
		Bytes* v =0;
		if(!decodeBytes(v)){
			m_coder.setState(false);
		}
		return v;
	}
	/** encode decode packet **/
	void Bytes::encodePacket(const PACKET& v){
		m_coder.encodePacket(v);
	}
	bool Bytes::decodePacket(PACKET& v){
		return m_coder.decodePacket(v);
	}
	PACKET Bytes::decodePacket(){
		return m_coder.decodePacket();
	}

	/*** read write ***/
	void Bytes::writeInt8(const int8_t v){
		m_coder.writeInt8(v);
	}
	void Bytes::writeInt16(const int16_t v){
		m_coder.writeInt16(v);
	}
	void Bytes::writeInt32(const int32_t v){
		m_coder.writeInt32(v);
	}
	void Bytes::writeInt64(const int64_t v){
		m_coder.writeInt64(v);
	}
	void Bytes::writeUint8(const uint8_t v){
		m_coder.writeUint8(v);
	}
	void Bytes::writeUint16(const uint16_t v){
		m_coder.writeUint16(v);
	}
	void Bytes::writeUint32(const uint32_t v){
		m_coder.writeUint32(v);
	}
	void Bytes::writeUint64(const uint64_t v){
		m_coder.writeUint64(v);
	}
	void Bytes::writeFloat32(const float32_t v){
		m_coder.writeFloat32(v);
	}
	void Bytes::writeFloat64(const float64_t v){
		m_coder.writeFloat64(v);
	}

	int8_t Bytes::readInt8(){
		return m_coder.readInt8();
	}
	int16_t Bytes::readInt16(){
		return m_coder.readInt16();
	}
	int32_t Bytes::readInt32(){
		return m_coder.readInt32();
	}
	int64_t Bytes::readInt64(){
		return m_coder.readInt64();
	}
	uint8_t Bytes::readUint8(){
		return m_coder.readUint8();
	}
	uint16_t Bytes::readUint16(){
		return m_coder.readUint16();
	}
	uint32_t Bytes::readUint32(){
		return m_coder.readUint32();
	}
	uint64_t Bytes::readUint64(){
		return m_coder.readUint64();
	}
	float32_t Bytes::readFloat32(){
		return m_coder.readFloat32();
	}
	float64_t Bytes::readFloat64(){
		return m_coder.readFloat64();
	}
}
