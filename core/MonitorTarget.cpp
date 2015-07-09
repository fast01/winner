/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl MonitorTarget ***/
	/** ctor & dtor **/
	MonitorTarget::MonitorTarget()
		: m_heart_beat_timer(0)
		, m_dead_time(0)
		, m_monitor(0)
		, m_monitor_id(-1){
	}
	MonitorTarget::~MonitorTarget(){}

	/** Object **/
	void MonitorTarget::init(){
		Super::init();
	}
	void MonitorTarget::finalize(){
		Super::finalize();
	}

	/** MonitorTarget **/
	bool MonitorTarget::reborn(){
		return false;
	}
	void MonitorTarget::sucide(){
	}
	bool MonitorTarget::isLive(){
		return false;
	}
	bool MonitorTarget::canReborn(){
		return false;
	}
	bool MonitorTarget::canMonitor(){
		return true;
	}
	int64_t MonitorTarget::getMonitorId(){
		return m_monitor_id;
	}
	bool MonitorTarget::isMonitored(){
		return m_monitor_id != -1;
	}
	Monitor* MonitorTarget::getMonitor(){
		return m_monitor;
	}
	/** event **/
	bool MonitorTarget::onEvent(const fd_t fd, const uint64_t events){
		if(events & EVT_ERROR){
			return false;
		}
		else{
			return true;
		}
	}
	bool MonitorTarget::onAttachEvent(Monitor* monitor){
		// check
		if(!monitor){
			ERROR("call %s failed, monitor is null", __func__);
			return false;
		}

		// check and set
		if(m_monitor){
			if(m_monitor != monitor){
				ERROR("call %s failed, monitor mismatch", __func__);
				return false;
			}
		}
		else{
			m_monitor =monitor;
			m_monitor_id =m_monitor->getId();
		}

		// check live
		return isLive();
	}
	void MonitorTarget::onDetachEvent(){
	}
	void MonitorTarget::onAbandon(){
		m_monitor =0;
		m_monitor_id =-1;
	}

	/** heart beat **/
	void MonitorTarget::setHeartBeatTimer(const int64_t secs){
		m_heart_beat_timer =secs;
	}
	int64_t MonitorTarget::getHeartBeatTimer(){
		return m_heart_beat_timer;
	}
	bool MonitorTarget::isHeartBeatEnable(){
		return m_heart_beat_timer > 0;
	}
	void MonitorTarget::heartBeat(const int64_t now){
		m_dead_time =now + m_heart_beat_timer;
	}
	int64_t MonitorTarget::getDeadTime(){
		return m_dead_time;
	}
}
