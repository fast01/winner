/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BYTES__
#define H_CORE_BYTES__
namespace core{
	/** Bytes **/
	class Bytes : public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_SUPPORT_SCRIPT
	public:
		Bytes();
		Bytes(const void* v, const int64_t len);
		virtual ~Bytes();
	public:
		static Bytes* New();
	public:
		virtual Object* clone();
		virtual bool equals(Object* obj);
		virtual int64_t getHashCode();
		virtual String* toString();
		virtual bool fromBytes(Bytes* bytes);
		virtual Bytes* toBytes(Bytes* bytes);
	public:
		void* data();
		char* c_str();
		int64_t capacity();
		int64_t length();
		int64_t size();
		int64_t getWriteCursor();
		int64_t getReadCursor();
		int64_t getRemainCount();
		bool good();
		bool bad();
		void clearState();
	public:
		void set(const void* data, const int64_t len);
		void clear();
	public:
		void reserve(const int64_t len);
		void resize(const int64_t len);
		void zero();
		void one();
		void appendNull();
		void fill(const int32_t val);
		void write(const void* data, const int64_t len);
		bool pick(void* data, const int64_t len);
		bool read(void* data, const int64_t len);
		bool skip(const int64_t len);
		void rewind();
		void setReadCursor(const int64_t cursor);
		void setWriteCursor(const int64_t cursor);
		Bytes* appendString(const char* str);
		Bytes* appendString(String* str);
		Bytes* append(const void* data, const int64_t len);
		Bytes* append(Bytes* bs);
		Bytes* appendStringNull();
	public:
		void encodeBoolean(const bool v);
		bool decodeBoolean(bool& v);
		bool decodeBoolean();
	public:
		void encodeFloat32(const float32_t v);
		bool decodeFloat32(float32_t & v);
		float32_t decodeFloat32();

		void encodeFloat64(const float64_t v);
		bool decodeFloat64(float64_t & v);
		float64_t decodeFloat64();
	public:
		void encodeInt64(const int64_t v);
		bool decodeInt64(int64_t& v);
		int64_t decodeInt64();
	public:
		void encodeUint64(const uint64_t v);
		bool decodeUint64(uint64_t& v);
		uint64_t decodeUint64();
	public:
		void encodeString(const char* str, const int64_t len);
		void encodeString(String* str);
		bool decodeString(String*& v);
		String* decodeString();
	public:
		void encodeBytes(const void* data, const int64_t s);
		void encodeBytes(Bytes* bs);
		bool decodeBytes(Bytes*& v);
		Bytes* decodeBytes();
	public:
		void encodePacket(const PACKET& packet);
		bool decodePacket(PACKET& packet);
		PACKET decodePacket();
	public:
		void writeInt8(const int8_t v);
		void writeInt16(const int16_t v);
		void writeInt32(const int32_t v);
		void writeInt64(const int64_t v);
		void writeUint8(const uint8_t v);
		void writeUint16(const uint16_t v);
		void writeUint32(const uint32_t v);
		void writeUint64(const uint64_t v);
		void writeFloat32(const float32_t v);
		void writeFloat64(const float64_t v);
	public:
		int8_t readInt8();
		int16_t readInt16();
		int32_t readInt32();
		int64_t readInt64();
		uint8_t readUint8();
		uint16_t readUint16();
		uint32_t readUint32();
		uint64_t readUint64();
		float32_t readFloat32();
		float64_t readFloat64();
	private:
		BinaryCoder<32> m_coder;
	};
}

#endif
