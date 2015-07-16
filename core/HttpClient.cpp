/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpClient ***/
	/** ctor & dtor **/
	HttpClient::HttpClient()
		: TcpConnection(0)
		, m_header_length(0)
		, m_content_length(0)
		, m_packet_length(0)
		, m_sign(0)
		, m_coroutine(0){
		setHeartBeatTimer(HTTP_CONNECTION_KEEP_ALIVE_TIME);
	}
	HttpClient::~HttpClient(){
	}

	/** Object **/
	void HttpClient::init(){
		Super::init();
	}
	void HttpClient::finalize(){
		CLEAN_POINTER(m_coroutine);
		Super::finalize();
	}
	/** MonitorTarget **/
	bool HttpClient::reborn(){
		return false;
	}
	bool HttpClient::canReborn(){
		return false;
	}
	void HttpClient::onDetachEvent(){
		Super::onDetachEvent();
		_resume_coroutine(0);
	}
	/** TcpConnection **/
	int64_t HttpClient::on_auth(char* data, const int64_t s){
		setAuthed();
		return on_recv(data, s);
	}
	int64_t HttpClient::on_recv(char* data, const int64_t s){
		// build coder
		BinaryCoder<192> coder;
		coder.append(data, s);
		coder.appendNull();

		// calc http request package length
		if(m_packet_length == 0){
			char* sz =coder.c_str();
			const char* s1 =strstr(sz, "\r\nContent-Length: ");
			const char* s2 =strstr(sz, "\r\n\r\n");
			if(s2){
				if(s1 && s1<s2){
					long long len =0;
					if(1 != sscanf(s1+18, "%lld", &len)){
						return -1;
					}
					m_content_length =len;
				}
				else{
					m_content_length =0;
				}
				m_header_length =s2+4 - sz;
				m_packet_length =m_header_length + m_content_length;
			}
		}

		// resume coroutine
		if(s >= m_packet_length){
			HttpRespond* res =HttpRespond::Parse(coder.c_str());
			_resume_coroutine(res);

			// clean
			const int64_t ret =m_packet_length;
			m_header_length =0;
			m_content_length =0;
			m_packet_length =0;
			return ret;
		}
		return 0;
	}
	/** interface **/
	HttpRespond* HttpClient::Get(String* szurl){
		HttpClient* client =SafeNew<HttpClient>();
		return client->_get(szurl);
	}
	HttpRespond* HttpClient::Post(String* szurl, Hash* header, Hash* param){
		HttpClient* client =SafeNew<HttpClient>();
		return client->_post(szurl, header, param);
	}
	HttpRespond* HttpClient::Request(String* szmethod, String* szurl, String* szversion, Hash* header, Bytes* content){
		HttpClient* client =SafeNew<HttpClient>();
		return client->_request(szmethod, szurl, szversion, header, content);
	}

	/** request **/
	HttpRespond* HttpClient::_get(String* szurl){
		return _request(STR("GET"), szurl, 0, 0, 0);
	}
	HttpRespond* HttpClient::_post(String* szurl, Hash* header, Hash* param){
		// add header
		if(!header){
			header =SafeNew<Hash>();
		}
		header->set("Content-Type", STR("application/x-www-form-urlencoded"));

		// set content
		Bytes* content =0;
		if(param){
			content =SafeNew<Bytes>();
			HashIterator* it =static_cast< HashIterator* >(param->iterator());
			while(it->next()){
				String* name =dynamic_cast< String* >(it->getKey());
				String* value =dynamic_cast< String* >(it->getValue());
				if(name && value){
					name =UrlEncode::Encode(name);
					value =UrlEncode::Encode(value);
				}
				if(name && value){
					if(content->size() > 0){
						content->appendString("&");
					}
					content->appendString(name);
					content->appendString("=");
					content->appendString(value);
				}
			}
		}
		
		// request
		return _request(STR("POST"), szurl, 0, header, content);
	}
	HttpRespond* HttpClient::_request(String* szmethod, String* szurl, String* szversion, Hash* header, Bytes* content){
		// check
		if(!szmethod || szmethod->empty()){
			ERROR("HttpClient request failed, szmethod `%s` invalid", szmethod ? szmethod->c_str() : "null");
			return 0;
		}
		// parse url
		Url* url =SafeNew<Url>();
		if(!url->parse(szurl)){
			ERROR("HttpClient request failed, szurl `%s` invalid", szurl ? szurl->c_str() : "null");
			return 0;
		}
		String* protocol =url->getProtocol();
		String* host =url->getHost();
		String* path =url->getPath();
		String* query_string =url->getQueryString();
		if(protocol && (protocol->is("http")==false)){
			ERROR("HttpClient request failed, szurl `%s` invalid, protocol is not http", szurl->c_str());
			return 0;
		}
		if(!host || host->empty()){
			ERROR("HttpClient request failed, szurl `%s` invalid, missing host", szurl->c_str());
			return 0;
		}
		if(path && !path->hasPrefix("/")){
			ERROR("HttpClient request failed, szurl `%s` invalid, invalid path", szurl->c_str());
			return 0;
		}
		String* full_path =0;
		if(path){
			full_path =path;
		}
		else{
			full_path =STR("/");
		}
		if(query_string){
			full_path =full_path->append("?");
			full_path =full_path->append(query_string);
		}

		// parse content
		int64_t content_len =0;
		if(content){
			content_len =content->size();
		}
		
		//// build
		BinaryCoder<4096> coder;
		
		/// head line
		// method
		coder.append(szmethod);
		coder.append(" ");
		// path
		coder.append(full_path);
		coder.append(" ");
		// version
		if(szversion){
			coder.append(szversion);
		}
		else{
			coder.append("HTTP/1.1");
		}
		coder.append("\r\n");

		/// header
		if(content_len){
			if(!header || !header->has("Content-Type")){
				coder.append("Content-Type: text/html\r\n");
			}
			if(!header || !header->has("Content-Length")){
				coder.append(String::Format("Content-Length: %lld\r\n", (long long)content_len));
			}
		}
		if(!header || !header->has("Accept")){
			coder.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
		}
		// if(!header || !header->has("Accept-Encoding")){
			// coder.append("Accept-Encoding: gzip, deflate\r\n");
		// }
		if(!header || !header->has("Accept-Language")){
			coder.append("Accept-Language: en-US,en;q=0.5\r\n");
		}
		if(!header || !header->has("Cache-Control")){
			coder.append("Cache-Control: max-age=0\r\n");
		}
		if(!header || !header->has("Connection")){
			coder.append("Connection: keep-alive\r\n");
		}
		if(!header || !header->has("User-Agent")){
			coder.append("User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0\r\n");
		}
		if(!header || !header->has("Host")){
			coder.append(String::Format("Host: %s\r\n", host->c_str()));
		}
		if(header){
			HashIterator* it =static_cast< HashIterator* >(header->iterator());
			while(it->next()){
				if(String* name =dynamic_cast< String* >(it->getKey())){
					if(String* value =dynamic_cast< String* >(it->getValue())){
						coder.append(name);
						coder.append(": ");
						coder.append(value);
						coder.append("\r\n");
					}
					else if(HttpCookie* cookie =dynamic_cast< HttpCookie* >(it->getValue())){
						if(String* cookie_value =cookie->getValue()){
							coder.append(String::Format("Cookie: %s=%s\r\n", name->c_str(), cookie_value->c_str()));
						}
					}
				}
			}
		}
		coder.append("\r\n");

		/// content
		if(content_len > 0){
			coder.append(content->c_str(), content_len);
		}

		//// connect
		char szip[32] ={0};
		int32_t port =0;
		if(!ParseIpPort(host, szip, port)){
			ERROR("HttpClient request failed, parse ip:port error");
			return 0;
		}
		if(!connect(szip, port)){
			ERROR("HttpClient request failed, connect error");
			return 0;
		}

		//// monitor
		Monitor* monitor =Monitor::Instance();
		ASSERT(monitor);
		if(!monitor->monitor(this)){
			ERROR("HttpClient request failed, monitor error");
			return 0;
		}

		//// send
		const bool ok =send(coder.c_str(), coder.size());
		if(ok){
			if(HttpRespond* res =_yield_coroutine()){
				return res;
			}
			else{
				ERROR("HttpClient request failed, yield error");
				return 0;
			}
		}
		else{
			ERROR("HttpClient request failed, send error");
			monitor->demonitor(this);
			return 0;
		}
	}

	/** yield & resume **/
	HttpRespond* HttpClient::_yield_coroutine(){
		// check env
		Coroutine* cr =Coroutine::Running();
		if(!cr){
			WARN("http client yield failed, not in coroutine.");
			return false;
		}
		ASSERT(cr->canYield());

		// set coroutine
		ASSIGN_POINTER(m_coroutine, cr);

		// yield
		m_sign =CoroutineService::GenRpcId();
		ENSURE(cr->yield(0, m_sign));

		// respond
		HttpRespond* res =dynamic_cast< HttpRespond* >(cr->getResumeParam());
		CLEAN_POINTER(m_coroutine);
		return res;
	}
	void HttpClient::_resume_coroutine(HttpRespond* res){
		if(m_coroutine && m_coroutine->isWaiting()){
			CoroutineService* service =dynamic_cast< CoroutineService* >(Service::Current());
			ASSERT(service);
			CoroutinePool* crp =service->getCoroutinePool();
			ASSERT(crp);
			crp->resume(m_coroutine->getId(), res, m_sign);
			CLEAN_POINTER(m_coroutine);
		}
	}
}
