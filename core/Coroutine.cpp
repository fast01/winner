/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** valgrind helper ***/
	static int64_t valgrind_register(void* beg, void* end){
#ifdef VALGRIND_CHECK_ENABLE
		const int64_t id =VALGRIND_STACK_REGISTER(beg, end);
		return id;
#else
		return 0;
#endif
	}
	static void valgrind_unregister(const int64_t id){
#ifdef VALGRIND_CHECK_ENABLE
		VALGRIND_STACK_DEREGISTER(id);
#endif
	}

	/*** impl Coroutine ***/
	/** ctor & dtor **/
	Coroutine::Coroutine(CoroutinePool* pool)
		: m_id(0)
		, m_coroutine_pool(pool)
		, m_object_pool(0)
		, m_resumer_cr(0)
		, m_stack(0)
		, m_stack_size(0)
		, m_status(STATUS_INIT)
		, m_waiting_expire_time(0)
		, m_sign(0)
		, m_task(0)
		, m_arg(0)
		, m_yield_param(0)
		, m_resume_param(0)
		, m_valgrind_id(0){
		ASSERT(m_coroutine_pool);
		memset(&m_ctx, 0, sizeof(m_ctx));
	}
	Coroutine::~Coroutine(){
	}
	/** init & finalize **/
	void Coroutine::init(){
		Super::init();
	}
	void Coroutine::finalize(){
		if(m_status != STATUS_DEAD){
			FATAL("!!! coroutine need cleaned");
		}
#ifdef VALGRIND_CHECK_ENABLE
		if(m_stack){
			_valgrind_unregister();
		}
#endif
		DEALLOCATE(m_stack);
		m_status =STATUS_DESTORY;
		CLEAN_POINTER(m_arg);
		CLEAN_POINTER(m_yield_param);
		CLEAN_POINTER(m_resume_param);
		CLEAN_POINTER(m_resumer_cr);
		ASSERT(m_object_pool == 0);
		Super::finalize();
	}
	/** id **/
	void Coroutine::setId(const int64_t id){
		m_id =id;
	}
	int64_t Coroutine::getId(){
		return m_id;
	}
	/** status **/
	int64_t Coroutine::getStatus(){
		return m_status;
	}
	bool Coroutine::isDead(){
		return m_status == STATUS_DEAD;
	}
	bool Coroutine::isRunning(){
		return m_status == STATUS_RUNNING;
	}
	bool Coroutine::isWaiting(){
		return m_status == STATUS_WAITING;
	}
	bool Coroutine::isIdle(){
		return m_status == STATUS_IDLE;
	}
	bool Coroutine::canYield(){
		if(!isRunning()){
			return false;
		}
		CoroutineService* service =dynamic_cast< CoroutineService* >(Service::Current());
		ASSERT(service);
		CoroutinePool* crp =service->getCoroutinePool();
		ASSERT(crp);
		return false == crp->isCleaning();
	}
	/** running **/
	static thread_local Coroutine* g_running =0;
	Coroutine* Coroutine::Running(){
		return g_running;
	}
	/** task **/
	void Coroutine::setTask(PFN_COROUTINE_TASK pfn, Object* arg){
		if(m_status == STATUS_DEAD){
			WARN("coroutine set task failed, status is dead");
			return;
		}
		m_task =pfn;
		ASSIGN_POINTER(m_arg, arg);
	}
	Coroutine::PFN_COROUTINE_TASK Coroutine::getTask(){
		return m_task;
	}
	Object* Coroutine::getArg(){
		return m_arg;
	}
	ucontext_t* Coroutine::getContext(){
		return &m_ctx;
	}
	ObjectPool* Coroutine::getObjectPool(){
		return m_object_pool;
	}
	/** yield **/
	bool Coroutine::yield(Object* yield_param, const int64_t sign){
		return _yield(yield_param, STATUS_WAITING, sign);
	}
	Object* Coroutine::getYieldParam(){
		return m_yield_param;
	}
	bool Coroutine::Yield(Object* param, const int64_t sign){
		if(Coroutine* cr =Running()){
			return cr->yield(param, sign);
		}
		else{
			ERROR("fail to %s, in main thread", __FUNCTION__);
			return false;
		}
	}
	/** resume **/
	int64_t Coroutine::resume(Object* param, const int64_t sign){
		return _resume(param, sign);
	}
	Object* Coroutine::getResumeParam(){
		return m_resume_param;
	}
	/** expire **/
	bool Coroutine::isWaitingAndExpire(const int64_t now){
		return isWaiting() && now >= m_waiting_expire_time && m_sign>=0;
	}
	/** entry **/
	void Coroutine::_entry(){
		// prepare
		ObjectPool* obj_pool =New<ObjectPool>();
		// DEBUG("new set op %p", obj_pool);
		ObjectPool::Set(obj_pool);

		Coroutine* self =Running();
		ASSERT(self);
		self->retain();
		ASSIGN_POINTER(self->m_object_pool, obj_pool);

		// loop
		while(1){
			OPH();
			// check break
			if(Error* err= dynamic_cast< Error* >(self->m_resume_param)){
				if(err->getCode() == ErrorCode::COROUTINE_CLEAN){
					break;
				}
			}
			// run task
			try{
				if(self->m_task){
					self->m_task(self->m_arg);
					self->m_task =0;
					CLEAN_POINTER(self->m_arg);
				}
			}
			catch(...){
				ERROR("coroutine exec task occurs exception, perhaps coroutine is cleaned");
				break;
			}
			// yield
			self->_yield(0, STATUS_IDLE, 0);
		}
		
		// clean
		{
			OPH();
			ASSERT(self->m_object_pool == ObjectPool::Instance());
			// DEBUG("coroutine %lld exit", (long long)self->m_id);
			self->m_status =STATUS_DEAD;
			g_running =0;
			CLEAN_POINTER(self->m_object_pool);
			self->release();
		}

		// release object pool
		// DEBUG("new set op 0");
		ObjectPool::Release();
	}
	bool Coroutine::_yield(Object* yield_param, const int64_t status, const int64_t sign){
		// set sign
		m_sign =sign;

		// check
		if(m_coroutine_pool->isCleaning()){
			WARN("coroutine yield failed, coroutine pool is cleaning");
			return false;
		}
		if(m_status != STATUS_RUNNING){
			FATAL("coroutine yield failed, not running");
			return false;
		}
		// assigin pointer by hand
		if(m_yield_param != yield_param){
			if(yield_param){
				yield_param->retain();
			}
			if(m_yield_param){
				m_yield_param->release();
			}
			m_yield_param =yield_param;
		}

#ifndef VALGRIND_CHECK_ENABLE
		// store stack
		char tricky =0;
		m_stack_size =m_coroutine_pool->getMainStackHighAddr() - &tricky + STACK_PROTECT_SIZE;
		if(m_stack_size > m_coroutine_pool->getMainStackSize()){
			FATAL("stack penetrate, stack size is %lld", m_stack_size);
			return false;
		}
		m_stack =reinterpret_cast< char* >(REALLOCATE(m_stack, m_stack_size));
		memcpy(m_stack, m_coroutine_pool->getMainStackHighAddr() - m_stack_size, m_stack_size);
#endif
		// set status
		Coroutine* resumer_cr =m_resumer_cr;
		m_resumer_cr =0;
		m_status =status;
		g_running =resumer_cr;

		// set wait timestamp
		if(m_status == STATUS_WAITING){
			m_waiting_expire_time =stable_time() + 3; //Coroutine::WAITING_TIMER;
		}

		// prepare context
		ucontext_t* resumer_ctx;
		if(resumer_cr){
			if(resumer_cr->isDead()){
				FATAL("coroutine yield failed, resumer coroutine is dead");
				return false;
			}
			else{
				resumer_ctx =resumer_cr->getContext();
			}
		}
		else{
			resumer_ctx =m_coroutine_pool->getMainContext();
		}
		ASSERT(resumer_ctx);

		// swap context
		if(0 != _swapcontext(&m_ctx, resumer_ctx)){
			FATAL("fail to %s, %s", get_last_error_desc());
		}
		CLEAN_POINTER(resumer_cr);
		return true;
	}
	int64_t Coroutine::_resume(Object* param, const int64_t sign){
		// check sign
		if(m_status == STATUS_WAITING){
			if(sign!=0 && sign!=m_sign){
				WARN("coroutine resume failed, sign missmatch");
				return -ErrorCode::COROUTINE_SIGN_MISSMATCH;
			}
		}
		// check
		Coroutine* running =g_running;
		Coroutine* cr =this;
		while(cr && running){
			if(running == m_resumer_cr){
				FATAL("coroutine resume failed, resume loop");
				return -ErrorCode::SYSTEM_ERROR;
			}
			cr =cr->m_resumer_cr;
		}
		// check status
		if(m_status == STATUS_DEAD){
			FATAL("coroutine resume failed, status is dead");
			return -ErrorCode::SYSTEM_ERROR;
		}
		// prepare resumer coroutine and context
		ASSIGN_POINTER(m_resumer_cr, running);
		ucontext_t* resumer_ctx;
		if(m_resumer_cr){
			resumer_ctx =m_resumer_cr->getContext();
		}
		else{
			resumer_ctx =m_coroutine_pool->getMainContext();
		}
		ASSERT(resumer_ctx);

		// prepare stack
#ifdef VALGRIND_CHECK_ENABLE
		if(m_stack == 0){
			m_stack =reinterpret_cast< char* >(ALLOCATE(CoroutinePool::MAIN_STACK_SIZE));
			m_stack_size =CoroutinePool::MAIN_STACK_SIZE;
			if(0 != getcontext(&m_ctx)){
				FATAL("fail to %s, %s", get_last_error_desc());
				return -ErrorCode::SYSTEM_ERROR;
			}
			m_ctx.uc_stack.ss_sp =m_stack;
			m_ctx.uc_stack.ss_size =m_stack_size;
			m_ctx.uc_stack.ss_flags =0;
			m_ctx.uc_link =m_coroutine_pool->getMainContext();
			makecontext(&m_ctx, (void(*)())(_entry), 0);
			_valgrind_register();
		}
#else
		if(m_stack == 0){
			if(0 != getcontext(&m_ctx)){
				FATAL("fail to %s, %s", get_last_error_desc());
				return -ErrorCode::SYSTEM_ERROR;
			}
			m_stack_size =0;
			m_ctx.uc_stack.ss_sp =m_coroutine_pool->getMainStack();
			m_ctx.uc_stack.ss_size =m_coroutine_pool->getMainStackSize();
			m_ctx.uc_stack.ss_flags =0;
			m_ctx.uc_link =m_coroutine_pool->getMainContext();
			makecontext(&m_ctx, (void(*)())(_entry), 0);
		}
		else{
			memcpy(m_coroutine_pool->getMainStackHighAddr() - m_stack_size, m_stack, m_stack_size);
		}
#endif

		// set status
		m_status =STATUS_RUNNING;
		g_running =this;
		ASSIGN_POINTER(m_resume_param, param);

		// change object pool
		// DEBUG("resume set op %p", m_object_pool);
		ObjectPool* old =ObjectPool::Instance();
		RETAIN_POINTER(old);
		ObjectPool::Set(m_object_pool);

		// swap context
		if(0 != _swapcontext(resumer_ctx, &m_ctx)){
			FATAL("fail to %s, %s", get_last_error_desc());
			return -ErrorCode::SYSTEM_ERROR;
		}
		ObjectPool::Set(old);
		RELEASE_POINTER(old);
		return m_status;
	}
	void Coroutine::_valgrind_register(){
		m_valgrind_id =valgrind_register(m_stack, m_stack + m_stack_size);
	}
	void Coroutine::_valgrind_unregister(){
		valgrind_unregister(m_valgrind_id);
		m_valgrind_id =0;
	}
	int Coroutine::_swapcontext(ucontext_t *oucp, ucontext_t *ucp){
		memory_barrier();
		const int ret =swapcontext(oucp, ucp);
		memory_barrier();
		return ret;
	}
}
