/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "../service.h"

namespace service{
	/*** impl S6HttpService ***/
	BEGIN_CLASS_INFO(S6HttpService)
	END_CLASS_INFO

	/** ctor & dtor **/
	S6HttpService::S6HttpService(){
		m_id =SERVICE_ID;
		m_protocol_group_id =::protocol::ID;
	}
	S6HttpService::~S6HttpService(){}

	/** New **/
	Service* S6HttpService::New(){
		return SafeNew<S6HttpService>();
	}

	/** Service **/
	bool S6HttpService::on_load(){
		if(!Super::on_load()) return false;
		ASSIGN_POINTER(m_name, STR("S6HttpService"));
		return true;
	}
	void S6HttpService::on_unload(){
		Super::on_unload();
		DEBUG("unload S6HttpService");
	}
	void S6HttpService::update(const int64_t now){
		Super::update(now);
		if(ApplicationBase::Instance()->getDataPath()->indexOf("client") >= 0){
			static bool done =false;
			if(done) return;
			done =true;

			// get
			HttpRespond* res =HttpClient::Get(STR("127.0.0.1:19871/?name=fool&age=90"));
			if(res && res->getContent()){
				res->getContent()->appendStringNull();
				DEBUG("%s\n", res->getContent()->c_str());
			}
			else{
				DEBUG("fail to get from 127.0.0.1:19871");
			}

			// post
			Hash* param =SafeNew<Hash>();
			param->set("name", STR("fool"));
			param->set("age", STR("99"));
			res =HttpClient::Post(STR("127.0.0.1:19871"), 0, param);
			if(res && res->getContent()){
				res->getContent()->appendStringNull();
				DEBUG("%s\n", res->getContent()->c_str());
			}
			else{
				DEBUG("fail to post to 127.0.0.1:19871");
			}
		}
	}
	void S6HttpService::on_request(HttpRequest* request, HttpRespond* respond){
		Hash* param =0;
		if(request->getPostTable() && request->getPostTable()->size()>0){
			param =request->getPostTable();
		}
		else if(request->getUrl()){
			param =request->getUrl()->getQueryTable();
		}
		// printf("%p\n", param);
		String* path =ApplicationBase::Instance()->getConfigPath("html/view/index.lua");
		String* content =render_html(read_string(path->c_str()), param);
		DEBUG(content->c_str());
		respond->write(content);
	}
}
