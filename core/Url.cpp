/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Url ***/
	/** Script **/
	DEFINE_SUPPORT_SCRIPT(Url, "core::Url")

	/** ctor & dtor **/
	Url::Url()
		: m_url(0)
		, m_protocol(0)
		, m_auth(0)
		, m_host(0)
		, m_path(0)
		, m_query_string(0)
		, m_query(0)
		, m_fragment(0){
	}
	Url::~Url(){
	}

	/** Object **/
	void Url::init(){
		Super::init();
	}
	void Url::finalize(){
		clean();
		Super::finalize();
	}

	/** New **/
	Url* Url::New(){
		return SafeNew<Url>();
	}

	/** modify **/
	bool Url::parse(String* url){
		// clean
		clean();
		
		// check
		if(!url){
			return false;
		}

		// prepare sz
		const char* sz =url->c_str();
		// puts(sz);

		// protocol
		const char* cursor =strstr(sz, "://");
		int64_t pos =(cursor ? (cursor-sz) : -1);
		if(pos == 0){
			ERROR("fail to call %s, len(protocol) can't be 0");
			return false;
		}
		else if(pos > 0){
			ASSIGN_POINTER(m_protocol, String::New(sz, pos));
			sz =cursor + 3;
		}

		// auth
		cursor =strchr(sz, '@');
		pos =(cursor ? (cursor-sz) : -1);
		if(pos == 0){
			CLEAN_POINTER(m_protocol);
			ERROR("fail to call %s, len(auth) can't be 0");
			return false;
		}
		else if(pos > 0){
			ASSIGN_POINTER(m_auth, String::New(sz, pos));
			sz =cursor + 1;
		}

		// host
		const char* szbeg =sz;
		while(const char ch =*sz){
			if(ch=='/' || ch=='?' || ch=='#'){
				break;
			}
			++sz;
		}
		if(sz > szbeg){
			ASSIGN_POINTER(m_host, String::New(szbeg, sz-szbeg));
		}

		// path
		if(*sz == '/'){
			const char* szbeg =sz++;
			while(const char ch =*sz){
				if(ch=='?' || ch=='#'){
					break;
				}
				++sz;
			}
			ASSIGN_POINTER(m_path, String::New(szbeg, sz-szbeg));
		}

		// query
		if(*sz == '?'){
			const char* szbeg =++sz;
			while(const char ch =*sz){
				if(ch == '#'){
					break;
				}
				++sz;
			}
			if(sz > szbeg){
				ASSIGN_POINTER(m_query_string, String::New(szbeg, sz-szbeg));
				ASSIGN_POINTER(m_query, SafeNew<Hash>());
				if(!ParseQuery(m_query_string, m_query)){
					clean();
					return false;
				}
			}
			else{
				ASSIGN_POINTER(m_query_string, String::New(szbeg, sz-szbeg));
			}
		}

		// fragment
		if(*sz == '#'){
			ASSIGN_POINTER(m_fragment, String::New(sz+1));
		}

		// url
		ASSIGN_POINTER(m_url, url);

		return true;
	}
	bool Url::build(String* protocol, String* auth, String* host, String* path, Hash* query, String* fragment){
		return _build(protocol, auth, host, path, 0, query, fragment);
	}
	bool Url::build(String* protocol, String* auth, String* host, String* path, String* query_string, String* fragment){
		return _build(protocol, auth, host, path, query_string, 0, fragment);
	}
	void Url::clean(){
		CLEAN_POINTER(m_url);
		CLEAN_POINTER(m_protocol);
		CLEAN_POINTER(m_auth);
		CLEAN_POINTER(m_host);
		CLEAN_POINTER(m_path);
		CLEAN_POINTER(m_query_string);
		CLEAN_POINTER(m_query);
		CLEAN_POINTER(m_fragment);
	}

	/** getter **/
	String* Url::getUrl(){
		return m_url;
	}
	String* Url::getProtocol(){
		return m_protocol;
	}
	String* Url::getAuth(){
		return m_auth;
	}
	String* Url::getHost(){
		return m_host;
	}
	String* Url::getPath(){
		return m_path;
	}
	String* Url::getQueryString(){
		return m_query_string;
	}
	Hash* Url::getQuery(){
		return m_query;
	}
	String* Url::getFragment(){
		return m_fragment;
	}
	/** helper **/
	bool Url::ParseQuery(String* query_str, Hash* tb){
		// prepare
		BinaryCoder<1024> coder;
		coder.append(query_str);
		coder.appendNull();
		char* save_ptr =0;
		char* query_string =coder.c_str();

		// split
		const char* szitem=strtok_r(query_string, "&", &save_ptr);
		while(szitem){
			// split key value
			const char* cursor =strchr(szitem, '=');
			const int64_t pos =cursor ? cursor-szitem : -1;
			if(pos > 0){
				const char* szkey =szitem;
				const char* szvalue =szitem + pos + 1;
				if(strchr(szvalue, '=') != 0){
					return false;
				}
				String* key =UrlEncode::Decode(szkey, pos);
				String* value =UrlEncode::Decode(szvalue, -1);
				tb->set(key, value);
				// DEBUG("%s=%s", key->c_str(), value->c_str());
			}
			else if(pos<0 && *szitem){
				String* key =UrlEncode::Decode(szitem, -1);
				String* value =STR("true");
				tb->set(key, value);
				// DEBUG("%s=%s", key->c_str(), value->c_str());
			}

			// next key value item
			szitem =strtok_r(0, "&", &save_ptr);
		}
		return true;
	}
	bool Url::_build(String* protocol, String* auth, String* host, String* path, String* query_string, Hash* query, String* fragment){
		// clean
		clean();

		// check
		if(path && path->size() && !path->hasPrefix("/")){
			return false;
		}

		// make url
		BinaryCoder<1024> coder;

		if(protocol && protocol->size()){
			ASSIGN_POINTER(m_protocol, protocol);
			coder.append(protocol);
			coder.append("://", 3);
		}
		if(auth && auth->size()){
			ASSIGN_POINTER(m_auth, auth);
			coder.append(auth);
			coder.append("@", 1);
		}
		if(host && host->size()){
			ASSIGN_POINTER(m_host, host);
			coder.append(host);
		}
		if(path && path->size()){
			ASSIGN_POINTER(m_path, path);
			coder.append(path);
		}
		if(query_string && query_string->size()){
			ASSIGN_POINTER(m_query_string, query_string);
			ASSIGN_POINTER(m_query, SafeNew<Hash>());
			if(!ParseQuery(m_query_string, m_query)){
				clean();
				return false;
			}
			coder.append("?", 1);
			coder.append(m_query_string);
		}
		else if(query && query->size()){
			ASSIGN_POINTER(m_query, query);
			// make query string
			BinaryCoder<1024> sub_coder;
			HashIterator* it =static_cast< HashIterator* >(query->iterator());
			while(it->next()){
				String* key =static_cast< String* >(it->getKey());
				String* val =static_cast< String* >(it->getValue());
				if(sub_coder.size() > 0){
					sub_coder.append("&", 1);
				}
				sub_coder.append(UrlEncode::Encode(key));
				sub_coder.append("=", 1);
				sub_coder.append(UrlEncode::Encode(val));
			}
			// set
			ASSERT(sub_coder.size());
			ASSIGN_POINTER(m_query_string, String::New(sub_coder.c_str(), sub_coder.size()));
			coder.append("?", 1);
			coder.append(m_query_string);
		}
		if(fragment && fragment->size()){
			ASSIGN_POINTER(m_fragment, fragment);
			coder.append("#", 1);
			coder.append(fragment);
		}
		ASSIGN_POINTER(m_url, String::New(coder.c_str(), coder.size()));
		return true;
	}
}
