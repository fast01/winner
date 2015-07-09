/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_THREAD_H__
#define H_CORE_THREAD_H__

namespace core{
	/** Thread **/
	class Thread: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef void (*PFN_CALLBACK)(Object* param);
	public:
		enum{
			STATE_INIT,
			STATE_RUNNING,
			STATE_DEAD	
		};
	private:
		Thread();
		virtual ~Thread();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual bool startup(PFN_CALLBACK pfn, Object* param);
		void join();
	private:
		static void _thread_entry(Thread* self);
	private:
		std::atomic<int64_t> m_state;
		std::thread m_thread;
		PFN_CALLBACK m_pfn;
		Object* m_param;
	};
}

#endif
