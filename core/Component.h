/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COMPONENT_H__
#define H_CORE_COMPONENT_H__

namespace core{
	/** predeclare **/
	class Actor;

	/** Component **/
	class Component: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	protected:
		Component();
		~Component();
	public:
		void setActor(Actor* actor);
		Actor* getActor();
	private:
		Actor* m_actor; // weak ptr
	};
}

#endif

