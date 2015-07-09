/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TCP_CONNECTION_H__
#define H_CORE_TCP_CONNECTION_H__

/*
 * TcpConnection is partial thread safe. but i think you shuold obey these rules:
 *      1. Only Methods like "send, sendv" can be called in multi-thread.
 */

namespace core{
	/** TcpConnection **/
	class TcpConnection: public MonitorTarget{
		typedef MonitorTarget Super;
		SUPPORT_NEWABLE
	public:
		enum{
			RECV_BUFFER_SIZE =1024,
			ACCEPT_CONNECTION_ID_MIN =640000,
			RECONNECT_TIMEOUT =1, // sec
		};
		enum{
			STATE_DISCONNECT,
			STATE_CONNECTING,
			STATE_CONNECT,
			STATE_AUTHED
		};
		enum{
			TYPE_UNKNOWN,
			TYPE_TCP,
			TYPE_UNIX,
			TYPE_ACCEPTED
		};
	protected:
		TcpConnection(const int64_t id);
		virtual ~TcpConnection();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t getId();
		void setId(const int64_t id);
	public:
		virtual bool reborn();
		virtual void sucide();
		virtual bool isLive();
		virtual bool canReborn();
		virtual bool onEvent(const fd_t fd, const uint64_t events);
		virtual bool onAttachEvent(Monitor* monitor);
		virtual void onDetachEvent();
	public:
		bool connect(const char* host, const int32_t port);
		bool connect(const char* path);
		void disconnect();
		bool attach(const int client_sock, const char* client_host, const int32_t client_port);
		void detach();
		bool ensureConnect();
		void setLastReconnectTime(const int64_t t);
		int64_t getLastReconnectTime();
		bool send(const char* data, const int64_t data_len);
		bool sendv(const PMEMORY_SLICE slice, const int64_t n);
		void setReady();
		void setAuthed();
		void setFlag(const uint64_t flag);
	public:
		fd_t getSocketFD();
		fd_t getEventFD();
		int32_t getState();
		int32_t getType();
		String* getAttachHost();
		int32_t getAttachPort();
		String* getPath();
		String* getHost();
		int32_t getPort();
		bool isActive();
		bool isPassive();
		int64_t getFlag();
		CycleBuffer* getRecvBuffer();
		BytesChannel* getSendBuffer();
		Requestor* toRequestor();
	public:
		bool onRecv();
		bool onSend();
		void onError();
	protected:
		virtual int64_t on_auth(char* data, const int64_t s);
		virtual int64_t on_recv(char* data, const int64_t s);
		virtual bool on_send(char* data, int64_t& cursor, int64_t& size, const int64_t capacity);
	private:
		bool _connect_tcp();
		bool _connect_unix();
		bool _connect(const int domain, struct sockaddr* addr, const int addr_len);
		void _clean();
		static bool _set_sock_nonblock(int sock);
		bool _ensure_connect();
		static int64_t _on_recv(char* data, const int64_t s, void* ctx);
		static bool _on_send(char* data, int64_t& cursor, int64_t& size, const int64_t capacity, void* ctx);
	public:
		int64_t m_id;
		int64_t m_flag;
		fd_t m_sock;
		int32_t m_type;
		std::atomic<int64_t> m_state;

		String* m_attach_host;
		int32_t m_attach_port;

		String* m_host;
		int32_t m_port;
		String* m_path;

		BytesChannel* m_send_buffer;
		CycleBuffer* m_recv_buffer;

		int64_t m_last_reconnect_time;

		TcpConnectionRequestor* m_requestor;
	};

}

#endif
