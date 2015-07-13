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
		return;

		static bool done =false;
		if(done) return;
		done =true;
		HttpRespond* res =HttpClient::Get(STR("http://www.sina.com/"));
		ASSERT(res && res->getContent());
		printf("%d\n", (int)res->getContent()->size());
		// printf("%*s\n", (int)res->getContent()->size(), res->getContent()->c_str());

		res =HttpClient::Get(STR("http://www.sina.com/"));
		ASSERT(res && res->getContent());
		printf("%s\n", res->getContent()->c_str());
	}
	void S6HttpService::on_request(HttpRequest* request, HttpRespond* respond){
		Hash* param =request->getUrl()->getQueryTable();
		String* path =ApplicationBase::Instance()->getConfigPath("html/view/index.lua");
		String* content =render_html(read_string(path->c_str()), param);
		DEBUG(content->c_str());
		respond->write(content);
	}
}
