/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpRespond ***/
	/** ctor & dtor **/
	HttpRespond::HttpRespond(Requestor* requestor)
		: m_version(0)
		, m_code(200)
		, m_msg(0)
		, m_header_tb(0)
		, m_cookie_tb(0)
		, m_content(0)
		, m_post_tb(0)
		, m_requestor(requestor)
		, m_auto_flush(true){
	}
	HttpRespond::~HttpRespond(){
	}

	/** Object **/
	void HttpRespond::init(){
		ASSIGN_POINTER(m_header_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_cookie_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_content, SafeNew<Bytes>());
		RETAIN_POINTER(m_requestor);
		Super::init();
	}
	void HttpRespond::finalize(){
		// head line
		CLEAN_POINTER(m_version);
		CLEAN_POINTER(m_msg);

		// header
		CLEAN_POINTER(m_header_tb);
		CLEAN_POINTER(m_cookie_tb);

		// content
		CLEAN_POINTER(m_content);

		// mime
		CLEAN_POINTER(m_post_tb);

		// misc
		CLEAN_POINTER(m_requestor);

		Super::finalize();
	}
	/** head line **/
	void HttpRespond::setVersion(String* version){
		ASSIGN_POINTER(m_version, version);
	}
	String* HttpRespond::getVersion(){
		return m_version;
	}

	void HttpRespond::setCode(const int64_t code){
		m_code =code;
	}
	int64_t HttpRespond::getCode(){
		return m_code;
	}

	void HttpRespond::setMsg(String* msg){
		ASSIGN_POINTER(m_msg, msg);
	}
	String* HttpRespond::getMsg(){
		return m_msg;
	}

	/** header & cookie **/
	void HttpRespond::setHeader(String* name, String* value){
		m_header_tb->set(name, value);
	}
	String* HttpRespond::getHeader(String* name){
		return static_cast< String* >(m_header_tb->get(name));
	}
	Hash* HttpRespond::getHeaderTable(){
		return m_header_tb;
	}

	void HttpRespond::setCookie(String* name, String* value, const int64_t max_age){
		HttpCookie* cookie =SafeNew<HttpCookie>();
		cookie->setName(name);
		cookie->setValue(value);
		cookie->setMaxAge(max_age);
		m_cookie_tb->set(name, cookie);
	}
	void HttpRespond::setCookie(String* name, HttpCookie* cookie){
		m_cookie_tb->set(name, cookie);
	}
	HttpCookie* HttpRespond::getCookie(String* name){
		return static_cast< HttpCookie* >(m_cookie_tb->get(name));
	}
	Hash* HttpRespond::getCookieTable(){
		return m_cookie_tb;
	}

	/** content **/
	void HttpRespond::write(const char* str){
		if(str){
			write(str, strlen(str));
		}
	}
	void HttpRespond::write(const char* str, int64_t len){
		m_content->append(str, len);
	}
	void HttpRespond::write(String* str){
		if(str){
			write(str->c_str(), str->size());
		}
	}
	Bytes* HttpRespond::getContent(){
		return m_content;
	}

	/** mime **/
	String* HttpRespond::getPost(String* name){
		return m_post_tb ? static_cast< String* >(m_post_tb->get(name)) : 0;
	}
	Hash* HttpRespond::getPostTable(){
		return m_post_tb;
	}

	/** flush **/
	Requestor* HttpRespond::getRequestor(){
		return m_requestor;
	}
	void HttpRespond::enableAutoFlush(const bool yes){
		m_auto_flush =yes;
	}
	bool HttpRespond::isEnableAutoFlush(){
		return m_auto_flush;
	}
	bool HttpRespond::flush(){
		// check
		if(m_requestor == 0){
			WARN("http flush failed, requestor is null");
			return false;
		}

		//// build data
		BinaryCoder<4096> coder;

		/// head line
		// version
		if(m_version){
			coder.append(m_version);
		}
		else{
			coder.append("HTTP/1.1");
		}
		coder.append(" ");
		// code
		coder.append(String::Format("%lld", (long long)m_code));
		coder.append(" ");
		// msg
		coder.append(_code_to_msg(m_code));
		coder.append("\r\n");

		/// header
	  	if(!m_header_tb->has("Content-Type")){
			coder.append("Content-Type: text/html\r\n");
		}
	  	if(!m_header_tb->has("Content-Length")){
			coder.append(String::Format("Content-Length: %lld\r\n", (long long)m_content->size()));
		}
		HashIterator* it =static_cast< HashIterator* >(m_header_tb->iterator());
		while(it->next()){
			if(String* name =dynamic_cast< String* >(it->getKey())){
				if(String* value =dynamic_cast< String* >(it->getValue())){
					coder.append(name);
					coder.append(": ");
					coder.append(value);
					coder.append("\r\n");
				}
			}
		}
		it =static_cast< HashIterator* >(m_cookie_tb->iterator());
		while(it->next()){
			if(dynamic_cast< String* >(it->getKey())){
				if(HttpCookie* cookie =dynamic_cast< HttpCookie* >(it->getValue())){
					if(String* str =cookie->build(false)){
						coder.append(str);
						coder.append("\r\n");
					}
				}
			}
		}
		coder.append("\r\n");

		/// content
		if(m_content->size() > 0){
			coder.append(m_content->c_str(), m_content->size());
		}

		//// send
		const bool ok =m_requestor->send(coder.c_str(), coder.size());

		//// clear
		CLEAN_POINTER(m_version);
		m_code =200;
		CLEAN_POINTER(m_msg);
		m_header_tb->clear();
		m_cookie_tb->clear();
		m_content->clear();

		//// post process
		if(!ok){
			close();
		}
		m_auto_flush =false;
		return ok;
	}
	void HttpRespond::close(){
		CLEAN_POINTER(m_requestor);
	}
	HttpRespond* HttpRespond::Parse(const char* str){
		if(!str){
			return 0;
		}
		// prepare respond
		HttpRespond* respond =SafeNew<HttpRespond>(static_cast< Requestor* >(0));

		//// head line
		// version
		const char* cursor =strchr(str, ' ');
		if(!cursor || cursor==str){
			WARN("invalid http respond format, when parse version");
			return 0;
		}
		respond->setVersion(String::New(str, cursor-str));
		str =cursor+1;
		// code
		cursor =strchr(str, ' ');
		if(!cursor || cursor==str){
			WARN("invalid http respond format, when parse code");
			return 0;
		}
		String* code =String::New(str, cursor-str);
		if(!FromString<int64_t>(code, respond->m_code)){
			WARN("invalid http respond format, when parse code");
			return 0;
		}
		str =cursor+1;
		// msg
		cursor =strstr(str, "\r\n");
		if(!cursor || cursor==str){
			WARN("invalid http respond format, when parse msg");
			return 0;
		}
		respond->setMsg(String::New(str, cursor-str));
		str =cursor+2;

		//// header
		int64_t content_len =0;
		while(*str){
			cursor =strstr(str, "\r\n");
			if(cursor == str){
				str =cursor+2;
				break;
			}
			else if(cursor == 0){
				WARN("invalid http respond format, when parse header");
				return 0;
			}
			else{
				const char* header =str;
				const char* header_end =cursor;
				str =header_end + 2;
				cursor =strstr(header, ": ");
				if(cursor==0 || cursor==header || cursor>header_end){
					WARN("invalid http respond format, when parse header name");
					return 0;
				}
				String* header_name =String::New(header, cursor-header);
				String* header_value =String::New(cursor+2, header_end-(cursor+2));
				if(header_name->is("Content-Length")){
					if(!FromString<int64_t>(header_value, content_len) || content_len<0){
						WARN("invalid http respond format, when parse content length");
						return 0;
					}
					respond->setHeader(header_name, header_value);
				}
				else if(header_name->is("Set-Cookie")){
					if(HttpCookie* cookie =HttpCookie::Parse(header_value->c_str())){
						respond->setCookie(cookie->getName(), cookie);
					}
					else{
						return 0;
					}
				}
				else{
					respond->setHeader(header_name, header_value);
				}
			}
		}

		//// content
		respond->m_content->append(str, content_len);
		
		//// mime
		if(!respond->_parse_mime()){
			return 0;
		}
		return respond;
	}
	const char* HttpRespond::_code_to_msg(const int64_t code){
		if(m_msg){
			return m_msg->c_str();
		}
		switch(code){
		case 200:
			return "OK";
		case 400:
			return "Bad Request";
		case 404:
			return "Not Found";
		case 503:
			return "Service Unavailable";
		default:
			return "Unknown";
		}
	}
	bool HttpRespond::_parse_mime(){
		// check
		if(!m_content || !m_header_tb){
			return true;
		}
		String* content_type =static_cast< String* >(m_header_tb->get("Content-Type"));
		if(!content_type){
			return true;
		}
		// parse
		if(content_type->is("multipart/form-data")){
			WARN("parse mime failed, multipart/form-data not support");
			return false;
		}
		else if(content_type->is("application/x-www-form-urlencoded")){
			ASSIGN_POINTER(m_post_tb, SafeNew<Hash>());
			if(Url::ParseQuery(String::New(m_content->c_str(), m_content->size()), m_post_tb)){
				return true;
			}
			else{
				WARN("parse mime failed, application/x-www-form-urlencoded invalid");
				return false;
			}
		}
		else{
			return true;
		}
	}
}
