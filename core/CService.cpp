/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** CService impl ***/
	/** ctor & dtor **/
	CService::CService()
		: CommandService()
		, m_dl(0)
		, m_on_load(0)
		, m_on_update(0)
		, m_on_start_command(0)
		, m_on_unload(0){
	}
	CService::~CService(){
	}

	/* Service */
	bool CService::load_module(const int64_t id, const char* path){
		if(!Super::load_module(id, path)) return false;
		if(!path) return false;
		m_id =id;

		// load dll
		m_dl =::dlopen(path, RTLD_NOW | RTLD_GLOBAL);
		if(m_dl == 0){
			const char* err =::dlerror();
			ERROR("fail to call dlopen <%s>, %s", path, err ? err : "no error");
			return false;
		}

		// object pool manager
		OPH();

		// set path
		String* p =String::NewString(path);
		ASSIGN_POINTER(m_path, p);

		// get name
		{
			const char* (*fn_get_name)() =(const char* (*)())::dlsym(m_dl, "get_name");
			if(!fn_get_name){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, get_name>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
			const char* name =fn_get_name();
			if(!name) name ="unnamed";
			CLEAN_POINTER(m_name);
			m_name =String::NewString(name);
			m_name->retain();
		}

		// get desc
		{
			const char* (*fn_get_desc)() =(const char* (*)())::dlsym(m_dl, "get_desc");
			if(!fn_get_desc){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, get_desc>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
			const char* desc =fn_get_desc();
			if(!desc) desc ="";
			CLEAN_POINTER(m_desc);
			m_desc =String::NewString(desc);
			m_desc->retain();
		}

		// get on_load
		{
			m_on_load =(PFN_ON_LOAD)::dlsym(m_dl, "on_load");
			if(!m_on_load){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, on_load>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
		}

		// get on_update
		{
			m_on_update =(PFN_ON_UPDATE)::dlsym(m_dl, "on_update");
			if(!m_on_update){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, on_update>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
		}

		// get on_start_command
		{
			m_on_start_command =(PFN_ON_START_COMMAND)::dlsym(m_dl, "on_start_command");
			if(!m_on_start_command){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, on_start_command>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
		}

		// get on_unload
		{
			m_on_unload =(PFN_ON_UNLOAD)::dlsym(m_dl, "on_unload");
			if(!m_on_unload){
				const char* err =::dlerror();
				ERROR("fail to call dlsym <%s, on_unload>, %s", path, err ? err : "no error");
				unload_module();
				return false;
			}
		}

		return true;
	}
	void CService::unload_module(){
		m_on_load =0;
		m_on_update =0;
		m_on_start_command =0;
		m_on_unload =0;
		if(m_dl){
			if(0 != ::dlclose(m_dl)){
				ERROR("fail to call dlclose for %s", m_path->c_str());	
			}
			m_dl =0;
		}
		Super::unload_module();
	}
	bool CService::on_load(){
		if(!Super::on_load()) return false;
		if(m_on_load){
			return 0 == m_on_load();
		}
		return false;
	}
	void CService::on_update(const int64_t now){
		Super::on_update(now);
		if(m_on_update) m_on_update(now);	
	}
	int64_t CService::on_start_command(Command* command){
		Super::on_start_command(command);
		return m_on_start_command ? m_on_start_command(command) : Command::STATE_ERROR;	
	}
	void CService::on_unload(){
		if(m_on_unload) m_on_unload();	
		Super::on_unload();
	}
}
