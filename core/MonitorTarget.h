/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_MONITOR_TARGET_H__
#define H_CORE_MONITOR_TARGET_H__

namespace core{
	/** predecl **/
	class Monitor;

	/** MonitorTarget **/
	class MonitorTarget: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
	public:
		enum{
			EVT_READ =1 << 0,
			EVT_WRITE =1 << 1,
			EVT_ERROR =1 << 2,
		};
	protected:
		MonitorTarget();
		virtual ~MonitorTarget();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual bool reborn();
		virtual bool canReborn();
		virtual void sucide();
		virtual bool isLive();
		virtual bool canMonitor();
		int64_t getMonitorId();
		bool isMonitored();
		Monitor* getMonitor();
	public:
		virtual bool onEvent(const fd_t fd, const uint64_t events);
		virtual bool onAttachEvent(Monitor* monitor);
		virtual void onDetachEvent();
		virtual void onAbandon();
	public:
		void setHeartBeatTimer(const int64_t secs);
		int64_t getHeartBeatTimer();
		bool isHeartBeatEnable();
		void heartBeat(const int64_t now);
		int64_t getDeadTime();
	private:
		int64_t m_heart_beat_timer;
		int64_t m_dead_time;
	protected:
		Monitor* m_monitor; // weak ptr
		std::atomic<int64_t> m_monitor_id; // monitor id 
	};
}
#endif
