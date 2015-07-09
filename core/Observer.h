/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_OBSERVER_H__
#define H_CORE_OBSERVER_H__

namespace core{
	/** predeclare **/
	class Actor;
	class Event;

	/** Observer **/
	class Observer: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	protected:
		Observer();
		~Observer();
	public:
		virtual void notify(Actor* actor, Event* evt);
	};
}

#endif
