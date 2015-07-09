/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LOGGER_H__
#define H_CORE_LOGGER_H__
#include <stdlib.h>
namespace core{
	/** Logger **/
	class Logger: public Object{
		typedef Object Super;
		DECLARE_THREAD_LOCAL_SINGLETON(Logger)
	public:
		enum{
			DEBUG, INFO, WARN, ERROR, FATAL,
			USER_MIN_LEVEL,
			USER_MAX_LEVEL =1000000
		};
		enum{
			SHOW_ALL =0,
			SHOW_NONE =USER_MAX_LEVEL + 1
		};
		enum{
			FLUSH_MODE_AUTO =0,
			FLUSH_MODE_FLUSH =1,
			FLUSH_MODE_SYNC =2,
			FLUSH_MODE_DEFAULT =FLUSH_MODE_AUTO
		};
		enum{
			FLUSH_TIMER =30, // 30 secs
			FLUSH_THRESHOLD =5
		};
	public:
		Logger();
		virtual ~Logger();
	public:
		virtual void finalize();
	public:
		void setName(String* name);
		String* getName();
		void setLevel(const int64_t lv);
		int64_t getLevel();
		void setFileLifeTime(const int64_t life_time);
		int64_t getFileLifeTime();
		void setFlushMode(const int64_t mode);
		int64_t getFlushMode();
	public:
		void printv(const char* fmt, va_list vl);
		void printf(const char* fmt, ...);
	public:
		void log(const int64_t lv, const char* fmt, ...);
		void logv(const int64_t lv, const char* fmt, va_list vl);
		void info(const char* fmt, ...);
		void warn(const char* fmt, ...);
		void error(const char* fmt, ...);
		void fatal(const char* fmt, ...);
		void debug(const char* fmt, ...);
	public:
		static void Printf(String* str);
		static void Log(const int64_t lv, String* str);
		static void Info(String* str);
		static void Warn(String* str);
		static void Error(String* str);
		static void Fatal(String* str);
		static void Debug(String* str);

		static void SetName(String* name);
		static String* GetName();
		static void SetLevel(const int64_t lv);
		static int64_t GetLevel();
		static void SetFileLifeTime(const int64_t life_time);
		static int64_t GetFileLifeTime();
		static void SetFlushMode(const int64_t mode);
		static int64_t GetFlushMode();
	private:
		void _open_file();
	private:
		int64_t m_level;
		String* m_szName;
		FILE* m_file;
		int64_t m_file_open_time;
		int64_t m_file_life_time;
		int64_t m_flush_mode;
		int64_t m_flush_last_time;
		int64_t m_log_count_before_flush;
	};
}
#endif
