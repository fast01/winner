/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PROCESS_LOCAL_H__
#define H_CORE_PROCESS_LOCAL_H__

namespace core{
	/** ProcessLocal **/
	class ProcessLocal: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
	public:
		typedef bool (*PFN_LUA_INIT_CALLBACK)(lua_State*);
	protected:
		ProcessLocal();
		virtual ~ProcessLocal();
	public:
		virtual void init();
		virtual void finalize();
	public:
		bool update();
	public:
		static void Init();
		static ProcessLocal* Instance();
		static void Release();
	public:
		void registerLuaInitCallback(PFN_LUA_INIT_CALLBACK);
		bool initLua(lua_State* L);
	public:
		ObjectFactory* getObjectFactory();
		StringTable* getStringTable();
		ProtocolManager* getProtocolManager();

		DispatcherManager* getDispatcherManager();
		Network* getNetwork();
		ServiceManager* getServiceManager();
	private:
		ObjectFactory* m_object_factory;
		StringTable* m_string_table;
		ProtocolManager* m_protocol_manager;
		Array* m_lua_init_callback;

		DispatcherManager* m_dispatcher_manager;
		Network* m_network;
		ServiceManager* m_service_manager;
	};
}

#endif
