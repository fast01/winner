/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "../service.h"
		
namespace service{
	/*** impl S2CallbackService ***/
	BEGIN_CLASS_INFO(S2CallbackService)
		FIELD_INFO(&S2CallbackService::m_version, "version", "")
	END_CLASS_INFO

	/** ctor & dtor **/
	S2CallbackService::S2CallbackService(){
		m_id =SERVICE_ID;
		m_protocol_group_id =::protocol::ID;
	}
	S2CallbackService::~S2CallbackService(){}

	/** New **/
	Service* S2CallbackService::New(){
		return SafeNew<S2CallbackService>();
	}

	/** Service **/
	bool S2CallbackService::on_load(){
		if(!Super::on_load()) return false;
		ASSIGN_POINTER(m_name, STR("S2CallbackService"));
		return true;
	}
	void S2CallbackService::on_unload(){
		Super::on_unload();
		DEBUG("unload S2CallbackService");
	}
	void S2CallbackService::on_update(const int64_t now){
		Super::on_update(now);
		static thread_local bool done =false;
		if(done){
			return;	
		}
		done =true;
		// rpc
		if(0){ // change to `if(1){` for testing rpc in update
			sleep_sec(3); // wait for service manager load already
			::protocol::S3FirstRequest* rpc_param =SafeNew<::protocol::S3FirstRequest>();
			rpc_param->setParam1(2);
			rpc_param->setParam2(false);
			rpc_param->setParam3(STR("s2"));
			auto ok =rpc(0, SERVICE_ID_S3_COROUTINE, ::protocol::PROTOCOL_S3_FIRST_REQUEST, rpc_param, 
				SafeNew<CallbackRpcInfo>(::protocol::ID, (Object*)0, _on_first_rpc_respond)
			);
			ASSERT(ok);
		}
	}
	void S2CallbackService::register_command(){
		on(::protocol::PROTOCOL_S2_FIRST_REQUEST, SafeNew< ::core::CallbackCommandDesc >(::protocol::PROTOCOL_S2_FIRST_RESPOND, on_first_request));
		on(::protocol::PROTOCOL_S2_SECOND_REQUEST, SafeNew< ::core::CallbackCommandDesc >(::protocol::PROTOCOL_S2_SECOND_RESPOND, on_second_request));
	}

	/*** first request ***/
	IMPL_COMMAND(S2CallbackService, on_first_request, ::protocol::S2FirstRequest, ::protocol::S2FirstRespond)
		ASSERT(request->getParam1() == 1);
		ASSERT(request->getParam2() == false);
		ASSERT(request->getParam3()->is("s1"));

		// rpc
		::protocol::S3FirstRequest* rpc_param =SafeNew<::protocol::S3FirstRequest>();
		rpc_param->setParam1(2);
		rpc_param->setParam2(false);
		rpc_param->setParam3(STR("s2"));
		auto ok =self->rpc(command->getWho(), SERVICE_ID_S3_COROUTINE, ::protocol::PROTOCOL_S3_FIRST_REQUEST, rpc_param, 
			SafeNew<CallbackRpcInfo>(::protocol::ID, command, _on_first_rpc_respond)
		);
		ASSERT(ok);
		return 1;
	END_COMMAND

	int64_t S2CallbackService::_on_first_rpc_respond(Object* param, Object* context){
		// prepare
		auto rpc_respond =static_cast< Command* >(param);
		auto rpc_result  =static_cast< ::protocol::S3FirstRespond* >(rpc_respond->getRequest());
		ASSERT(rpc_result->getResult1() == 30);
		ASSERT(rpc_result->getResult2() == true);
		ASSERT(rpc_result->getResult3()->is("from s3"));

		if(auto req_command =static_cast< Command* >(context)){
			auto request =dynamic_cast< ::protocol::S2FirstRequest* >(req_command->getRequest());
			auto respond =dynamic_cast< ::protocol::S2FirstRespond* >(req_command->getRespond());
			ASSERT(request && respond && rpc_result);

			// process
			respond->setResult1(20);
			respond->setResult2(true);
			respond->setResult3(STR("from s2"));

			// respond
			Bytes* bs =respond->toBytes(0);
			req_command->reply(req_command->getRespondCommand(), bs->data(), bs->size());
			return 0;
		}
		else{
			DEBUG("S2 RPC S3 respond");
			return 0;
		}
	}
	/*** second request ***/
	int64_t S2CallbackService::on_second_request(Command* command){
		// prepare
		S2CallbackService* self =dynamic_cast< S2CallbackService* >(command->getService());
		UNUSED(self);
		auto request =static_cast< ::protocol::S2SecondRequest* >(command->getRequest());
		auto respond =static_cast< ::protocol::S2SecondRespond* >(command->getRespond());
		ASSERT(request && respond);

		ASSERT(request->getParam1() == 1);
		ASSERT(request->getParam2() == false);
		ASSERT(request->getParam3()->is("s1"));

		// rpc group
		::protocol::S3SecondRequest* rpc_param =SafeNew<::protocol::S3SecondRequest>();
		rpc_param->setParam1(2);
		rpc_param->setParam2(false);
		rpc_param->setParam3(STR("s2"));

		CallbackGroupRpcInfo* grp =SafeNew<CallbackGroupRpcInfo>(command, _on_second_rpc_respond);
		auto ok =grp->begin();
		ASSERT(ok);
		grp->rpc(0, SERVICE_ID_S3_COROUTINE, ::protocol::PROTOCOL_S3_SECOND_REQUEST, rpc_param, SafeNew<CallbackRpcInfo>(::protocol::ID));
		grp->end();

		return 1;
	}

	int64_t S2CallbackService::_on_second_rpc_respond(CallbackGroupRpcInfo* grp){
		// check group
		ASSERT(grp->getSuccessCount()==1 && grp->getErrorCount()==0 && grp->getCount()==1);

		// paepare
		auto rpc_respond =static_cast< Command* >(grp->getValue(0));
		auto req_command =static_cast< Command* >(grp->getContext());
		auto request =dynamic_cast< ::protocol::S2SecondRequest* >(req_command->getRequest());
		auto respond =dynamic_cast< ::protocol::S2SecondRespond* >(req_command->getRespond());
		auto rpc_result =static_cast< ::protocol::S3SecondRespond* >(rpc_respond->getRequest());
		ASSERT(request && respond && rpc_result);
		ASSERT(rpc_result->getResult1() == 30);
		ASSERT(rpc_result->getResult2() == true);
		ASSERT(rpc_result->getResult3()->is("from s3"));

		// process
		respond->setResult1(20);
		respond->setResult2(true);
		respond->setResult3(STR("from s2"));

		// respond
		req_command->reply();
		return 0;
	}
}
