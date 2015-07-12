/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpCookie ***/
	/** ctor & dtor **/
	HttpCookie::HttpCookie()
		: m_Name(0)
		, m_Value(0)
		, m_Domain(0)
		, m_Path(0)
		, m_MaxAge(0)
		, m_Secure(false){
	}
	HttpCookie::~HttpCookie(){
	}

	/** Object **/
	void HttpCookie::init(){
		Super::init();
	}
	void HttpCookie::finalize(){
		CLEAN_POINTER(m_Name);
		CLEAN_POINTER(m_Value);
		CLEAN_POINTER(m_Domain);
		CLEAN_POINTER(m_Path);
		Super::finalize();
	}
	/** SELF **/
	DEFINE_PROPERTY_P(HttpCookie, String*, Name)
	DEFINE_PROPERTY_P(HttpCookie, String*, Value)
	DEFINE_PROPERTY_P(HttpCookie, String*, Domain)
	DEFINE_PROPERTY_P(HttpCookie, String*, Path)
	DEFINE_PROPERTY(HttpCookie, int64_t, MaxAge)
	DEFINE_PROPERTY(HttpCookie, bool, Secure)

	String* HttpCookie::build(){
		if(!m_Name || m_Name->empty() || !m_Value || m_Value->empty()){
			WARN("build cookie failed, name or value is invalid");
			return 0;
		}
		BinaryCoder<1024> coder;
		coder.append("Set-Cookie: ", 12);

		// name = value
		coder.append(UrlEncode::Encode(m_Name));
		coder.append("=", 1);
		coder.append(UrlEncode::Encode(m_Value));

		// domain
		if(m_Domain){
			coder.append(";Domain=", 8);
			coder.append(UrlEncode::Encode(m_Domain));
		}

		// path
		if(m_Domain){
			coder.append(";Path=", 6);
			coder.append(UrlEncode::Encode(m_Path));
		}

		// secure
		if(m_Secure){
			coder.append(";Secure", 7);
		}

		// max age
		if(m_MaxAge > 0){
			coder.append(";Max-Age=", 9);
			coder.append(String::Format("%lld", (long long)m_MaxAge));
		}
		return String::New(coder.c_str(), coder.size());
	}
	/** parse **/
	HttpCookie* HttpCookie::Parse(const char* str){
		if(!str){
			return false;
		}
		if(strchr(str, '&')){
			WARN("invalid cookie format:%s", str);
			return 0;
		}
		const char* cursor =strchr(str, '=');
		if(cursor==0 || cursor==str){
			WARN("invalid cookie format:%s", str);
			return 0;
		}
		// name
		String* name =String::New(str, cursor-str);
		name =UrlEncode::Decode(name);

		// value
		String* value =String::New(cursor+1);
		value =UrlEncode::Decode(value);

		if(name && value){
			HttpCookie* cookie =SafeNew<HttpCookie>();
			cookie->setName(name);
			cookie->setValue(value);
			return cookie;
		}
		else{
			return 0;
		}
	}
}
