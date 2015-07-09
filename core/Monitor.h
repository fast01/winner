/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_MONITOR_H__
#define H_CORE_MONITOR_H__

namespace core{
	/** Monitor **/
	class Monitor: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_THREAD_LOCAL_SINGLETON(Monitor)
	public:
		enum{
			EPOLL_SIZE =102400,
			EVENT_LIST_MAX_COUNT =32,
			REBORN_TIMER =3 // 3 secs
		};
	private:
		Monitor();
		virtual ~Monitor();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setId(const int64_t id);
		int64_t getId();
	public:
		bool watch(const int64_t timeout);
		void close();
	public:
		bool monitor(MonitorTarget* target);
		void demonitor(MonitorTarget* target);
	public:
		bool attachEvent(const int fd, const int64_t events, MonitorTarget* target);
		bool modifyEvent(const int fd, const int64_t events, MonitorTarget* target);
		void detachEvent(const int fd);
	public:
		static void Close();
	private:
		bool _check_epoll();
		void _close_epoll();
		uint64_t _translate_from_epoll_events(const uint64_t evts);
		uint64_t _translate_to_epoll_events(const uint64_t evts);
		void _abandon(MonitorTarget* target);
		void _clean();
	private:
		int64_t m_id;
		int m_epoll_fd;
		struct epoll_event m_event_list[EVENT_LIST_MAX_COUNT];

		Hash* m_target_tb;
		Hash* m_reborn_tb;
		Hash* m_dying_tb;
		Hash* m_fd_tb;

		bool m_close_flag;
	};
}

#endif
