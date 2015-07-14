/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	///*** ProcessLocal Impl ***///
	//** ctor & dtor **//
	ProcessLocal::ProcessLocal()
		: m_object_factory(0)
		, m_string_table(0)
		, m_protocol_manager(0)
		, m_lua_init_callback(0)
		, m_dispatcher_manager(0)
		, m_network(0)
		, m_service_manager(0){
	}
	ProcessLocal::~ProcessLocal(){
		mysql_library_end();
	}

	/** Object **/
	void ProcessLocal::init(){
		OPH();
		Super::init();
		// basic layer
		m_object_factory =New<ObjectFactory>();
		RETAIN_POINTER(m_object_factory);

		m_string_table =New<StringTable>();
		RETAIN_POINTER(m_string_table);

		m_protocol_manager =New<ProtocolManager>();
		RETAIN_POINTER(m_protocol_manager);

		m_lua_init_callback =New<Array>();
		RETAIN_POINTER(m_lua_init_callback);

		// kernel layer
		m_dispatcher_manager =New<DispatcherManager>();
		RETAIN_POINTER(m_dispatcher_manager);

		m_network =New<Network>();
		RETAIN_POINTER(m_network);

		m_service_manager =New<ServiceManager>();
		RETAIN_POINTER(m_service_manager);

		// add dispatcher
		m_dispatcher_manager->addDispatcher(m_service_manager);
		m_dispatcher_manager->addDispatcher(m_network);
		OutputDebug("process local init [ ok ]");
	}
	void ProcessLocal::finalize(){
		OPH();
		// clear dispatcher
		m_dispatcher_manager->clearDispatcher();

		CLEAN_POINTER(m_service_manager);
		CLEAN_POINTER(m_network);
		CLEAN_POINTER(m_dispatcher_manager);

		CLEAN_POINTER(m_lua_init_callback);
		CLEAN_POINTER(m_protocol_manager);
		CLEAN_POINTER(m_string_table);
		CLEAN_POINTER(m_object_factory);

		Super::finalize();
		OutputDebug("process local released [ ok ]");
	}

	/** update **/
	bool ProcessLocal::update(){
		OPH();
		const int64_t now =DateTime::Now();
		m_service_manager->update(now);
		return m_network->update(now);
	}

	/** Instance **/
	static ProcessLocal* g_process_local =0;
	void ProcessLocal::Init(){
		ASSERT(g_process_local == 0);
		g_process_local =new ProcessLocal();
		g_process_local->init();
		RETAIN_POINTER(g_process_local);
	}
	ProcessLocal* ProcessLocal::Instance(){
		return g_process_local;
	}
	void ProcessLocal::Release(){
		ASSERT(g_process_local->getRefCount() == 1);
		g_process_local->finalize();
		delete g_process_local;
		g_process_local=0;
	}
	/** lua register **/
	void ProcessLocal::registerLuaInitCallback(PFN_LUA_INIT_CALLBACK pfn){
		Pointer* pt =New<Pointer>();
		pt->setValue(reinterpret_cast< void* >(pfn));
		m_lua_init_callback->push_back(pt);
	}
	bool ProcessLocal::initLua(lua_State* L){
		// register path
		ApplicationBase* app =ApplicationBase::Instance();
		lua_pushstring(L, app->getRootPath()->c_str());
		lua_setglobal(L, "ROOT_PATH");
		lua_pushstring(L, app->getBinPath()->c_str());
		lua_setglobal(L, "BIN_PATH");
		lua_pushstring(L, app->getDataPath()->c_str());
		lua_setglobal(L, "DATA_PATH");

		// call register
		const int64_t cnt =m_lua_init_callback->size();
		for(int64_t i=0; i<cnt; ++i){
			Pointer* pt =dynamic_cast< Pointer* >(m_lua_init_callback->get(i));
			PFN_LUA_INIT_CALLBACK pfn =reinterpret_cast<PFN_LUA_INIT_CALLBACK>(pt->getValue());
			if(!pfn(L)){
				return false;
			}
		}
		return true;
	}

	/** basic **/
	ObjectFactory* ProcessLocal::getObjectFactory(){
		return m_object_factory;
	}
	StringTable* ProcessLocal::getStringTable(){
		return m_string_table;
	}
	ProtocolManager* ProcessLocal::getProtocolManager(){
		return m_protocol_manager;
	}

	/** engine **/
	DispatcherManager* ProcessLocal::getDispatcherManager(){
		return m_dispatcher_manager;
	}
	Network* ProcessLocal::getNetwork(){
		return m_network;
	}
	ServiceManager* ProcessLocal::getServiceManager(){
		return m_service_manager;
	}
}
