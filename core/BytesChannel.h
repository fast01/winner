/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BYTES_CHANNEL_H__
#define H_CORE_BYTES_CHANNEL_H__

namespace core{
	/** BytesChannel **/
	class BytesChannel : public MonitorTarget{
		SUPPORT_NEWABLE
		typedef MonitorTarget Super;
		enum{
			INVALID_FD = -1	
		};
	public:
		typedef bool (*PFN_ON_EVENT)(BytesChannel*, const fd_t, const int64_t events, Object* ctx);
	protected:
		BytesChannel();
		virtual ~BytesChannel();
	public:
		virtual void init();
		virtual Object* clone();
		virtual void finalize();
	public:
		virtual bool reborn();
		virtual void sucide();
		virtual bool isLive();
		virtual bool canReborn();
		virtual bool onEvent(const fd_t fd, const uint64_t events);
		virtual bool onAttachEvent(Monitor* monitor);
		virtual void onDetachEvent();
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
		int64_t getCapacity();
		virtual char* c_str();
		virtual void* data();

		virtual int64_t flushIn(CycleBuffer::PFN_FLUSH_IN pfn, void* ctx);
		virtual bool flushOut(CycleBuffer::PFN_FLUSH_OUT pfn, void* ctx);
	public:
		fd_t getEventFD();
		void setEventCallback(PFN_ON_EVENT pfn, Object* ctx);
	public:
		void setPushable(const bool y);
		bool canPush();
		bool good();
		bool signal();
		bool unsignal();
	private:
		bool _create_fd();
		void _close_fd();
		void _read_event_fd();
		bool _push(const char* p, const int64_t s);
		bool _pushv(const PMEMORY_SLICE mem, const int64_t n);
		bool _pop(char* p, const int64_t s);
	private:	
		CycleBuffer* m_cycle_buffer;
		fd_t m_event_fd;
		bool m_pushable;
		std::mutex m_mutex;

		PFN_ON_EVENT m_on_event;
		Object* m_ctx;
	};
}
#endif
