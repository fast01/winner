/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Component impl ***/
	/** ctor & dtor**/
	Component::Component()
		: m_actor(0){
	}
	Component::~Component(){}

	/** self **/
	void Component::setActor(Actor* actor){
		m_actor =actor; // weak ptr
	}
	Actor* Component::getActor(){
		return m_actor;
	}
}
