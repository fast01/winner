/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl ApplicationBase ***/
	/** ctor & dtor **/
	ApplicationBase::ApplicationBase()
		: m_root_path(0)
		, m_bin_path(0)
		, m_data_path(0){
	}
	ApplicationBase::~ApplicationBase(){
	}

	/** Object **/
	void ApplicationBase::init(){
		Super::init();
		// basic
		::init_platform();
		ObjectPool::Set(New<ObjectPool>());

		// misc
		{
			OPH();

			// logger & monitor
			Logger::Init();
			Monitor::Init();

			// process local
			ProcessLocal::Init();

			// register
			register_misc();
			register_lua();
			register_protocol();
			register_class();

			// observer & actor
			ObserverManager::Init();
			ActorManager::Init();
		}
	}
	void ApplicationBase::finalize(){
		// misc
		{
			OPH();

			// actor & observer
			ActorManager::Release();
			ObserverManager::Release();

			// process local
			ProcessLocal::Release();

			// monitor & logger
			Monitor::Release();
			Logger::Release();

			// path
			setDataPath(0);
		}

		// basic
		ObjectPool::Release();
		::clean_platform();

		Super::finalize();
	}
	/** instance **/
	static ApplicationBase* g_app =0;
	void ApplicationBase::SetInstance(ApplicationBase* app){
		g_app =app;
	}
	ApplicationBase* ApplicationBase::Instance(){
		return g_app;
	}
	/** self **/
	void ApplicationBase::run(const int argc, const char** argv){
		OPH();
		// on load
		if(!on_load(argc, argv)){
			ERROR("Core load failed");
			return;
		}

		// run
		ProcessLocal* process =ProcessLocal::Instance();
		while(process->update());

		// on unload
		on_unload();
	}
	void ApplicationBase::quit(){
		Network::Close();
	}
	/** load unload **/
	bool ApplicationBase::on_load(const int argc, const char** argv){
		OPH();
		// prepare path
		if(argc > 1){
			INFO("data path is %s", argv[1]);
			setDataPath(STR(argv[1]));
		}
		else{
			INFO("data path is ../data/");
			setDataPath(STR("../data/"));
		}
		// load
		core::Network* net =core::Network::Instance();
		bool ok =net->load(_config_path("startup/network.lua"));
		CHECK_EXIT(ok && "load network failed", 1);

		ok =ServiceManager::Instance()->load(_config_path("startup/service_manager.lua"));
		CHECK_EXIT(ok && "load service manager failed", 1);

		ok =ActorManager::Instance()->load(_config_path("startup/actor_manager.lua"));
		CHECK_EXIT(ok && "load actor manager failed", 1);

		return ok;
	}
	void ApplicationBase::on_unload(){
	}
	/** register lua **/
	void ApplicationBase::register_misc(){
		OPH();
		Base64::Init();
	}
	void ApplicationBase::register_lua(){
		OPH();
		ProcessLocal* pl =ProcessLocal::Instance();
		pl->registerLuaInitCallback(Requestor::RegisterToLua);
		pl->registerLuaInitCallback(ProtocolBase::RegisterToLua);
		pl->registerLuaInitCallback(ProtocolManager::RegisterToLua);
		pl->registerLuaInitCallback(Md5::RegisterToLua);
		pl->registerLuaInitCallback(Sha::RegisterToLua);
		pl->registerLuaInitCallback(Base64::RegisterToLua);
		pl->registerLuaInitCallback(XmlForLua::RegisterToLua);
		pl->registerLuaInitCallback(JsonForLua::RegisterToLua);
		pl->registerLuaInitCallback(UrlEncode::RegisterToLua);
		pl->registerLuaInitCallback(CharsetConvert::RegisterToLua);
		pl->registerLuaInitCallback(MysqlForLua::RegisterToLua);
		pl->registerLuaInitCallback(DispatcherManager::RegisterToLua);
		pl->registerLuaInitCallback(LogService::RegisterToLua);
		pl->registerLuaInitCallback(core::cpplua::register_cpplua);
		// pl->registerLuaInitCallback(core::data::register_cpplua);
		// pl->registerLuaInitCallback(core::protocol::register_cpplua);
	}
	void ApplicationBase::register_protocol(){
		OPH();
		core::protocol::register_protocol();
	}
	void ApplicationBase::register_class(){
		OPH();
		LogService::RegisterClass();
		TcpListener::RegisterClass();
		HttpListener::RegisterClass();
		HttpService::RegisterClass();
	}
	void ApplicationBase::setDataPath(String* path){
		// root
		ASSIGN_POINTER(m_data_path, path);

		// bin & data
		if(m_data_path){
			ASSIGN_POINTER(m_root_path, FileSystem::Join(m_data_path, STR("../")));
			ASSIGN_POINTER(m_bin_path, FileSystem::Join(m_root_path, STR("bin/")));

			INFO("data path is '%s'", m_data_path->c_str());
			INFO("root path is '%s'", m_root_path->c_str());
			INFO("bin path is '%s'", m_bin_path->c_str());
		}
		else{
			CLEAN_POINTER(m_root_path);
			CLEAN_POINTER(m_bin_path);
		}
	}
	String* ApplicationBase::getDataPath(){
		return m_data_path;
	}
	String* ApplicationBase::getRootPath(){
		return m_root_path;
	}
	String* ApplicationBase::getBinPath(){
		return m_bin_path;
	}
	String* ApplicationBase::getConfigPath(const char* sub_path){
		ASSERT(m_data_path);
		ASSERT(sub_path && strlen(sub_path)>0 && sub_path[0]!='/');
		String* cfg_path =FileSystem::Join(m_data_path, STR(sub_path));
		return cfg_path;
	}
	/** private **/
	const char* ApplicationBase::_config_path(const char* sub_path){
		return getConfigPath(sub_path)->c_str();
	}
}
