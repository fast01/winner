/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** ActorManager impl ***/
	/*** singleton ***/
	DEFINE_THREAD_LOCAL_SINGLETON(ActorManager)

	/** ctor & dtor **/
	ActorManager::ActorManager()
		: m_actor_tb(0)
		, m_template_tb(0){
	}
	ActorManager::~ActorManager(){
	}

	/** ctor & dtor **/
	void ActorManager::init(){
		Super::init();
		m_actor_tb =New<Hash>();
		m_actor_tb->retain();

		m_template_tb =New<Hash>();
		m_template_tb->retain();
	}
	void ActorManager::finalize(){
		CLEAN_POINTER(m_actor_tb);
		CLEAN_POINTER(m_template_tb);
		Super::finalize();
	}

	/** self **/
	void ActorManager::update(const int64_t now){
		return;
	}
	Actor* ActorManager::create(String* templ_name, const int64_t id){
		if(m_actor_tb->has(id)){
			ERROR("actor %lld already exists", (long long)id);
			return 0;
		}
		if(!templ_name){
			return 0;
		}
		if(protocol::ActorConfig* templ =dynamic_cast< protocol::ActorConfig* >(m_template_tb->get(templ_name))){
			// new actor
			if(Actor* actor =static_cast< Actor* >(ObjectFactory::Create(templ->getClassName()))){
				m_actor_tb->set(id, actor);

				// add observer
				const int64_t observer_cnt =templ->getObserverCount();
				for(int64_t i=0; i<observer_cnt; ++i){
					if(Observer* observer =static_cast< Observer* >(ObjectFactory::Create(templ->getObserver(i)))){
						actor->addObserver(observer);
					}
				}
				
				// add component
				const int64_t component_cnt =templ->getComponentCount();
				for(int64_t i=0; i<component_cnt; ++i){
					if(Component* com =static_cast< Component* >(ObjectFactory::Create(templ->getComponent(i)))){
						actor->addComponent(com);
					}
				}
				return actor;
			}
			else{
				return 0;
			}
		}
		else{
			ERROR("actor template %s not registered", templ_name->c_str());
			return 0;
		}
	}
	Actor* ActorManager::get(const int64_t id){
		return static_cast< Actor* >(m_actor_tb->get(id));
	}
	void ActorManager::remove(const int64_t id){
		m_actor_tb->remove(id);
	}
	void ActorManager::forEach(void (*pfn)(Object*, void*), void* userdata){
		if(!pfn) return;
		HashIterator* it =static_cast< HashIterator* >(m_actor_tb->iterator());
		while(it->next()){
			pfn(it->getValue(), userdata);
		}
	}
	void ActorManager::removeIf(bool (*pfn)(Object*, Object*)){
		m_actor_tb->removeIf(pfn);
	}
	bool ActorManager::load(const char* path){
		// load config
		protocol::ActorManagerConfig* cfg =(protocol::ActorManagerConfig*)protocol::create_protocol_from_lua_file(protocol::PROTOCOL_ACTOR_MANAGER_CONFIG, path);
		if(!cfg){
			DEBUG("init actor manager fail, cfg load error");
			return false;
		}
		// dump config
		Bytes* str =SafeNew<Bytes>();
		cfg->appendToString(str, 0);
		DEBUG(str->toString()->c_str());

		// process
		const int64_t n =cfg->getActorCount();
		for(int64_t i=0; i<n; ++i){
			protocol::ActorConfig* ac =cfg->getActor(i);
			m_template_tb->set(ac->getTemplateName(), ac);
		}
		return true;
	}
}
