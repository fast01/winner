/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Observer impl ***/
	/** ctor & dtor**/
	Observer::Observer(){
	}
	Observer::~Observer(){}

	/** self **/
	void Observer::notify(Actor* actor, Event* evt){
	}
}

