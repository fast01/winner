/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_C_SERVICE_H__
#define H_CORE_C_SERVICE_H__

namespace core{
	/** CService **/
	class CService: public CommandService {
		typedef CommandService Super;
		typedef int (*PFN_ON_LOAD)();
		typedef void (*PFN_ON_UPDATE)(const int64_t now);
		typedef int (*PFN_ON_START_COMMAND)(Command*);
		typedef int (*PFN_ON_RESUME_COMMAND)(Command*, Command*);
		typedef int (*PFN_ON_RESUME_RPC)(Command*);
		typedef int (*PFN_CANCEL_COMMAND)(Command*);
		typedef void (*PFN_ON_UNLOAD)();
	public:
		CService();
		virtual ~CService();
	protected:
		virtual bool load_module(const int64_t id, const char* path);
		virtual void unload_module();
		virtual bool on_load();
		virtual void on_update(const int64_t now);
		virtual int64_t on_start_command(Command* command);
		virtual void on_unload();
	private:
		void* m_dl;
		PFN_ON_LOAD m_on_load;
		PFN_ON_UPDATE m_on_update;
		PFN_ON_START_COMMAND m_on_start_command;
		PFN_ON_UNLOAD m_on_unload;
	};
}

#endif
