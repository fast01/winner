/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_INVOKER_H__
#define H_CORE_INVOKER_H__

namespace core{
	/** Invoker **/
	class Invoker: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef int64_t (*PFN_FUNC)(Object* arg);
	protected:
		Invoker();
		virtual ~Invoker();
	public:
		virtual void finalize();
	public:
		virtual int64_t run(Object* arg);
	public:
		void setFunc(PFN_FUNC pfn);
		PFN_FUNC getFunc();
	private:
		PFN_FUNC m_pfn;
	};
}

#endif
