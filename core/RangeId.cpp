/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** RangeId  ***/
	//** ctor & dtor **//
	RangeId::RangeId()
		: m_begin_id(0)
		, m_end_id(0)
		, m_algrithm_type(ALG_DEFAULT)
		, m_round_robin_id(0){
	}
	RangeId::~RangeId(){
	}
	/** self **/
	int64_t RangeId::getAlgrithm(){
		return m_algrithm_type;
	}
	int64_t RangeId::getBeginId(){
		return m_begin_id;
	}
	int64_t RangeId::getEndId(){
		return m_end_id;
	}
	int64_t RangeId::getId(const int64_t from, const int64_t who){
		const int64_t cnt =m_end_id - m_begin_id;
		if(cnt <= 1){
			return m_begin_id;
		}
		if(m_algrithm_type==ALG_USE_WHO){
			return m_begin_id + who % cnt;
		}
		else if(m_algrithm_type == ALG_USE_FROM){
			return m_begin_id + from % cnt;
		}
		else if(m_algrithm_type == ALG_USE_FROM_WHO){
			return m_begin_id + (from*who) % cnt;
		}
		else if(m_algrithm_type == ALG_RANDOM){
			return m_begin_id + rand() % cnt;
		}
		else if(m_algrithm_type == ALG_ROUND_ROBIN){
			m_round_robin_id +=1;
			return m_begin_id + m_round_robin_id % cnt;
		}
		else{
			return m_begin_id;
		}
	}
	bool RangeId::isConsistent(){
		return m_algrithm_type < ALG_RANDOM;
	}
	void RangeId::setRange(const int64_t beg, const int64_t end){
		m_begin_id =beg;
		m_end_id =end;
	}
	void RangeId::setAlgrithm(const int64_t type){
		m_algrithm_type =type;
	}
}
