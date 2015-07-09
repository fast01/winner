/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Parallel ***/
	/** ctor & dtor **/
	Parallel::Parallel()
		: m_node_list(0)
		, m_context(0)
		, m_cr(0)
		, m_remain_count(0)
		, m_error_count(0){
	}
	Parallel::~Parallel(){
	}

	/** Object **/
	void Parallel::init(){
		Super::init();
		m_node_list =New<Array>();
		m_node_list->retain();
	}
	void Parallel::finalize(){
		if(m_node_list->size() > 0){
			WARN("Parallel %p is not empty", (void*)this);
		}
		CLEAN_POINTER(m_node_list);
		ASSERT(m_context == 0);
		Super::finalize();
	}

	/** self **/
	void Parallel::addTask(ParallelNode::PFUNC fn){
		m_node_list->push_back(SafeNew<ParallelNode>(fn, this));
	}
	int64_t Parallel::getTaskCount(){
		return m_node_list->size();
	}
	bool Parallel::run(Object* context){
		// reset
		m_remain_count =0;
		m_error_count =0;

		// check env
		Coroutine* cr =Coroutine::Running();
		if(!cr){
			FATAL("Parallel run failed, is not in coroutine.");
			return false;
		}

		// prepare
		CoroutineService* service =dynamic_cast< CoroutineService* >(Service::Current());
		ASSERT(service);
		CoroutinePool* crp =service->getCoroutinePool();
		ASSERT(crp);
		if(crp->isCleaning()){
			WARN("Parallel run failed, coroutine pool is cleaning.");
			return false;
		}

		const int64_t n =m_node_list->size();
		m_remain_count =n;
		if(n == 0){
			return true;
		}

		// set context
		ASSIGN_POINTER(m_context, context);

		// go
		int64_t cr_id =0;
		for(int64_t i=0; i<n; ++i){
			ParallelNode* node =static_cast< ParallelNode* >(m_node_list->get(i));
			const int64_t result =crp->go(ParallelNode::Invoke, node, cr_id);
			ASSERT(result >= 0);
		}

		// post process
		if(m_remain_count > 0){
			// DEBUG("parallel yield, m_remain_count > 0");
			m_cr =cr;
			ASSERT(m_cr->yield(0));
			// DEBUG("parallel done");
		}
		ASSERT(0 == m_remain_count);
		CLEAN_POINTER(m_context);
		m_cr =0;
		return 0 == m_error_count;
	}
	int64_t Parallel::getRemainCount(){
		return m_remain_count;
	}
	int64_t Parallel::getErrorCount(){
		return m_error_count;
	}
	void Parallel::decrRemainCount(){
		--m_remain_count;
	}
	void Parallel::incrErrorCount(){
		++m_error_count;
	}
	Object* Parallel::getContext(){
		return m_context;
	}
	void Parallel::resume(){
		if(0==m_remain_count && m_cr && m_cr->isWaiting()){
			CoroutineService* service =dynamic_cast< CoroutineService* >(Service::Current());
			ASSERT(service);
			CoroutinePool* crp =service->getCoroutinePool();
			ASSERT(crp);
			crp->resume(m_cr->getId(), 0);
		}
	}
}
