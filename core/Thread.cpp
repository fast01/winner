/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Thread ***/
	/** ctor & dtor **/
	Thread::Thread()
		: m_state(STATE_INIT)
		, m_param(0){
	}
	Thread::~Thread(){
	}

	/** Object **/
	void Thread::init(){
		Super::init();
	}
	void Thread::finalize(){
		CLEAN_POINTER(m_param);
		Super::finalize();
	}
	/** Thread **/
	bool Thread::startup(PFN_CALLBACK pfn, Object* param){
		if(m_state != STATE_INIT){
			ERROR("thread start failed, state is not STATE_INIT");
			return false;
		}
		if(!pfn){
			ERROR("thread start failed, pfn is null");
			return false;
		}
		try{
			m_pfn =pfn;
			ASSIGN_POINTER(m_param, param);
			m_thread =std::thread(_thread_entry, this);
			return true;
		}
		catch(...){
			FATAL("std::thread error");
			return false;
		}
	}
	void Thread::join(){
		m_thread.join();
		ASSERT(m_state == STATE_DEAD);
	}
	/** thread **/
	void Thread::_thread_entry(Thread* self){
		self->memoryOrderSync();
		ObjectPool* object_pool =New<ObjectPool>();
		ObjectPool::Set(object_pool);

		{
			// init
			OPH();
			Logger::Init();
			Monitor::Init();
			ObserverManager::Init();
			ActorManager::Init();

			// run
			self->m_state =STATE_RUNNING;
			self->m_pfn(self->m_param);
			self->m_pfn =0;
			CLEAN_POINTER(self->m_param);
			self->m_state =STATE_DEAD;

			// release
			ActorManager::Release();
			ObserverManager::Release();
			Monitor::Release();
			Logger::Release();
		}
		ObjectPool::Release();
	}
}
