/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ACTOR_MANAGER_H__
#define H_CORE_ACTOR_MANAGER_H__

namespace core{
	/** ActorManager  **/
	class ActorManager: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
		DECLARE_THREAD_LOCAL_SINGLETON(ActorManager)
	public:
		typedef Actor* (*PFN_CREATE_ACTOR)();
	protected:
		ActorManager();
		virtual ~ActorManager();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void update(const int64_t now);
	public:
		Actor* create(String* templ_name, const int64_t id);
		Actor* get(const int64_t id);
		void remove(const int64_t id);
	public:
		void forEach(void (*pfn)(Object*, void*), void* userdata);
		void removeIf(bool (*pfn)(Object*, Object*));
	public:
		bool load(const char* path);
	private:
		Hash* m_actor_tb;
		Hash* m_template_tb;
	};
}

#endif
