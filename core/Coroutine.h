/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COROUTINE_H__
#define H_CORE_COROUTINE_H__

namespace core{
	/** predeclare **/
	class CoroutinePool;

	/** Coroutine **/
	class Coroutine : public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef void (*PFN_COROUTINE_TASK)(Object* arg);
		enum{
			STATUS_INIT =0,
			STATUS_DESTORY =1,
			STATUS_DEAD =2,
			STATUS_RUNNING =3,
			STATUS_WAITING =4,
			STATUS_IDLE =5,
		};
		enum{
			STACK_PROTECT_SIZE =64,
			WAITING_TIMER =30
		};
	public:
		Coroutine(CoroutinePool* pool);
		virtual ~Coroutine();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setId(const int64_t id);
		int64_t getId();
	public:
		int64_t getStatus();
		bool isDead();
		bool isRunning();
		bool isWaiting();
		bool isIdle();
		bool canYield();
		bool isWaitingAndExpire(const int64_t now);
	public:
		void setTask(PFN_COROUTINE_TASK pfn, Object* arg);
		PFN_COROUTINE_TASK getTask();
		Object* getArg();
		ucontext_t* getContext();
		ObjectPool* getObjectPool();
	public:
		bool yield(Object* yield_param, const int64_t sign);
		Object* getYieldParam();
		static bool Yield(Object* param, const int64_t sign);
	public:
		int64_t resume(Object* param, const int64_t sign);
		Object* getResumeParam();
	public:
		static Coroutine* Running();
	private:
		static void _entry();
		bool _yield(Object* yield_param, const int64_t status, const int64_t sign);
		int64_t _resume(Object* param, const int64_t sign);
		void _valgrind_register();
		void _valgrind_unregister();
		static int _swapcontext(ucontext_t *oucp, ucontext_t *ucp);
	private:
		ucontext_t m_ctx;
		int64_t m_id;
		CoroutinePool* m_coroutine_pool; // weak ptr
		ObjectPool* m_object_pool;
		Coroutine* m_resumer_cr;
		char* m_stack;
		int64_t m_stack_size;
		int64_t m_status;
		int64_t m_waiting_expire_time;
		int64_t m_sign;
		PFN_COROUTINE_TASK m_task;
		Object* m_arg;
		Object* m_yield_param;
		Object* m_resume_param;
		int64_t m_valgrind_id;
		bool m_valgrind_registered;
	};
}

#endif
