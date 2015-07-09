/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PAIR_H__
#define H_CORE_PAIR_H__

namespace core{
	/** Pair **/
	class Pair: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		Pair(Object* first, Object* second);
		virtual ~Pair();
	public:
		virtual void init();
		virtual void finalize();
	public:
		DECLARE_PROPERTY(Object*, First)
		DECLARE_PROPERTY(Object*, Second)
	};
}

#endif
