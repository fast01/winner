/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** String impl ***/
	/** ctor & dtor **/
	String::String()
		: m_data(0)
		, m_length(0){
	}
	String::~String(){
		if(m_data)
			DEALLOCATE(m_data);
	}

	/** Object **/
	String* String::toString(){
		return this;
	}
	int64_t String::getHashCode(){
		return String::CalcHashCode(m_data, m_length);
	}
	bool String::equals(Object* str){
		if(this == str) return true;
		if(String* s =dynamic_cast<String*>(str)){
			if(s->m_data && m_data){
				return 0 == strcmp(m_data, s->m_data);
			}
			else if(s->m_data==0 && m_data==0){
				return true;
			}
			else{
				return false;
			}
		}
		return false;
	}

	/** Compareable **/
	int64_t String::compare(Compareable* obj){
		if(obj == 0){ return 1; }
		if(this == obj){
			return 0;
		}
		else{
			return strcmp(m_data, CAST_POINTER(obj, String)->m_data);
		}
	}

	/** new **/
	String* String::NewString(){
		String* o =SafeNew<String>();
		o->m_length =0;
		o->m_data =reinterpret_cast< char* >(ALLOCATE(o->m_length + 1));
		*(o->m_data) =0;
		return o;
	}
	String* String::NewString(const char* str){
		if(!str) return NewString();
		String* o =SafeNew<String>();
		o->m_length =(int64_t)strlen(str);
		o->m_data =reinterpret_cast< char* >(ALLOCATE(o->m_length + 1));
		if(o->m_length > 0){
			memcpy(o->m_data, str, (size_t)o->m_length);
		}
		o->m_data[o->m_length] =0;
		return o;
	}
	String* String::NewString(const char* str, const int64_t len){
		if(!str) return NewString();
		String* o =SafeNew<String>();
		if(len >= 0){
			o->m_length =len;
		}
		else{
			o->m_length =strlen(str);
		}
		o->m_data =reinterpret_cast< char* >(ALLOCATE(o->m_length + 1));
		if(o->m_length > 0){
			memcpy(o->m_data, str, (size_t)o->m_length);
		}
		o->m_data[o->m_length] =0;
		return o;
	}
	String* String::New(){
		return String::NewString();
	}
	String* String::New(const char* str){
		return String::NewString(str);
	}
	String* String::New(const char* str, const int64_t len){
		return String::NewString(str, len);
	}

	/** length **/
	int64_t String::Length(String* str){
		return str ? str->m_length : 0;
	}

	/** format **/
	String* String::FormatV(const char* fmt, va_list vl){
		va_list vc;
		va_copy(vc, vl);
		int64_t len =0;
		len =vsnprintf(0, 0, fmt, vl);
		if(len < 0){
			OutputDebug("String::FormatV:vsnprintf fail");
			return 0;
		}
		else if(len == 0){
			return NewString();
		}
		else{
			String* o =NewString();
			o->m_length =len;
			o->m_data =reinterpret_cast< char* >(REALLOCATE(o->m_data, o->m_length + 1));
			vsnprintf(o->m_data, (size_t)(o->m_length+1), fmt, vc);
			return o;
		}
	}
	String* String::FormatV(String* fmt, va_list vl){
		return String::FormatV(fmt->c_str(), vl);
	}
	String* String::Format(const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		String* o =String::FormatV(fmt, vl);
		va_end(vl);
		return o;
	}
	String* String::Format(String* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		String* o =String::FormatV(fmt->c_str(), vl);
		va_end(vl);
		return o;
	}
	String* String::Repeat(const char ch, const int64_t cnt){
		if(cnt < 0) return 0;
		String* o =NewString();
		o->m_length =cnt;
		o->m_data =reinterpret_cast< char* >(REALLOCATE(o->m_data, o->m_length + 1));
		memset(o->m_data, ch, cnt);
		o->m_data[cnt] =0;
		return o;
	}

	/** query **/
	const char* String::c_str(){
		return m_data;
	}
	int64_t String::length(){
		return m_length;
	}
	int64_t String::size(){
		return m_length;
	}
	bool String::empty(){
		return m_length == 0;
	}
	char String::get(const int64_t idx){
		if(idx>=0 && idx < m_length){
			return m_data[idx];
		}
		else{
			return char();
		}
	}
	char String::at(const int64_t idx){
		return get(idx);
	}
	bool String::hasPrefix(const char* prefix){
		const int64_t len =strlen(prefix);
		if(len<=0){
			return true;
		}
		return indexOf(prefix) == 0;
	}
	bool String::hasPrefix(String* str){
		return hasPrefix(str->c_str());
	}
	bool String::hasSubfix(const char* subfix){
		const int64_t len =strlen(subfix);
		if(len <= 0){
			return true;
		}
		const int64_t idx =lastIndexOf(subfix);
		return idx>=0 ? (idx+len) == m_length : false;
	}
	bool String::hasSubfix(String* str){
		return hasSubfix(str->c_str());
	}
	int64_t String::indexOf(const char* str){
		const int64_t len =strlen(str);
		if(len<=0){
			return -1;
		}
		char* sz =strstr(m_data, str);
		if(sz){
			return sz - m_data;
		}
		else{
			return -1;
		}
	}
	int64_t String::indexOf(String* str){
		return indexOf(str->c_str());
	}
	int64_t String::lastIndexOf(const char* str){
		const int64_t len =strlen(str);
		if(len<=0){
			return -1;
		}
		int64_t last_idx =-1;
		char* cursor =m_data;
		while(cursor){
			char* tmp =strstr(cursor, str);
			if(tmp){
				last_idx =tmp - m_data;
				cursor +=1;
			}
			else{
				break;
			}
		}
		return last_idx;
	}
	int64_t String::lastIndexOf(String* str){
		return lastIndexOf(str->c_str());
	}
	bool String::is(const char* str){
		if(!m_data || !str) return false;
		return strcmp(str, m_data) == 0;
	}
	bool String::is(String* str){
		return equals(str);
	}

	/** builder **/
	String* String::append(const char* str){
		return String::Format("%s%s", m_data, str);
	}
	String* String::append(String* str){
		return append(str->c_str());
	}
	String* String::lower(){
		String* l =NewString(m_data, m_length);
		for(int64_t i=0; i< m_length; ++i){
			l->m_data[i] =static_cast<char>(tolower(m_data[i]));
		}
		return l;
	}
	String* String::upper(){
		String* l =NewString(m_data, m_length);
		for(int64_t i=0; i< m_length; ++i){
			l->m_data[i] =static_cast<char>(toupper(m_data[i]));
		}
		return l;
	}
	String* String::subString(int64_t start, int64_t count){
		if(start < 0) start =0;
		if(count < 0){
			count =m_length - start;
		}
		if(start >= m_length){
			return NewString();
		}
		if(start+count > m_length){
			count =m_length-start;
		}
		return NewString(m_data + start, count);
	}
	String* String::replace(const char* from, const char* to, const bool bGlobal){
		// prepare
		const int64_t from_len =strlen(from);
		const int64_t to_len =strlen(to);
		if(from_len <= 0 || to_len < 0){
			return 0;
		}
		// calc replace count
		int64_t rep_cnt =0;
		char* tmp =m_data;
		do{
			if((tmp =strstr(tmp, from))){
				tmp +=from_len;
				rep_cnt +=1;
			}
			else{
				break;
			}
		}while(bGlobal);
		if(rep_cnt == 0){
			return this;
		}

		// replace
		char* src_cursor =m_data;
		char* src_end =m_data + m_length;
		String* o =SafeNew<String>();
		o->m_length =rep_cnt * (to_len - from_len) + m_length;
		o->m_data =reinterpret_cast< char* >(ALLOCATE(o->m_length + 1));
		char* dst_cursor =o->m_data;
		do{
			if(char* f =strstr(src_cursor, from)){
				const int mv_cnt =(int)(f - src_cursor);
				if(mv_cnt > 0){
					memcpy(dst_cursor, src_cursor, mv_cnt);
					dst_cursor +=mv_cnt;
				}
				if(to_len){
					memcpy(dst_cursor, to, (int)to_len);
					dst_cursor +=to_len;
				}
				src_cursor =f + from_len;
			}
			else{
				break;
			}
		}while(bGlobal);
		if(src_end != src_cursor){
			memcpy(dst_cursor, src_cursor, (int)(src_end - src_cursor));
		}
		o->m_data[o->m_length] =0;
		return o;
	}
	String* String::replace(String* from, String* to, const bool bGlobal){
		return replace(from->c_str(), to->c_str(), bGlobal);
	}
	Array* String::split(const char* sep){
		// check & prepare
		if(!sep) return 0;
		if(m_length <= 0) return 0;
		Bytes* bs =SafeNew<Bytes>();
		bs->set(m_data, m_length+1);
		char* sz =bs->c_str();
		Array* arr =SafeNew<Array>();
		char* saveptr =0;

		// split
		char* token =strtok_r(sz, sep, &saveptr);
		while(token){
			arr->push_back(String::NewString(token));
			token =strtok_r(0, sep, &saveptr);
		}
		return arr;
	}
	Array* String::split(String* sep){
		if(!sep) return 0;
		return split(sep->c_str());
	}
	String* String::ltrim(){
		if(!m_data) return 0;
		if(m_length == 0) return this;
		int64_t cursor =0;
		for(; cursor<m_length; ++cursor){
			if(!isspace(m_data[cursor])){
				break;
			}
		}
		if(cursor == 0){
			return this;
		}
		else{
			return subString(cursor, m_length-cursor);
		}
	}
	String* String::rtrim(){
		if(!m_data) return 0;
		if(m_length == 0) return this;
		int64_t cursor =m_length-1;
		for(; cursor>=0; --cursor){
			if(!isspace(m_data[cursor])){
				break;
			}
		}
		if(cursor == m_length-1){
			return this;
		}
		else{
			return subString(0, cursor+1);
		}
	}
	String* String::trim(){
		if(String* str =ltrim()){
			return str->rtrim();
		}
		else{
			return 0;
		}
	}
	int64_t String::CalcHashCode(const char* str, int64_t len){
		if(!str) return 0;
		if(len < 0){
			len =strlen(str);
		}
		register uint32_t step =static_cast<uint32_t>(len<32 ? 1 : len/32);
		register uint32_t cnt =static_cast<uint32_t>(len / step);
		register uint32_t code =0;
		for(uint32_t i=0; i<cnt; i+=step){
			code = code*65599 + (uint32_t)(str[i]);
		}
		return static_cast<int64_t>(code);
	}
	String* String::EncodeQuotableString(String* str){
		if(!str) return 0;
		str =str->replace("\\", "\\\\", true);
		str =str->replace("\"", "\\\"", true);
		str =str->replace("\'", "\\\'", true);
		return str;
	}
}
