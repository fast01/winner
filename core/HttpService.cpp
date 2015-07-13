/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpService ***/
	DEFINE_CLASS_INFO(HttpService)

	/** ctor & dtor **/
	HttpService::HttpService()
		: m_html_template(0){
	}
	HttpService::~HttpService(){
	}

	/** Object **/
	void HttpService::init(){
		Super::init();
	}
	void HttpService::finalize(){
		CLEAN_POINTER(m_html_template);
		Super::finalize();
	}
	/** CoroutineService **/
	void HttpService::register_command(){
		on(SERVICE_HTTP_PROCESS_REQUEST, false, _process);
	}
	/** self **/
	void HttpService::on_request(HttpRequest* request, HttpRespond* respond){
	}
	String* HttpService::render_html(String* source, Hash* param){
		if(!m_html_template){
			HtmlTemplate* tmpl =SafeNew<HtmlTemplate>();
			if(tmpl->good()){
				ASSIGN_POINTER(m_html_template, tmpl);
			}
		}
		if(m_html_template){
			return m_html_template->render(source, param);
		}
		else{
			return 0;
		}
	}
	/** private **/
	void HttpService::_process(Object* arg){
		OPH();
		// prepare
		HttpService* self =dynamic_cast< HttpService* >(Service::Current());
		ASSERT(self);

		Command* cmd =static_cast< Command* >(arg);
		ASSERT(cmd);

		Bytes* body =cmd->getBody();
		if(!body){
			ERROR("http process request failed, body is null");
			return;
		}
		body->appendStringNull();

		// check
		Requestor* requestor =cmd->getRequestor();
		if(!requestor){
			ERROR("http process request failed, requestor is null");
			return;
		}

		// build HttpRequest & HttpRespond
		HttpRequest* req =HttpRequest::Parse(body->c_str());
		if(!req){
			ERROR("http process request failed, parse http `%s` failed", body->c_str());
			return;
		}
		HttpRespond* res =SafeNew<HttpRespond>(requestor);

		// call
		self->on_request(req, res);

		// flush
		if(res->isEnableAutoFlush()){
			res->flush();
		}
	}
}
