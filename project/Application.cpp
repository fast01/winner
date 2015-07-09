#include "main.h"

namespace project{
	/*** impl Application ***/
	/** ctor & dtor **/
	Application::Application(){
	}
	Application::~Application(){
	}

	/** Object **/
	void Application::init(){
		Super::init();
	}
	void Application::finalize(){
		Super::finalize();
	}
	/** load unload **/
	bool Application::on_load(const int argc, const char** argv){
		return Super::on_load(argc, argv);
	}
	void Application::on_unload(){
		Super::on_unload();
	}
	/** register **/
	void Application::register_misc(){
		Super::register_misc();
	}
	void Application::register_lua(){
		Super::register_lua();
		core::ProcessLocal::Instance()->registerLuaInitCallback(::cpplua::register_cpplua);
	}
	void Application::register_protocol(){
		Super::register_protocol();
		::protocol::register_protocol();
	}
	void Application::register_class(){
		Super::register_class();
		::service::register_service();
	}
}
