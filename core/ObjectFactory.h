/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_OBJECT_FACTORY_H__
#define H_CORE_OBJECT_FACTORY_H__

namespace core{
	/** ObjectFactory **/
	class ObjectFactory: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
		DECLARE_PROCESS_LOCAL_SINGLETON(ObjectFactory)
	private:
		ObjectFactory();
		virtual ~ObjectFactory();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void registerClassInfo(ClassInfo* ci);
		ClassInfo* getClassInfo(String* id);
		Object* create(String* id);
	public:
		static Object* Create(String* id);
		static ClassInfo* GetClassInfo(String* id);
	private:
		Hash* m_class_info_tb;
	};
}

#endif
