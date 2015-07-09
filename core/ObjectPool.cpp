/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/** ctor & dtor **/
	ObjectPool::ObjectPool()
		: m_pObjStack(0){
	}
	ObjectPool::~ObjectPool(){
	}

	/** init & finalize **/
	void ObjectPool::init(){
		Object::init();
		m_pObjStack =New<Array>();
		m_pObjStack->retain();
	}
	void ObjectPool::finalize(){
		if(m_pObjStack && m_pObjStack->size()>0){
			const int64_t n =m_pObjStack->size();
			OutputDebug("!!!!!!!!!!!!! ObjectPool remain count is %lld !!!!!!!!!!!!!", (long long)n);
			for(int64_t i=0; i<n; ++i){
				if(const char* name =m_pObjStack->get(i)->typeName()){
					OutputDebug("!!!!!!!!!!!!! ObjectPool leak %s %p !!!!!!!!!!!!!", name, m_pObjStack->at(i));
				}
			}
		}
		CLEAN_POINTER(m_pObjStack);
		Object::finalize();
	}

	/** push & save & restore **/
	void ObjectPool::push(Object* obj){
		if(obj){
			m_pObjStack->push_back(obj);
		}
		if(getCount() > 10000){
			WARN("############### object pool need optimize %d ###############", (int)getCount());
		}
	}
	int64_t ObjectPool::getCount(){
		return m_pObjStack->size();
	}
	void ObjectPool::restore(const int64_t count){
		while(m_pObjStack->size() > count){
			m_pObjStack->pop_back();
		}
	}	

	/** thread local singlton **/
	static thread_local ObjectPool* g_object_pool =0;
	ObjectPool* ObjectPool::Instance(){
		return g_object_pool;
	}
	void ObjectPool::Set(ObjectPool* pool){
		// DEBUG("%s object pool change: %p => %p", dynamic_cast< CoroutineService* >(Service::Current())?"***":"", g_object_pool, pool);
		ASSIGN_POINTER(g_object_pool, pool);
	}
	void ObjectPool::Release(){
		ASSERT(g_object_pool && g_object_pool->getRefCount()==1);
		CLEAN_POINTER(g_object_pool);
	}

	/** ObjectPool **/
	ObjectPoolHelper::ObjectPoolHelper()
		: m_object_pool(ObjectPool::Instance()){
			m_iCount =m_object_pool->getCount();
	}
	ObjectPoolHelper::~ObjectPoolHelper(){
		m_object_pool->restore(m_iCount);
	}
}
