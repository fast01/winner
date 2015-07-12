/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HttpListener ***/
	DEFINE_CLASS_INFO(HttpListener)

	/** ctor & dtor **/
	HttpListener::HttpListener()
		: m_service_id(0){
		printf("listener %p\n", (void*)this);
	}
	HttpListener::~HttpListener(){
	}

	/** Object **/
	void HttpListener::init(){
		Super::init();
	}
	void HttpListener::finalize(){
		Super::finalize();
	}
	/** create conn **/
	TcpConnection* HttpListener::create_connection(){
		return SafeNew<HttpConnection>(GenConnectionId(), m_service_id);
	}
	/** extra param **/
	bool HttpListener::setExtraParam(String* param){
		if(!param || param->empty()){
			return true;
		}
		if(!FromString<int64_t>(param, m_service_id)){
			WARN("http listener set extra param failed, invalid param `%s`", param ? param->c_str() : "null");
			return false;
		}
		return true;
	}
	/** self **/
	int64_t HttpListener::getServiceId(){
		return m_service_id;
	}
	void HttpListener::setServiceId(const int64_t id){
		m_service_id =id;
	}
}
