/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ACTOR_H__
#define H_CORE_ACTOR_H__

namespace core{
	/** predeclare **/
	class DataBase;

	/** Actor **/
	class Actor: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	protected:
		Actor();
		~Actor();
	public:
		virtual void init();
		virtual void finalize();
	public:
		bool addObserver(Observer* observer);
		Observer* getObserver(String* observer_class_name);
		Array* getAllObserver();
	public:
		bool addComponent(Component* com);
		Component* getComponent(String* component_class_name);
		Array* getAllComponent();
	private:
		Hash* m_observer_manager;
		Hash* m_component_manager;
	};
}

#endif
