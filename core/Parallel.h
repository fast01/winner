/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PARALLEL_H__
#define H_CORE_PARALLEL_H__

namespace core{
	/** Parallel **/
	class Parallel: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		Parallel();
		virtual ~Parallel();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void addTask(ParallelNode::PFUNC fn);
		int64_t getTaskCount();
	public:
		bool run(Object* context);
	public:
		int64_t getRemainCount();
		int64_t getErrorCount();

		void decrRemainCount();
		void incrErrorCount();
	public:
		Object* getContext();
		void resume();
	private:
		Array* m_node_list;
		Object* m_context;
		Coroutine* m_cr; // weak ptr
		int64_t m_remain_count;
		int64_t m_error_count;
		bool m_yield;
	};
}

#endif
