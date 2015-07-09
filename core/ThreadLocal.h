/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_THREAD_LOCAL_H__
#define H_CORE_THREAD_LOCAL_H__

namespace core{
	/** ThreadLocal **/
	class ThreadLocal: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
	protected:
		ThreadLocal();
		virtual ~ThreadLocal();
	public:
		virtual void init();
		virtual void finalize();
	public:
		bool update();
	public:
		static void Init();
		static ThreadLocal* Instance();
		static void Release();
	public:
		Logger* getLogger();
		Monitor* getMonitor();
		ObserverManager* getObserverManager();
		ActorManager* getActorManager();
	private:
		Logger* m_logger;
		Monitor* m_monitor;
		ObserverManager* m_observer_manager;
		ActorManager* m_actor_manager;
	};
}

#endif
