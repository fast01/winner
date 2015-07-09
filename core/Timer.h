/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TIMER_H__
#define H_CORE_TIMER_H__

namespace core{
	/** predecl **/
	class Service;

	/** class Timer **/
	class Timer : public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef void (*PFN_TIMEOUT_CALLBACK)(Service* service, const int64_t now, const int64_t interval, void* userdata);
	private:
		Timer(const int64_t id);
		~Timer();
	public:
		void set(const int64_t interval, PFN_TIMEOUT_CALLBACK pfn, void* userdata);
		void setOneShot(const bool one_shot);
	public:
		int64_t getId();
		int64_t getExpireTime();
		int64_t getInterval();
		bool isOneShot();
		void* getUserdata();
		PFN_TIMEOUT_CALLBACK getCallback();
	public:
		void wakeup(Service* service, const int64_t now);
	public:
		int64_t m_id;
		int64_t m_interval;
		int64_t m_expire_time;
		bool m_one_shot;
		void* m_userdata;
		PFN_TIMEOUT_CALLBACK m_pfn;
	};
}

#endif
