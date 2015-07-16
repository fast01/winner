/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl CoroutinePool ***/
	/** ctor & dtor **/
	CoroutinePool::CoroutinePool()
		: m_active_coroutine_table(0)
		, m_idle_coroutine_list(0)
		, m_coroutine_id(0)
		, m_cleaning(false){
		memset(m_main_stack, 0, MAIN_STACK_SIZE);
		memset(&m_main_context, 0, sizeof(m_main_context));
	}
	CoroutinePool::~CoroutinePool(){
	}
	/** Object **/
	void CoroutinePool::init(){
		Super::init();
		ASSIGN_POINTER(m_active_coroutine_table, SafeNew<Hash>());
		ASSIGN_POINTER(m_idle_coroutine_list, SafeNew<Array>());
	}
	void CoroutinePool::finalize(){
		m_cleaning =true;

		// clean idle list
		for(int64_t i=0; i<m_idle_coroutine_list->size(); ++i){
			Coroutine* cr =static_cast< Coroutine* >(m_idle_coroutine_list->get(i));
			cr->resume(SafeNew<Error>(ErrorCode::COROUTINE_CLEAN), 0);
			ASSERT(cr->getStatus() == Coroutine::STATUS_DEAD);
		}
		CLEAN_POINTER(m_idle_coroutine_list);

		// clean active table
		Hash* active_cr_tb =m_active_coroutine_table;
		m_active_coroutine_table =0;
		HashIterator* it =static_cast< HashIterator* >(active_cr_tb->iterator());
		while(it->next()){
			Coroutine* cr =static_cast< Coroutine* >(it->getValue());
			cr->resume(SafeNew<Error>(ErrorCode::COROUTINE_CLEAN), 0);
			ASSERT(cr->getStatus() == Coroutine::STATUS_DEAD);
		}
		CLEAN_POINTER(active_cr_tb);

		// super
		Super::finalize();
	}
	/** stack **/
	char* CoroutinePool::getMainStack(){
		return m_main_stack;
	}
	int64_t CoroutinePool::getMainStackSize(){
		return MAIN_STACK_SIZE;
	}
	char* CoroutinePool::getMainStackHighAddr(){
		return m_main_stack + MAIN_STACK_SIZE;
	}
	ucontext_t* CoroutinePool::getMainContext(){
		return &m_main_context;
	}
	bool CoroutinePool::isCleaning(){
		return m_cleaning;
	}
	/** coroutine manager **/
	void CoroutinePool::update(const int64_t now){
		// process timeout
		if(m_active_coroutine_table && m_active_coroutine_table->size()>0){
			// prepare
			Int64Array* ls =0;
			HashIterator* it =static_cast< HashIterator* >(m_active_coroutine_table->iterator());
			while(it->next()){
				Coroutine* cr =static_cast< Coroutine* >(it->getValue());
				if(cr->isWaitingAndExpire(now)){
					if(!ls){
						ls =SafeNew<Int64Array>();
					}
					ls->push_back(cr->getId());
				}
			}
			// resume
			if(ls && ls->size()>0){
				const int64_t n =ls->size();
				for(int64_t i=0; i<n; ++i){
					resume(ls->get(i), SafeNew<Error>(ErrorCode::TIMEOUT), 0);
				}
			}
		}
	}
	int64_t CoroutinePool::resume(const int64_t id, Object* param, const int64_t sign){
		if(m_cleaning){
			WARN("coroutine pool resume failed, cleaning");
			return -ErrorCode::SYSTEM_ERROR;
		}
		if(Coroutine* cr =static_cast< Coroutine* >(m_active_coroutine_table->get(id))){
			return _resume(cr, param, sign);
		}
		else{
			WARN("coroutine manager resume failed, coroutine %lld not found", (long long)id);
			return -ErrorCode::NOT_EXIST;
		}
	}
	int64_t CoroutinePool::go(Coroutine::PFN_COROUTINE_TASK pfn, Object* arg, int64_t& cr_id){
		if(m_cleaning){
			WARN("coroutine pool go failed, cleaning");
			return -ErrorCode::SYSTEM_ERROR;
		}
		if(!pfn){
			return -ErrorCode::INVALID_ARG;
		}
		// prepare
		Coroutine* cr =_prepare_coroutine(pfn, arg);
		const int64_t result =_resume(cr, 0, 0);
		if(result == Coroutine::STATUS_WAITING){
			cr_id =cr->getId();
		}
		return result;
	}
	/** helper **/
	Coroutine* CoroutinePool::_prepare_coroutine(Coroutine::PFN_COROUTINE_TASK pfn, Object* arg){
		// prepare coroutine
		GENERATE_ID(m_coroutine_id);
		Coroutine* cr =0;
		if(m_idle_coroutine_list->size() > 0){
			cr =dynamic_cast< Coroutine* >(m_idle_coroutine_list->back());
			m_active_coroutine_table->set(m_coroutine_id, cr);
			m_idle_coroutine_list->pop_back();
		}
		else{
			cr =SafeNew<Coroutine>(this);
			m_active_coroutine_table->set(m_coroutine_id, cr);
		}
		// set status
		cr->setId(m_coroutine_id);
		cr->setTask(pfn, arg);
		return cr;
	}
	int64_t CoroutinePool::_resume(Coroutine* cr, Object* param, const int64_t sign){
		// resume
		const int64_t ret =cr->resume(param, sign);
		if(ret < 0){
			return ret;
		}

		// process
		if(cr->isWaiting()){
			return ret;
		}
		else if(cr->isIdle()){
			if(m_idle_coroutine_list->size() < MAX_IDLE_COROUTINE_COUNT){
				m_idle_coroutine_list->push_back(cr);
			}
			m_active_coroutine_table->remove(cr->getId());
			return ret;
		}
		else{
			FATAL("never run to here %s(%d):%s", __FILE__, __LINE__, __func__);
			return ret;
		}
	}
}
