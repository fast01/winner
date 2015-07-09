/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PIPE_BUFFER__
#define H_CORE_PIPE_BUFFER__

namespace core{
	/** PipeBuffer **/
	class PipeBuffer : public CycleBuffer{
		SUPPORT_NEWABLE
		typedef CycleBuffer Super;
		enum{
			WAIT_TIMEOUT =1,
			PIPE_DEFAULT_CAPACITY =1024,
		};
	public:
		PipeBuffer();
		virtual ~PipeBuffer();
	public:
		virtual void init();
		virtual Object* clone();
		virtual void finalize();
	public:
		virtual bool setCapacity(const int64_t capacity);
		virtual bool push(const char* p, const int64_t s);
		virtual bool pushv(const PMEMORY_SLICE mem, const int64_t n);
		virtual bool pick(char* p, const int64_t s);
		virtual bool pop(char* p, const int64_t s);
		virtual bool skip(const int64_t s);
		virtual bool clear();
		virtual int64_t getReadCursor();
		virtual int64_t getWriteCursor();
		virtual int64_t getSize();
		virtual int64_t getCapacity();
		virtual char* c_str();
		virtual void* data();
	public:
		void lock();
		void unlock();
	public:
		void close();
		bool isClosed();
	public:
		void signal();
		bool wait(const int64_t secs);
	private:	
		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_is_closed;
	};
}
#endif
