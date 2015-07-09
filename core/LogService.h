/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LOG_SERVICE_H__
#define H_CORE_LOG_SERVICE_H__

namespace core{
	/** LogService **/
	class LogService : public Service{
		typedef Service Super;
		SUPPORT_NEWABLE
		DECLARE_CLASS_INFO
		enum{
			FLUSH_TIMER = 1 // 300 secs
		};
		enum{
			FLAG_AUTO =0,
			FLAG_FLUSH =1,
			FLAG_SYNC =2,
			FLAG_DEFAULT =FLAG_AUTO,
		};
	public:
		enum{
			SERVICE_ID = 4,
		};
		enum{
			SERVICE_LOG_NOTIFY = 17,
		};
	protected:
		LogService();
		virtual ~LogService();
	protected:
		virtual bool on_load();
		virtual void on_update(const int64_t now);
		virtual void on_unload();
		virtual void on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
	private:
		void _log(const int64_t type, const int64_t flag, String* content);
		bool _load(const char* path);
	public:
		static bool Log(const int64_t type, const int64_t flag, String* content);
		static bool Log(const int64_t type, const int64_t flag, const char* fmt, ...);
	private:
		static int _Log(lua_State* L);
	public:
		static bool RegisterToLua(lua_State* L);
	private:
		Hash* m_logger_table;
	};
}

#endif
