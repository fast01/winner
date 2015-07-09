/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "../service.h"

namespace service{
	/*** impl S3CoroutineService ***/
	BEGIN_CLASS_INFO(S3CoroutineService)
	END_CLASS_INFO

	/** ctor & dtor **/
	S3CoroutineService::S3CoroutineService(){
		m_id =SERVICE_ID;
		m_protocol_group_id =::protocol::ID;
	}
	S3CoroutineService::~S3CoroutineService(){}

	/** New **/
	Service* S3CoroutineService::New(){
		return SafeNew<S3CoroutineService>();
	}

	/** Service **/
	bool S3CoroutineService::on_load(){
		if(!Super::on_load()) return false;
		ASSIGN_POINTER(m_name, STR("S3CoroutineService"));
		return true;
	}
	void S3CoroutineService::on_unload(){
		Super::on_unload();
		DEBUG("unload S3CoroutineService");
	}
	void S3CoroutineService::update(const int64_t now){
		Super::update(now);
		static thread_local bool done =false;
		if(done) return;
		done =true;
		sleep_sec(3); // wait for service manager load already
		if(0){
			// rpc
			::protocol::S4FirstRequest* rpc_param =SafeNew<::protocol::S4FirstRequest>();
			rpc_param->setParam1(3);
			rpc_param->setParam2(false);
			rpc_param->setParam3(STR("s3"));

			auto rpc_respond =static_cast< Command* >(rpc(0, SERVICE_ID_S4_LUA, ::protocol::PROTOCOL_S4_FIRST_REQUEST, ::protocol::ID, rpc_param));
			ASSERT(rpc_respond);
			auto rpc_result =static_cast< ::protocol::S4FirstRespond* >(rpc_respond->getRequest());
			ASSERT(rpc_result);
			ASSERT(rpc_result->getResult1() == 40);
			ASSERT(rpc_result->getResult2() == true);
			ASSERT(rpc_result->getResult3()->is("from s4"));

			DEBUG("S3 rpc S4 respond");
		}
		if(0){
			test_parallel();
		}
		DEBUG("after update");
	}
	void S3CoroutineService::register_command(){
		on(::protocol::PROTOCOL_S3_FIRST_REQUEST, SafeNew< ::core::CoroutineCommandDesc >(::protocol::PROTOCOL_S3_FIRST_RESPOND, on_first_request));
		on(::protocol::PROTOCOL_S3_SECOND_REQUEST, SafeNew< ::core::CoroutineCommandDesc >(::protocol::PROTOCOL_S3_SECOND_RESPOND, on_second_request));
	}
	/*** first request ***/
	void S3CoroutineService::on_first_request(Object* param){
		// prepare
		S3CoroutineService* self =dynamic_cast< S3CoroutineService* >(Current());
		Command* command =static_cast< Command* >(param);
		auto request =dynamic_cast< ::protocol::S3FirstRequest* >(command->getRequest());
		auto respond =dynamic_cast< ::protocol::S3FirstRespond* >(command->getRespond());
		ASSERT(request && respond);
		ASSERT(request->getParam1() == 2);
		ASSERT(request->getParam2() == false);
		ASSERT(request->getParam3()->is("s2"));

		// rpc
		::protocol::S4FirstRequest* rpc_param =SafeNew<::protocol::S4FirstRequest>();
		rpc_param->setParam1(3);
		rpc_param->setParam2(false);
		rpc_param->setParam3(STR("s3"));

		auto rpc_respond =static_cast< Command* >(self->rpc(command->getWho(), SERVICE_ID_S4_LUA, ::protocol::PROTOCOL_S4_FIRST_REQUEST, ::protocol::ID, rpc_param));
		ASSERT(rpc_respond);
		auto rpc_result =static_cast< ::protocol::S4FirstRespond* >(rpc_respond->getRequest());
		ASSERT(rpc_result);
		ASSERT(rpc_result->getResult1() == 40);
		ASSERT(rpc_result->getResult2() == true);
		ASSERT(rpc_result->getResult3()->is("from s4"));

		// process
		respond->setResult1(30);
		respond->setResult2(true);
		respond->setResult3(STR("from s3"));

		// respond
		Bytes* bs =respond->toBytes(0);
		command->reply(command->getRespondCommand(), bs->data(), bs->size());
	}
	/*** second request ***/
	void S3CoroutineService::on_second_request(Object* param){
		// prepare
		S3CoroutineService* self =dynamic_cast< S3CoroutineService* >(Current());
		Command* command =static_cast< Command* >(param);
		auto request =static_cast< ::protocol::S3FirstRequest* >(command->getRequest());
		auto respond =static_cast< ::protocol::S3FirstRespond* >(command->getRespond());
		ASSERT(request && respond);
		ASSERT(request->getParam1() == 2);
		ASSERT(request->getParam2() == false);
		ASSERT(request->getParam3()->is("s2"));

		// rpc
		::protocol::S4FirstRequest* rpc_param =SafeNew<::protocol::S4FirstRequest>();
		rpc_param->setParam1(3);
		rpc_param->setParam2(false);
		rpc_param->setParam3(STR("s3"));

		auto rpc_respond =static_cast< Command* >(self->rpc(0, SERVICE_ID_S4_LUA, ::protocol::PROTOCOL_S4_FIRST_REQUEST, ::protocol::ID, rpc_param));
		ASSERT(rpc_respond);
		auto rpc_result =static_cast< ::protocol::S4FirstRespond* >(rpc_respond->getRequest());
		ASSERT(rpc_result);
		ASSERT(rpc_result->getResult1() == 40);
		ASSERT(rpc_result->getResult2() == true);
		ASSERT(rpc_result->getResult3()->is("from s4"));

		// process
		respond->setResult1(30);
		respond->setResult2(true);
		respond->setResult3(STR("from s3"));

		// respond
		command->reply();
	}
	/*** test parallel ***/
	void S3CoroutineService::test_parallel(){
		Parallel* parallel =SafeNew<Parallel>();
		parallel->addTask(_parallel_task1);
		parallel->addTask(_parallel_task2);
		parallel->addTask(_parallel_task3);
		
		Int64* val =SafeNew<Int64>();
		const bool ok =parallel->run(val);
		DEBUG("parallel %s:remain count %lld, error count %lld, value %lld", ok?"success":"fail"
				, parallel->getRemainCount(), parallel->getErrorCount(), val->getValue());
	}
	bool S3CoroutineService::_parallel_task1(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 1);
		DEBUG(__func__);
		if(1){
			// rpc
			S3CoroutineService* self =dynamic_cast< S3CoroutineService* >(Service::Current());
			ASSERT(self);
			::protocol::S4FirstRequest* rpc_param =SafeNew<::protocol::S4FirstRequest>();
			rpc_param->setParam1(3);
			rpc_param->setParam2(false);
			rpc_param->setParam3(STR("s3"));

			auto rpc_respond =static_cast< Command* >(self->rpc(0, SERVICE_ID_S4_LUA, ::protocol::PROTOCOL_S4_FIRST_REQUEST, ::protocol::ID, rpc_param));
			ASSERT(rpc_respond);
			auto rpc_result =static_cast< ::protocol::S4FirstRespond* >(rpc_respond->getRequest());
			ASSERT(rpc_result);
			ASSERT(rpc_result->getResult1() == 40);
			ASSERT(rpc_result->getResult2() == true);
			ASSERT(rpc_result->getResult3()->is("from s4"));

			DEBUG("in test parallel:S3 rpc S4 respond");

			// sub parallel
			sleep_sec(2);
			self->test_parallel_sub();
		}
		return true;
	}
	bool S3CoroutineService::_parallel_task2(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 10);
		DEBUG(__func__);
		return true;
	}
	bool S3CoroutineService::_parallel_task3(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 100);
		DEBUG(__func__);
		return true;
	}
	/*** test parallel ***/
	void S3CoroutineService::test_parallel_sub(){
		Parallel* parallel =SafeNew<Parallel>();
		parallel->addTask(_parallel_task_sub_1);
		parallel->addTask(_parallel_task_sub_2);
		parallel->addTask(_parallel_task_sub_3);
		
		Int64* val =SafeNew<Int64>();
		const bool ok =parallel->run(val);
		DEBUG("parallel %s:remain count %lld, error count %lld, value %lld", ok?"success":"fail"
				, parallel->getRemainCount(), parallel->getErrorCount(), val->getValue());
	}
	bool S3CoroutineService::_parallel_task_sub_1(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 2);
		DEBUG(__func__);

		S3CoroutineService* self =dynamic_cast< S3CoroutineService* >(Service::Current());
		ASSERT(self);
		// self->sleep(2);
		if(1){
			// rpc
			::protocol::S4FirstRequest* rpc_param =SafeNew<::protocol::S4FirstRequest>();
			rpc_param->setParam1(3);
			rpc_param->setParam2(false);
			rpc_param->setParam3(STR("s3"));

			auto rpc_respond =static_cast< Command* >(self->rpc(0, SERVICE_ID_S4_LUA, ::protocol::PROTOCOL_S4_FIRST_REQUEST, ::protocol::ID, rpc_param));
			ASSERT(rpc_respond);
			auto rpc_result =static_cast< ::protocol::S4FirstRespond* >(rpc_respond->getRequest());
			ASSERT(rpc_result);
			ASSERT(rpc_result->getResult1() == 40);
			ASSERT(rpc_result->getResult2() == true);
			ASSERT(rpc_result->getResult3()->is("from s4"));

			DEBUG("in test parallel sub:S3 rpc S4 respond");
		}
		return true;
	}
	bool S3CoroutineService::_parallel_task_sub_2(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 20);
		DEBUG(__func__);
		return true;
	}
	bool S3CoroutineService::_parallel_task_sub_3(Object* arg){
		Int64* val =dynamic_cast< Int64* >(arg);
		ASSERT(val);
		val->setValue(val->getValue() + 200);
		DEBUG(__func__);
		return true;
	}
}
