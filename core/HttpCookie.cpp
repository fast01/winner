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
		, m_ExpireTime(0)
		, m_Secure(false)
		, m_HttpOnly(false){
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
		CLEAN_POINTER(m_ExpireTime);
		Super::finalize();
	}
	/** SELF **/
	DEFINE_PROPERTY_P(HttpCookie, String*, Name)
	DEFINE_PROPERTY_P(HttpCookie, String*, Value)
	DEFINE_PROPERTY_P(HttpCookie, String*, Domain)
	DEFINE_PROPERTY_P(HttpCookie, String*, Path)
	DEFINE_PROPERTY(HttpCookie, int64_t, MaxAge)
	DEFINE_PROPERTY_P(HttpCookie, String*, ExpireTime)
	DEFINE_PROPERTY(HttpCookie, bool, Secure)
	DEFINE_PROPERTY(HttpCookie, bool, HttpOnly)

	String* HttpCookie::build(const bool client){
		if(!m_Name || m_Name->empty() || !m_Value || m_Value->empty()){
			WARN("build cookie failed, name or value is invalid");
			return 0;
		}
		BinaryCoder<1024> coder;
		if(client){
			coder.append("Cookie: ", 8);
		}
		else{
			coder.append("Set-Cookie: ", 12);
		}

		// name = value
		coder.append(m_Name);
		coder.append("=", 1);
		coder.append(m_Value);

		if(!client){
			// domain
			if(m_Domain){
				coder.append("; domain=", 9);
				coder.append(m_Domain);
			}

			// path
			if(m_Domain){
				coder.append("; path=", 7);
				coder.append(m_Path);
			}

			// secure
			if(m_Secure){
				coder.append("; secure", 8);
			}

			// HttpOnly
			if(m_HttpOnly){
				coder.append("; HttpOnly", 10);
			}

			// max age
			if(m_MaxAge > 0){
				coder.append("; max-age=", 10);
				coder.append(String::Format("%lld", (long long)m_MaxAge));
			}

			// expires
			if(m_ExpireTime){
				coder.append("; expires=", 10);
				coder.append(m_ExpireTime);
			}
		}
		return String::New(coder.c_str(), coder.size());
	}
	/** parse **/
	HttpCookie* HttpCookie::Parse(const char* str){
		if(!str){
			return 0;
		}
		// separate tokens
		Array* ls =String::New(str)->split(";");
		if(!ls){
			ERROR("invalid cookie `%s` syntax", str);
			return 0;
		}
		const int64_t count =ls->size();
		for(int64_t i=0; i<count; ++i){
			String* token =static_cast< String* >(ls->get(i));
			ls->set(i, token->trim());
		}

		// parse
		HttpCookie* cookie =SafeNew<HttpCookie>();

		// name=value
		if(String* token =static_cast< String* >(ls->get(0))){
			const int64_t idx =token->indexOf("=");
			if(idx < 0){
				ERROR("invalid cookie `%s` syntax", str);
				return 0;
			}
			cookie->setName(token->subString(0, idx));
			cookie->setValue(token->subString(idx+1, -1));
		}

		// others
		for(int64_t i=1; i<count; ++i){
			String* token =static_cast< String* >(ls->get(i));
			const char* token_str =token->c_str();
			if(0 == strcasecmp(token_str, "secure")){
				cookie->setSecure(true);
			}
			else if(0==strcasecmp(token_str, "HttpOnly") || 0==strcasecmp(token_str, "http-only")){
				cookie->setHttpOnly(true);
			}
			else{
				const int64_t idx =token->indexOf("=");
				if(idx < 0){
					ERROR("invalid cookie `%s` syntax", str);
					return 0;
				}
				String* k =token->subString(0, idx);
				String* v =token->subString(idx+1, -1);
				const char* k_str =k->c_str();
				if(0 == strcasecmp(k_str, "max-age")){
					int64_t max_age =0;
					if(!FromString<int64_t>(v, max_age)){
						ERROR("invalid cookie `%s` syntax", str);
						return 0;
					}
					cookie->setMaxAge(max_age);
				}
				else if(0 == strcasecmp(k_str, "path")){
					cookie->setPath(v);
				}
				else if(0 == strcasecmp(k_str, "domain")){
					cookie->setDomain(v);
				}
				else if(0 == strcasecmp(k_str, "expires")){
					cookie->setExpireTime(v);
				}
			}
		}
		return cookie;
	}
}
