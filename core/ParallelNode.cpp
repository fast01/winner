/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl ParallelNode ***/
	/** ctor & dtor **/
	ParallelNode::ParallelNode(PFUNC fn, Parallel* parallel)
		: m_fn(fn)
		, m_parallel(parallel){
		ASSERT(m_fn && m_parallel);
	}
	ParallelNode::~ParallelNode(){
	}

	/** Object **/
	void ParallelNode::init(){
		Super::init();
	}
	void ParallelNode::finalize(){
		Super::finalize();
	}

	/** self **/
	void ParallelNode::Invoke(Object* arg){
		// prepare
		ParallelNode* self =dynamic_cast< ParallelNode* >(arg);
		ASSERT(self);
		Parallel* parallel =self->m_parallel;
		Object* context =parallel->getContext();

//		DEBUG("task %p %lld", (void*)Coroutine::Running(), Coroutine::Running()->getId());
		
		// invoke
		const bool ok =self->m_fn(context);

		// post process
		parallel->decrRemainCount();
		if(!ok){
			parallel->incrErrorCount();
		}
		parallel->resume();
	}
}
