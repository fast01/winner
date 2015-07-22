/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/** singleton **/
	DEFINE_THREAD_LOCAL_SINGLETON(Logger)

	/** ctor & dtor **/
	Logger::Logger()
		: m_level(Logger::SHOW_NONE)
		, m_szName(0)
		, m_file(0)
		, m_file_open_time(0)
		, m_file_life_time(24 * 3600)
		, m_flush_mode((int64_t)FLUSH_MODE_DEFAULT)
		, m_flush_last_time(0)
		, m_log_count_before_flush(0){
	}
	Logger::~Logger(){
		if(m_file){
			fclose(m_file);
			m_file =0;
		}
	}

	/** object **/
	void Logger::finalize(){
		CLEAN_POINTER(m_szName);
		if(m_file){
			fclose(m_file);
			m_file =0;
		}
		Super::finalize();
	}

	/** query **/
	void Logger::setName(String* name){
		ASSIGN_POINTER(m_szName, name);
		if(m_file){
			fclose(m_file);
			m_file =0;
			m_file_open_time =0;
		}
		if(m_szName){
			_open_file();
		}
	}
	String* Logger::getName(){
		return m_szName;
	}
	void Logger::setLevel(const int64_t lv){
		m_level =lv;
	}
	int64_t Logger::getLevel(){
		return m_level;
	}
	void Logger::setFileLifeTime(const int64_t life_time){
		m_file_life_time =life_time;
		if(m_file_life_time < 3600){
			m_file_life_time =3600;
		}
	}
	int64_t Logger::getFileLifeTime(){
		return m_file_life_time;
	}

	void Logger::setFlushMode(const int64_t mode){
		if(m_flush_mode != mode){
			m_flush_mode =mode;
		}
	}
	int64_t Logger::getFlushMode(){
		return m_flush_mode;
	}

	void Logger::printv(const char* fmt, va_list vl){
		// prepare out
		FILE* out =stdout;
		if(m_szName){
			_open_file();
			if(m_file){
				out =m_file;
			}
		}
		// out
		vfprintf(out, fmt, vl);

		// try flush
		if((FLUSH_MODE_FLUSH==m_flush_mode) || (m_log_count_before_flush>=FLUSH_THRESHOLD)){
			fflush(out);
			m_flush_last_time =DateTime::Now();
			m_log_count_before_flush =0;
		}
		else{
			const int64_t now =DateTime::Now();
			if(now-m_flush_last_time >= FLUSH_TIMER){
				fflush(out);
				m_flush_last_time =now;
				m_log_count_before_flush =0;
			}
			else{
				++m_log_count_before_flush;
			}
		}
		ASSERT(out);
	}
	void Logger::printf(const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		printv(fmt, vl);
		va_end(vl);
	}
	void Logger::log(const int64_t lv, const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		logv(lv, fmt, vl);
		va_end(vl);
	}
	void Logger::logv(const int64_t lv, const char* fmt, va_list vl){
		if(lv < m_level) return;
		// prepare out
		FILE* out =stdout;
		if(m_szName){
			_open_file();
			if(m_file){
				out =m_file;
			}
		}

		// tm
		struct tm t;
		int microsecs =0;
		struct timeval tv;
		if(0 == gettimeofday(&tv, 0)){
			if(struct tm* tmp =localtime_r(&tv.tv_sec, &t)){
				t =*tmp;
				microsecs =tv.tv_usec;
			}
			else{
				memset(&t, 0, sizeof(t));
			}
		}
		else{
			memset(&t, 0, sizeof(t));
		}

		// out
		const char* tag =0;
		switch(lv){
		case Logger::INFO:
			tag ="INFO "; break;
		case Logger::WARN:
			tag ="WARN "; break;
		case Logger::ERROR:
			tag ="ERROR"; break;
		case Logger::FATAL:
			tag ="FATAL"; break;
		case Logger::DEBUG:
			tag ="DEBUG"; break;
		}
		if(tag){
			fprintf(out, "[%04d-%02d-%02d %02d:%02d:%02d.%06d] %s > ", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, microsecs, tag);
		}
		else{
			fprintf(out, "[%04d-%02d-%02d %02d:%02d:%02d.%06d] %.5lld > ", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, microsecs, (long long)lv);
		}

		/*
		char x[1024] ={0};
		vsprintf(x, fmt, vl);
		if(strcmp(x, "fail to call a1, arg test invalid") == 0){
			ASSERT(0);
		}
		*/
		vfprintf(out, fmt, vl);
		fprintf(out, "\n");
		if(FLUSH_MODE_FLUSH == m_flush_mode){
			fflush(out);
		}
	}
	void Logger::info(const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		logv(Logger::INFO, fmt, vl);
		va_end(vl);
	}
	void Logger::warn(const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		logv(Logger::WARN, fmt, vl);
		va_end(vl);
	}
	void Logger::error(const char* fmt, ...){
		// print_stack();
		va_list vl;
		va_start(vl, fmt);
		logv(Logger::ERROR, fmt, vl);
		va_end(vl);
	}
	void Logger::fatal(const char* fmt, ...){
		print_stack();
		va_list vl;
		va_start(vl, fmt);
		logv(Logger::FATAL, fmt, vl);
		va_end(vl);
		abort();
	}
	void Logger::debug(const char* fmt, ...){
		va_list vl;
		va_start(vl, fmt);
		logv(Logger::DEBUG, fmt, vl);
		va_end(vl);
	}

	/** static **/
	void Logger::Printf(String* str){
		if(str){
			PRINTF(str->c_str());
		}
	}
	void Logger::Log(const int64_t lv, String* str){
		if(str){
			LOG(lv, str->c_str());
		}
	}
	void Logger::Info(String* str){
		if(str){
			INFO(str->c_str());
		}
	}
	void Logger::Warn(String* str){
		if(str){
			WARN(str->c_str());
		}
	}
	void Logger::Error(String* str){
		if(str){
			ERROR(str->c_str());
		}
	}
	void Logger::Fatal(String* str){
		if(str){
			FATAL(str->c_str());
		}
	}
	void Logger::Debug(String* str){
		if(str){
			DEBUG(str->c_str());
		}
	}
	
	void Logger::SetName(String* name){
		Instance()->setName(name);
	}
	String* Logger::GetName(){
		return Instance()->getName();
	}
	void Logger::SetLevel(const int64_t lv){
		Instance()->setLevel(lv);
	}
	int64_t Logger::GetLevel(){
		return Instance()->getLevel();
	}
	void Logger::SetFileLifeTime(const int64_t life_time){
		Instance()->setFileLifeTime(life_time);
	}
	int64_t Logger::GetFileLifeTime(){
		return Instance()->GetFileLifeTime();
	}
	void Logger::SetFlushMode(const int64_t mode){
		Instance()->setFlushMode(mode);
	}
	int64_t Logger::GetFlushMode(){
		return Instance()->getFlushMode();
	}

	/** private **/
	void Logger::_open_file(){
		// check
		if(!m_szName || m_szName->empty()) return;
		OPH();
		const time_t now =stable_time();
		if(m_file){
			if(ferror(m_file) || ((int64_t)now - m_file_open_time) > m_file_life_time){
				fclose(m_file);
				m_file =0;
			}
		}
		if(m_file) return;

		// prepare time
		tm tmp;
		const tm* t =localtime_r(&now, &tmp);
		if(!t){
			printf("fail to call localtime_r, %s\n", get_last_error_desc());
			return;
		}

		// open
		String* path =String::Format("../log/%s_%04d%02d%02d%02d", m_szName->c_str(), t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour);
		m_file =fopen(path->c_str(), "a");
		if(m_file){
			m_file_open_time =now;
		}
		else{
			printf("fail to open %s, %s\n", path->c_str(), get_last_error_desc());
		}
	}
}
