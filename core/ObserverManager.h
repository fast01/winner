/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_OBSERVER_MANAGER_H__
#define H_CORE_OBSERVER_MANAGER_H__

namespace core{
	/** predecl **/
	class Actor;
	class Event;

	/** ObserverManager **/
	class ObserverManager: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_THREAD_LOCAL_SINGLETON(ObserverManager)
	protected:
		ObserverManager();
		virtual ~ObserverManager();
	public:
		virtual void init();
		virtual void finalize();
	public:
		Observer* get(String* observer_name);
	public:
		void notify(String* observer_name, Actor* actor, Event* evt);
		static void Notify(String* observer_name, Actor* actor, Event* evt);
	public:
		Hash* m_observer_table;
	};
}
#endif

