/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_APPLICATION_BASE_H__
#define H_CORE_APPLICATION_BASE_H__

namespace core{
	/** ApplicationBase **/
	class ApplicationBase: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	protected:
		ApplicationBase();
		virtual ~ApplicationBase();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void run(const int argc, const char** argv);
		void quit();
	protected:
		virtual bool on_load(const int argc, const char** argv);
		virtual void on_unload();
	protected:
		virtual void register_misc(); 
		virtual void register_lua(); 
		virtual void register_protocol(); 
		virtual void register_class(); 
	public:
		void setDataPath(String* path);
		String* getDataPath();
		String* getRootPath();
		String* getBinPath();
	private:
		const char* _config_path(const char* sub_path);
	private:
		String* m_root_path;
		String* m_bin_path;
		String* m_data_path;
	};
}

#endif
