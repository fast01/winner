/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PARALLEL_NODE_H__
#define H_CORE_PARALLEL_NODE_H__

namespace core{
	/** predecl **/
	class Parallel;

	/** ParallelNode **/
	class ParallelNode: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef bool (*PFUNC)(Object* context);
	private:
		ParallelNode(PFUNC fn, Parallel* parallel);
		virtual ~ParallelNode();
	public:
		virtual void init();
		virtual void finalize();
	public:
		static void Invoke(Object* node);
	private:
		PFUNC m_fn;
		Parallel* m_parallel; // weak ptr
	};
}

#endif
