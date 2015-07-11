/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CYCLE_BUFFER__
#define H_CORE_CYCLE_BUFFER__

namespace core{
	/** MEMORY SLICE **/
	typedef struct tagMEMORY_SLICE{
		char* ptr;
		int64_t size;
	}MEMORY_SLICE, *PMEMORY_SLICE;

	/** CycleBuffer **/
	class CycleBuffer : public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef int64_t (*PFN_FLUSH_IN)(char* data, const int64_t s, void* ctx);
		typedef int64_t (*PFN_FLUSH_OUT)(char* data, int64_t& cursor, int64_t& size, const int64_t capacity, void* ctx);
	protected:
		CycleBuffer();
		virtual ~CycleBuffer();
	public:
		virtual Object* clone();
		virtual void finalize();
	public:
		virtual bool setCapacity(const int64_t capacity);
		virtual bool push(const char* p, const int64_t s);
		virtual bool pushv(const PMEMORY_SLICE mem, const int64_t n);

		virtual bool pop(char* p, const int64_t s);
		virtual int64_t tryPop(char* p, const int64_t s);

		virtual bool pick(char* p, const int64_t s);
		virtual int64_t tryPick(char* p, const int64_t s);

		virtual bool skip(const int64_t s);
		virtual bool clear();
		virtual bool align();
		virtual int64_t flushIn(PFN_FLUSH_IN pfn, void* ctx);
		virtual bool flushOut(PFN_FLUSH_OUT pfn, void* ctx);
	public:
		virtual int64_t getReadCursor();
		virtual int64_t getWriteCursor();
		virtual int64_t getSize();
		virtual int64_t getCapacity();
		virtual char* c_str();
		virtual void* data();
	private:
		bool _set_capacity(int64_t capacity);
		bool _push(const char* p, const int64_t s);
		bool _pushv(const PMEMORY_SLICE mem, const int64_t n);

		bool _pop(char* p, const int64_t s);
		int64_t _try_pop(char* p, int64_t s);

		bool _pick(char* p, const int64_t s);
		int64_t _try_pick(char* p, int64_t s);
		bool _align();
		int64_t _flush_in(PFN_FLUSH_IN pfn, void* ctx);
		int64_t _flush_out(PFN_FLUSH_OUT pfn, void* ctx);
	private:	
		char* m_data;
		int64_t m_size;
		int64_t m_read_cursor;
		int64_t m_write_cursor;
		int64_t m_capacity;
	};
}
#endif
