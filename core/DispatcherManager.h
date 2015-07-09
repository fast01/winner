/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_DISPATCHER_MANAGER_H__
#define H_CORE_DISPATCHER_MANAGER_H__

namespace core{
	/** DispatcherManager **/
	class DispatcherManager: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
		DECLARE_PROCESS_LOCAL_SINGLETON(DispatcherManager)
	private:
		DispatcherManager();
		virtual ~DispatcherManager();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void addDispatcher(Dispatcher* dispatcher);
		void removeDispatcher(Dispatcher* dispatcher);
		void clearDispatcher();
	public:
		bool request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
		bool requestByObject(Requestor* requestor, PACKET& packet, Object* obj);
	public:
		bool notify(PACKET& packet, void* body, const int64_t body_len);
		bool notifyByObject(PACKET& packet, Object* obj);
	public:
		bool reply(PACKET& packet, void* body, const int64_t body_len);
		bool replyByObject(PACKET& packet, Object* obj);
	public:
		bool isMatch(const int64_t id);
	public:
		static bool Request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
		static bool RequestByObject(Requestor* requestor, PACKET& packet, Object* obj);
	public:
		static bool Notify(PACKET& packet, void* body, const int64_t body_len);
		static bool NotifyByObject(PACKET& packet, Object* obj);
	public:
		static bool Reply(PACKET& packet, void* body, const int64_t body_len);
		static bool ReplyByObject(PACKET& packet, Object* obj);
	public:
		static bool RegisterToLua(lua_State* L);
	private:
		static int _Request(lua_State* L);
		static int _RequestByObject(lua_State* L);
		static int _Notify(lua_State* L);
		static int _NotifyByObject(lua_State* L);
	private:
		Dispatcher* _match(const int64_t id);
	private:
		Array* m_dispatcher_list;

		typedef DEFAULT_LOCK_TYPE LOCK_TYPE;
		LOCK_TYPE m_lock;
	};
}

#endif
