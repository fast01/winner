/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TASK_H__
#define H_CORE_TASK_H__

namespace core{
	/** Task **/
	class Task: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		enum{
			STATE_INIT,
			STATE_RUNNING,
			STATE_FINISHED,
			STATE_CANCELED
		};
	private:
		Task();
		virtual ~Task();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual void run();
		int64_t getState();
	private:
		std::atomic<int64_t> m_state;
	};
}

#endif
