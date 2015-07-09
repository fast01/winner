/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CHANNEL_H__
#define H_CORE_CHANNEL_H__

namespace core{
	/** Channel **/
	class Channel : public MonitorTarget{
		SUPPORT_NEWABLE
		typedef MonitorTarget Super;
	public:
		typedef bool (*PFN_ON_EVENT)(Channel*, const fd_t, const int64_t events, Object* ctx);
	protected:
		Channel();
		virtual ~Channel();
	public:
		virtual void init();
		virtual void finalize();
		virtual Object* clone();
	public:
		virtual bool reborn();
		virtual void sucide();
		virtual bool isLive();
		virtual bool canReborn();
		virtual bool onEvent(const fd_t fd, const uint64_t events);
		virtual bool onAttachEvent(Monitor* monitor);
		virtual void onDetachEvent();
	public:
		bool push(Object* obj);
		void pop();
		Object* front();
		bool empty();
		int64_t size();
		bool good();
	public:
		void setPushable(const bool yes);
		bool canPush();
		bool signal();
		bool unsignal();
		void setEventCallback(PFN_ON_EVENT pfn, Object* ctx);
	private:
		bool _create_fd();
		void _close_fd();
		void _read_event_fd();
		bool _push(Object* obj);
		void _pop();
	private:	
		Array* m_container;
		fd_t m_event_fd;
		bool m_pushable;

		PFN_ON_EVENT m_on_event;
		Object* m_ctx;

		typedef DEFAULT_LOCK_TYPE LOCK_TYPE;
		LOCK_TYPE m_lock;
	};
}
#endif
