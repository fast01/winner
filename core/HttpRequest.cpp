/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpRequest ***/
	/** ctor & dtor **/
	HttpRequest::HttpRequest()
		: m_method(0)
		, m_path(0)
		, m_version(0)
		, m_url(0)
		, m_header_tb(0)
		, m_cookie_tb(0)
		, m_content(0)
		, m_post_tb(0){
	}
	HttpRequest::~HttpRequest(){
	}

	/** Object **/
	void HttpRequest::init(){
		Super::init();
	}
	void HttpRequest::finalize(){
		// head line
		CLEAN_POINTER(m_method);
		CLEAN_POINTER(m_path);
		CLEAN_POINTER(m_version);

		// url
		CLEAN_POINTER(m_url);

		// header & cookie
		CLEAN_POINTER(m_cookie_tb);
		CLEAN_POINTER(m_header_tb);

		// content
		CLEAN_POINTER(m_content);

		// mime
		CLEAN_POINTER(m_post_tb);

		Super::finalize();
	}

	/** head line **/
	String* HttpRequest::getMethod(){
		return m_method;
	}
	String* HttpRequest::getPath(){
		return m_path;
	}
	String* HttpRequest::getVersion(){
		return m_version;
	}

	/** url **/
	Url* HttpRequest::getUrl(){
		if(m_url == 0){
			m_url =SafeNew<Url>();
			if(m_url->parse(m_path)){
				RETAIN_POINTER(m_url);
			}
			else{
				m_url =0;
			}
		}
		return m_url;
	}

	/** header & cookie **/
	String* HttpRequest::getHeader(String* name){
		return m_header_tb ? static_cast< String* >(m_header_tb->get(name)) : 0;
	}
	Hash* HttpRequest::getHeaderTable(){
		return m_header_tb;
	}

	HttpCookie* HttpRequest::getCookie(String* name){
		return m_cookie_tb ? static_cast< HttpCookie* >(m_cookie_tb->get(name)) : 0;
	}
	Hash* HttpRequest::getCookieTable(){
		return m_cookie_tb;
	}

	/** content **/
	Bytes* HttpRequest::getContent(){
		return m_content;
	}

	/** mime **/
	String* HttpRequest::getPost(String* name){
		return m_post_tb ? static_cast< String* >(m_post_tb->get(name)) : 0;
	}
	Hash* HttpRequest::getPostTable(){
		return m_post_tb;
	}

	/** query & request **/
	String* HttpRequest::getGet(String* name){
		if(!name){
			return 0;
		}
		if(Url* url =getUrl()){
			return url->getQuery(name);
		}
		return 0;
	}
	String* HttpRequest::getRequest(String* name){
		if(!name){
			return 0;
		}
		if(String* val =getPost(name)){
			return val;
		}
		if(String* val =getGet(name)){
			return val;
		}
		return 0;
	}

	/** parse **/
	HttpRequest* HttpRequest::Parse(const char* str){
		if(!str){
			return 0;
		}
		//// head line
		// method
		const char* cursor =strchr(str, ' ');
		if(!cursor || cursor==str){
			WARN("invalid http request format, when parse method");
			return 0;
		}
		String* method =String::New(str, cursor-str);
		str =cursor+1;
		// path
		cursor =strchr(str, ' ');
		if(!cursor || cursor==str){
			WARN("invalid http request format, when parse path");
			return 0;
		}
		String* path =String::New(str, cursor-str);
		str =cursor+1;
		// version
		cursor =strstr(str, "\r\n");
		if(!cursor || cursor==str){
			WARN("invalid http request format, when parse version");
			return 0;
		}
		String* version =String::New(str, cursor-str);
		str =cursor+2;

		//// header
		int64_t content_len =0;
		Hash* header_tb =SafeNew<Hash>();
		Hash* cookie_tb =SafeNew<Hash>();
		while(*str){
			cursor =strstr(str, "\r\n");
			if(cursor == str){
				str =cursor+2;
				break;
			}
			else if(cursor == 0){
				WARN("invalid http request format, when parse header");
				return 0;
			}
			else{
				const char* header =str;
				const char* header_end =cursor;
				str =header_end + 2;
				cursor =strstr(header, ": ");
				if(cursor==0 || cursor==header || cursor>header_end){
					WARN("invalid http request format, when parse header name");
					return 0;
				}
				String* header_name =String::New(header, cursor-header);
				String* header_value =String::New(cursor+2, header_end-(cursor+2));
				if(header_name->is("Content-Length")){
					if(!FromString<int64_t>(header_value, content_len) || content_len<0){
						WARN("invalid http request format, when parse content length");
						return 0;
					}
					header_tb->set(header_name, header_value);
				}
				else if(header_name->is("Cookie")){
					if(HttpCookie* cookie =HttpCookie::Parse(header_value->c_str())){
						cookie_tb->set(header_name, cookie);
					}
					else{
						return 0;
					}
				}
				else{
					header_tb->set(header_name, header_value);
				}
			}
		}

		//// content
		Bytes* content =0;
		if(content_len > 0){
			content =SafeNew<Bytes>(str, content_len);
		}
		
		//// post process
		HttpRequest* request =SafeNew<HttpRequest>();
		// head line
		ASSIGN_POINTER(request->m_method, method);
		ASSIGN_POINTER(request->m_path, path);
		ASSIGN_POINTER(request->m_version, version);

		// header
		ASSIGN_POINTER(request->m_header_tb, header_tb);
		ASSIGN_POINTER(request->m_cookie_tb, cookie_tb);

		// content
		ASSIGN_POINTER(request->m_content, content);

		// mime
		if(!request->_parse_mime()){
			return 0;
		}
		return request;
	}
	bool HttpRequest::_parse_mime(){
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
