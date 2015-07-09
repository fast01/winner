/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BINARY_CODER_HPP__
#define H_CORE_BINARY_CODER_HPP__
namespace core{
	/** BinaryCoder **/
	template<int64_t N =4096>
	class BinaryCoder{
		static_assert(N>0, "BinaryCoder template paramter N can't be <= 0");
	private:
		char m_fixed_data[N];
		void* m_data;
		int64_t m_capacity;
		int64_t m_length;
		int64_t m_read_cursor;
		int64_t m_write_cursor;
		bool m_good;
	public:
		BinaryCoder()
			: m_data(m_fixed_data)
			, m_capacity(N)
			, m_length(0)
			, m_read_cursor(0)
			, m_write_cursor(0)
			, m_good(true){
		}
		BinaryCoder(const void* data, const int64_t len)
			: m_data(m_fixed_data)
			, m_capacity(N)
			, m_length(0)
			, m_read_cursor(0)
			, m_write_cursor(0)
			, m_good(true){
			set(data, len);
		}
		BinaryCoder(const BinaryCoder& bc)
			: m_data(m_fixed_data)
			, m_capacity(N)
			, m_length(0)
			, m_read_cursor(0)
			, m_write_cursor(0)
			, m_good(true){
			set(bc.m_data, bc.m_length);
		}
		~BinaryCoder(){
			if(m_data != m_fixed_data){
				DEALLOCATE(m_data);
			}
		}
	public:
		BinaryCoder& operator =(const BinaryCoder& bc){
			set(bc.m_data, bc.m_length);
			return *this;
		}
		bool operator ==(const BinaryCoder& bc)const{
			if(bc.m_length == m_length){
				if(m_length == 0){
					return true;
				}
				else{
					return 0 == memcmp(m_data, bc.m_data, m_length);
				}
			}
			else{
				return false;
			}
		}
	public:
		void* data(){
			return m_data;
		}
		const void* data()const{
			return m_data;
		}
		char* c_str(){
			return (char*)m_data;
		}
		const char* c_str()const{
			return (char*)m_data;
		}
		int64_t capacity()const{
			return m_capacity;
		}
		int64_t length()const{
			return m_length;
		}
		int64_t size()const{
			return m_length;
		}
		int64_t getWriteCursor()const{
			return m_write_cursor;
		}
		int64_t getReadCursor()const{
			return m_read_cursor;
		}
		int64_t getRemainCount()const{
			return m_length - m_read_cursor;
		}
		bool good()const{
			return m_good;
		}
		bool bad()const{
			return !m_good;
		}
	public:
		void clearState(){
			m_good =true;
		}
		void setState(const bool state){
			m_good =state;
		}
		void set(const void* data, const int64_t len){
			clear();
			if(data && len>0){
				reserve(len);
				m_length =len;
				m_write_cursor =len;
				memcpy(m_data, data, (size_t)len);
			}
		}
		void clear(){
			if(m_data != m_fixed_data){
				DEALLOCATE(m_data);
				m_data =m_fixed_data;
			}
			m_capacity =N;
			m_length =0;
			m_read_cursor =0;
			m_write_cursor =0;
			m_good =true;
		}
		void reserve(const int64_t len){
			if(len <= m_capacity){
				return;
			}
			m_capacity =((len%64) ? ((len/64 + 1)*64) : len);
			if(m_data == m_fixed_data){
				m_data =ALLOCATE(m_capacity);
				if(m_length > 0){
					memcpy(m_data, m_fixed_data, m_length);
				}
			}
			else{
				m_data =REALLOCATE(m_data, m_capacity);
			}
		}
		void resize(const int64_t len){
			// reserve
			if(len < 0){
				m_length =0;
			}
			else{
				m_length =len;
			}
			reserve(m_length);

			// adjust cursor
			if(m_write_cursor > m_length){
				m_write_cursor =m_length;
			}
			if(m_read_cursor > m_length){
				m_read_cursor =m_length;
			}
		}
		void zero(){
			fill(0);
		}
		void one(){
			fill(0xFF);
		}
		void fill(const int32_t val){
			memset(m_data, static_cast<int>(val), m_capacity);
		}
		void appendNull(){
			write("\0", 1);
		}
		void write(const void* data, const int64_t len){
			if(!data || len <= 0) return;
			const int64_t space =m_length - m_write_cursor;
			if(space < len){
				resize(m_length + len - space);
			}
			memcpy(((uint8_t*)m_data)+m_write_cursor, data, (size_t)len);
			m_write_cursor +=len;
		}
		bool pick(void* data, const int64_t len)const{
			if(!data || len<=0 || (m_length-m_read_cursor)<len){
				return false;
			}
			char* src =(char*)m_data;
			memcpy(data, src+m_read_cursor, len);
			return true;
		}
		bool read(void* data, const int64_t len){
			if(pick(data, len)){
				m_read_cursor +=len;
				return true;
			}
			return false;
		}
		bool skip(const int64_t len){
			if((m_length-m_read_cursor)<len){
				return false;
			}
			m_read_cursor +=len;
			return true;
		}
		void rewind(){
			m_read_cursor =0;	
			m_write_cursor =0;	
		}
		void setReadCursor(const int64_t cursor){
			m_read_cursor =cursor;
			if(m_read_cursor < 0) m_read_cursor =0;
			if(m_read_cursor > m_length) m_read_cursor =m_length;
		}
		void setWriteCursor(const int64_t cursor){
			m_write_cursor =cursor;	
			if(m_write_cursor < 0) m_write_cursor =0;
			if(m_write_cursor > m_length) m_write_cursor =m_length;
		}
		BinaryCoder& append(const char* str){
			if(str){
				write(str, strlen(str));
			}
			return *this;
		}
		BinaryCoder& append(const char* str, int64_t len){
			if(str){
				if(len < 0){
					len =strlen(str);
				}
				write(str, len);
			}
			return *this;
		}
		BinaryCoder& append(String* str){
			if(str){
				write(str->c_str(), str->size());
			}
			return *this;
		}
		BinaryCoder& append(const uint8_t* data, const int64_t len){
			write(data, len);
			return *this;
		}
		BinaryCoder& append(const BinaryCoder& bc){
			write(static_cast< char* >(bc.m_data), bc.m_size);
			return *this;
		}
	public:
		// boolean
		void encodeBoolean(const bool v){
			char sz[1] ={0};
			const int64_t n =encode_boolean(sz, sizeof(sz), v);
			write(sz, n);
		}
		bool decodeBoolean(bool& v){
			const int64_t n =decode_boolean(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		bool decodeBoolean(){
			bool v =false;
			m_good =decodeBoolean(v);
			return v;
		}
		// float32
		void encodeFloat32(const float32_t v){
			char sz[sizeof(v)] ={0};
			const int64_t n =encode_float32(sz, sizeof(sz), v);
			write(sz, n);
		}
		bool decodeFloat32(float32_t & v){
			const int64_t n =decode_float32(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		float32_t decodeFloat32(){
			float32_t v =0;
			m_good =decodeFloat32(v);
			return v;
		}
		// float64
		void encodeFloat64(const float64_t v){
			char sz[sizeof(v)] ={0};
			const int64_t n =encode_float64(sz, sizeof(sz), v);
			write(sz, n);
		}
		bool decodeFloat64(float64_t & v){
			const int64_t n =decode_float64(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		float64_t decodeFloat64(){
			float64_t v =0;
			m_good =decodeFloat64(v);
			return v;
		}
		// integer
		void encodeInt64(const int64_t v){
			char sz[10] ={0};
			const int64_t n =encode_int64(sz, 10, v);
			write(sz, n);
		}
		void encodeUint64(const uint64_t v){
			char sz[10] ={0};
			const int64_t n =encode_uint64(sz, 10, v);
			write(sz, n);
		}
		bool decodeInt64(int64_t& v){
			const int64_t n =decode_int64(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		int64_t decodeInt64(){
			int64_t v =0;
			m_good =decodeInt64(v);
			return v;
		}
		bool decodeUint64(uint64_t& v){
			const int64_t n =decode_uint64(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		uint64_t decodeUint64(){
			uint64_t v =0;
			m_good =decodeUint64(v);
			return v;
		}
		// string
		void encodeString(const char* str, int64_t len){
			if(!str){
				encodeInt64(-1); // null object
			}
			else{
				if(len < 0){
					len =strlen(str);
				}
				encodeInt64(len);
				write(str, len);
			}
		}
		void encodeString(String* str){
			if(!str){
				// null object
				encodeInt64(-1);
			}
			else{
				encodeInt64(str->size());
				write(str->c_str(), str->size());
			}
		}
		bool decodeString(String*& v){
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
			v =String::NewString(c_str()+m_read_cursor, cnt);
			skip(cnt);
			return true;
		}
		String* decodeString(){
			String* v =0;
			m_good =decodeString(v);
			return v;
		}
		// packet
		void encodePacket(const PACKET& v){
			char sz[PACKET_ENCODE_MAX_SIZE] ={0};
			const int64_t n =encode_packet(sz, sizeof(sz), v);
			write(sz, n);
		}
		bool decodePacket(PACKET& v){
			const int64_t n =decode_packet(c_str()+m_read_cursor, getRemainCount(), v);
			if(n < 0){
				return false;
			}
			skip(n);
			return true;
		}
		PACKET decodePacket(){
			PACKET v;
			m_good =decodePacket(v);
			return v;
		}
	public:
		void writeInt8(const int8_t v){
			write(&v, sizeof(v));
		}
		void writeInt16(const int16_t v){
			write(&v, sizeof(v));
		}
		void writeInt32(const int32_t v){
			write(&v, sizeof(v));
		}
		void writeInt64(const int64_t v){
			write(&v, sizeof(v));
		}
		void writeUint8(const uint8_t v){
			write(&v, sizeof(v));
		}
		void writeUint16(const uint16_t v){
			write(&v, sizeof(v));
		}
		void writeUint32(const uint32_t v){
			write(&v, sizeof(v));
		}
		void writeUint64(const uint64_t v){
			write(&v, sizeof(v));
		}
		void writeFloat32(const float32_t v){
			write(&v, sizeof(v));
		}
		void writeFloat64(const float64_t v){
			write(&v, sizeof(v));
		}

		int8_t readInt8(){
			int8_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		int16_t readInt16(){
			int16_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		int32_t readInt32(){
			int32_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		int64_t readInt64(){
			int64_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		uint8_t readUint8(){
			uint8_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		uint16_t readUint16(){
			uint16_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		uint32_t readUint32(){
			uint32_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		uint64_t readUint64(){
			uint64_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		float32_t readFloat32(){
			float32_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
		float64_t readFloat64(){
			float64_t v =0;
			m_good =read(&v, sizeof(v));
			return v;
		}
	};
}

#endif
