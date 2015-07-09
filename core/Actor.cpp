/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Actor impl ***/
	/** ctor & dtor **/
	Actor::Actor()
		: m_observer_manager(0)
		, m_component_manager(0){
	}
	Actor::~Actor(){
	}

	/** Object **/
	void Actor::init(){
		Super::init();
		ASSIGN_POINTER(m_observer_manager, SafeNew<Hash>());
		ASSIGN_POINTER(m_component_manager, SafeNew<Hash>());
	}
	void Actor::finalize(){
		CLEAN_POINTER(m_observer_manager);
		CLEAN_POINTER(m_component_manager);
		Super::finalize();
	}

	/** self **/
	/* observer */
	bool Actor::addObserver(Observer* observer){
		if(!observer) return false;
		if(m_observer_manager->has(observer->getClassName())){
			return false;
		}
		m_observer_manager->set(observer->getClassName(), observer);
		return true;
	}
	Observer* Actor::getObserver(String* observer_class_name){
		return static_cast< Observer* >(m_observer_manager->get(observer_class_name));
	}
	Array* Actor::getAllObserver(){
		Array* ret =SafeNew<Array>();
		HashIterator* it =static_cast< HashIterator* >(m_observer_manager->iterator());
		while(it->next()){
			ret->push_back(it->getValue());
		}
		return ret;
	}

	/* component */
	bool Actor::addComponent(Component* com){
		if(!com) return false;
		if(m_component_manager->has(com->getClassName())){
			return false;
		}
		m_component_manager->set(com->getClassName(), com);
		return true;
	}
	Component* Actor::getComponent(String* component_class_name){
		return static_cast< Component* >(m_component_manager->get(component_class_name));
	}
	Array* Actor::getAllComponent(){
		Array* ret =SafeNew<Array>();
		HashIterator* it =static_cast< HashIterator* >(m_component_manager->iterator());
		while(it->next()){
			ret->push_back(it->getValue());
		}
		return ret;
	}
}
