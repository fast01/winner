/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Pair ***/
	/** ctor & dtor **/
	Pair::Pair(Object* first, Object* second)
		: m_First(first)
		, m_Second(second){
		RETAIN_POINTER(m_First);
		RETAIN_POINTER(m_Second);
	}
	Pair::~Pair(){
	}

	/** Object **/
	void Pair::init(){
		Super::init();
	}
	void Pair::finalize(){
		CLEAN_POINTER(m_First);
		CLEAN_POINTER(m_Second);
		Super::finalize();
	}

	/** Object **/
	DEFINE_PROPERTY_P(Pair, Object*, First)
	DEFINE_PROPERTY_P(Pair, Object*, Second)
}
