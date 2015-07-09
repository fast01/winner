/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Timer ***/	
	/** ctor & dtor **/	
	Timer::Timer(const int64_t id)
		: m_id(id)
		, m_interval(0)
		, m_expire_time(0)
		, m_one_shot(false)
		, m_userdata(0)
		, m_pfn(0){
	}
	Timer::~Timer(){
	}

	/** setter **/
	void Timer::set(const int64_t interval, PFN_TIMEOUT_CALLBACK pfn, void* userdata){
		if(interval < 1){
			m_interval =1;
		}
		else{
			m_interval =interval;
		}
		m_expire_time =stable_time() + m_interval;
		m_one_shot =false;
		m_userdata =userdata;
		m_pfn =pfn;
	}
	void Timer::setOneShot(const bool one_shot){
		m_one_shot =one_shot;
	}
	/** getter **/
	int64_t Timer::getId(){
		return m_id;
	}
	int64_t Timer::getExpireTime(){
		return m_expire_time;
	}
	int64_t Timer::getInterval(){
		return m_interval;
	}
	bool Timer::isOneShot(){
		return m_one_shot;
	}
	void* Timer::getUserdata(){
		return m_userdata;
	}
	Timer::PFN_TIMEOUT_CALLBACK Timer::getCallback(){
		return m_pfn;
	}
	/** wakeup **/
	void Timer::wakeup(Service* service, const int64_t now){
		if(m_pfn == 0){
			return;
		}
		m_pfn(service, now, m_interval, m_userdata);
		m_expire_time =now + m_interval;
	}
}
