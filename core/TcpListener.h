/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TCP_LISTENER_H__
#define H_CORE_TCP_LISTENER_H__

/*
 * TcpListener is not thread safe. but i think you shuold obey these rules:
 *      1. If listener is monitored, only the monitor thread can acccess it.
 *      2. If listener is not monitored, it can be call in one thread, but not multi-thread.
 */

namespace core{
	/** TcpListener **/
	class TcpListener: public MonitorTarget{
		typedef MonitorTarget Super;
		SUPPORT_NEWABLE
		DECLARE_CLASS_INFO
	public:
		enum{
			BACKLOG =1024
		};
	protected:
		TcpListener();
		virtual ~TcpListener();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t getId();
		virtual void setId(const int64_t id);
	public:
		virtual bool reborn();
		virtual void sucide();
		virtual bool isLive();
		virtual bool canReborn();
		virtual bool onEvent(const fd_t fd, const uint64_t events);
		virtual bool onAttachEvent(Monitor* monitor);
		virtual void onDetachEvent();
	public:
		bool listen(const char* path, const int32_t port);
		bool listen(const int32_t port);
		bool listen(const char* path);
		void shutdown();
	public:
		void setDeliverBegin(const int64_t range);
		int64_t getDeliverBegin();

		void setDeliverRange(const int64_t range);
		int64_t getDeliverRange();
	public:
		bool good();
		fd_t getNativeHandle();
	public:
		static int64_t GenConnectionId();
	protected:
		virtual TcpConnection* create_connection();
	private:
		bool _listen(const int domain, struct sockaddr* addr, const int addr_len);
	private:
		void _close_sock();
	private:
		int64_t m_id;
		int m_sock;
		String* m_path;
		int32_t m_port;
		int64_t m_deliver_begin;
		int64_t m_deliver_range;
	};
}

#endif
