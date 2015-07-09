/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CLASS_INFO_H__
#define H_CORE_CLASS_INFO_H__

namespace core{
	/** ClassInfo **/
	class ClassInfo: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef Object* (*PFN_CREATOR)();
	private:
		ClassInfo(String* name, PFN_CREATOR creator);
		virtual ~ClassInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		static ClassInfo* New(String* name, PFN_CREATOR creator);
	public:
		PFN_CREATOR getCreator();
		Object* create();
	public:
		void setName(String* name);
		String* getName();
		String* getSuperName();
		ClassInfo* getSuper();
		DECLARE_PROPERTY_DYNAMIC_ARRAY(Array*, FieldInfoBase*, FieldInfo)
	private:
		String* m_name;
		PFN_CREATOR	m_creator;
		String* m_super_name;
	};
}

#endif
