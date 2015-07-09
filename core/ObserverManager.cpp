/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** singleton ***/
	DEFINE_THREAD_LOCAL_SINGLETON(ObserverManager)

	/** impl ObserverManager  **/
	ObserverManager::ObserverManager()
		: m_observer_table(0){
	}
	ObserverManager::~ObserverManager(){
	}

	/** Object **/
	void ObserverManager::init(){
		Super::init();
		m_observer_table =New< Hash >();
		m_observer_table->retain();
	}
	void ObserverManager::finalize(){
		CLEAN_POINTER(m_observer_table);
		Super::finalize();
	}

	/** observer **/
	Observer* ObserverManager::get(String* observer_name){
		if(Observer* observer =static_cast< Observer* >(m_observer_table->get(observer_name))){
			return observer;
		}
		if(Observer* observer =static_cast< Observer* >(ObjectFactory::Create(observer_name))){
			m_observer_table->set(observer_name, observer);
			return observer;
		}
		ERROR("observer manager get observer %s failed", observer_name ? observer_name->c_str() : "null");
		return 0;
	}

	/** notify **/
	void ObserverManager::notify(String* observer_name, Actor* actor, Event* evt){
		if(Observer* observer =get(observer_name)){
			observer->notify(actor, evt);
		}
	}
	void ObserverManager::Notify(String* observer_name, Actor* actor, Event* evt){
		if(ObserverManager* mgr =ObserverManager::Instance()){
			mgr->notify(observer_name, actor, evt);
		}
	}
}
