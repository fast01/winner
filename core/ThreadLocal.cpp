/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	///*** ThreadLocal Impl ***///
	//** ctor & dtor **//
	ThreadLocal::ThreadLocal()
		: m_logger(0)
		, m_monitor(0)
		, m_observer_manager(0)
		, m_actor_manager(0){
		mysql_thread_init();
	}
	ThreadLocal::~ThreadLocal(){
		mysql_thread_end();
	}

	/** Object **/
	void ThreadLocal::init(){
		OPH();
		Super::init();

		m_logger =New<Logger>();
		RETAIN_POINTER(m_logger);

		m_monitor =New<Monitor>();
		RETAIN_POINTER(m_monitor);

		m_observer_manager =New<ObserverManager>();
		RETAIN_POINTER(m_observer_manager);

		m_actor_manager =New<ActorManager>();
		RETAIN_POINTER(m_actor_manager);

		OutputDebug("thread local 0x%p init [ ok ]", this);
	}
	void ThreadLocal::finalize(){
		OPH();
		CLEAN_POINTER(m_actor_manager);
		CLEAN_POINTER(m_observer_manager);
		CLEAN_POINTER(m_monitor);
		CLEAN_POINTER(m_logger);

		OutputDebug("thread local 0x%p clean [ ok ]", this);
		Super::finalize();
	}

	/** update **/
	bool ThreadLocal::update(){
		const int64_t now =DateTime::Now();
		m_actor_manager->update(now);
		return true;
	}

	/** Instance **/
	thread_local static ThreadLocal* g_thread_local =0;
	void ThreadLocal::Init(){
		ASSERT(g_thread_local == 0);
		g_thread_local =New<ThreadLocal>();
		RETAIN_POINTER(g_thread_local);
	}
	ThreadLocal* ThreadLocal::Instance(){
		return g_thread_local;
	}
	void ThreadLocal::Release(){
		ASSERT(g_thread_local->getRefCount() == 1);
		CLEAN_POINTER(g_thread_local);
	}
	/** subobj **/
	Logger* ThreadLocal::getLogger(){
		return m_logger;
	}
	Monitor* ThreadLocal::getMonitor(){
		return m_monitor;
	}
	ObserverManager* ThreadLocal::getObserverManager(){
		return m_observer_manager;
	}
	ActorManager* ThreadLocal::getActorManager(){
		return m_actor_manager;
	}
}
