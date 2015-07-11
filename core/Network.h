/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_NETWORK_H__
#define H_CORE_NETWORK_H__

/*
 * Network is complete thread safe. but i think you shuold obey these rules:
 *     1. Methods like "request, requestByObject, notify, notifyByObject, reply, replyByObject, isMatch" can called in multi-thread.
 *     2. Methods like "addListener, removeListener, hasListener" can called in multi-thread.
 *     3. Methods like "broadcastToConnection, sendToConnectionDirect, sendToConnection, addConnection, hasConnection, retainConnection, removeConnection" can called in multi-thread.
 *     4. Methods like "deliver, abandon" can called in multi-thread.
 *     4. Methods like "close, Close" can called in multi-thread.
 *     6. Methods like "update, load" should't be called, because framework call them in internal.
 */

namespace core{
	/** Network **/
	class Network: public Dispatcher{
		typedef Dispatcher Super;
		SUPPORT_NEWABLE
		DECLARE_PROCESS_LOCAL_SINGLETON(Network)
	public:
		enum{
			BROADCAST_FLAG_PASSIVE =0,
			BROADCAST_FLAG_ACTIVE =1,
			BROADCAST_FLAG_ALL =2,
		};
	private:
		Network();
		virtual ~Network();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual bool request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
		virtual bool requestByObject(Requestor* requestor, PACKET& packet, Object* obj);
	public:
		virtual bool notify(PACKET& packet, void* body, const int64_t body_len);
		virtual bool notifyByObject(PACKET& packet, Object* obj);
	public:
		virtual bool reply(PACKET& packet, void* body, const int64_t body_len);
		virtual bool replyByObject(PACKET& packet, Object* obj);
	public:
		virtual bool isMatch(const int64_t id);
	public:
		bool load(const char* path);
		bool update(const int64_t now);
		void close();
	public:
		void addListener(TcpListener* listener);
		void removeListener(const int64_t id);
		bool hasListener(const int64_t id);
	public:
		bool broadcastToConnection(const int64_t flag, const PMEMORY_SLICE slice, const int64_t n);
		bool sendToConnectionDirect(const int64_t id, const PMEMORY_SLICE slice, const int64_t n);
		bool sendToConnection(const int64_t id, const int64_t from, const int64_t who, const PMEMORY_SLICE slice, const int64_t n);
		bool hasConnection(const int64_t id);
		void addConnection(TcpConnection* conn);
		TcpConnection* retainConnection(const int64_t id);
		TcpConnection* retainConnection(const int64_t id, const int64_t from, const int64_t who);
		void removeConnection(const int64_t id);
	public:
		bool deliver(MonitorTarget* target, int64_t beg, int64_t end);
		void abandon(MonitorTarget* target);
	public:
		void setRoute(const int64_t id, const int64_t beg_id, const int64_t end_id, const int64_t alg_type);
		int64_t getRouteTarget(const int64_t id, const int64_t from, const int64_t who);
		bool getRouteRange(const int64_t id, int64_t &id_beg, int64_t &id_end);
	public:
		static void Close();
		static bool Deliver(MonitorTarget* target, int64_t beg, int64_t end);
		static void AddConnection(TcpConnection* conn);
		static TcpConnection* RetainConnection(const int64_t id);
		static TcpConnection* RetainConnection(const int64_t id, const int64_t from, const int64_t who);
		static void RemoveConnection(const int64_t id);
		static void AddListener(TcpListener* listener);
		static void RemoveListener(const int64_t id);
	private:
		bool _create_slave(const int64_t thread_count);
		void _shutdown_slave();
		static void _slave_loop(Object* param);
	private:
		void _init_deliver_channel();
		void _clear_deliver_channel();
		void _destroy_deliver_channel();
		void _append_deliver_channel(Channel* chan);
		Channel* _retain_deliver_channel(const int64_t id);
		Channel* _retain_deliver_channel(int64_t beg, int64_t end);
	private:
		void _add_requestor(const int64_t id, TcpConnectionRequestor* requestor);
		void _del_requestor(const int64_t id);
		TcpConnectionRequestor* _retain_requestor(const int64_t id);
	private:
		bool _sendv(const int64_t flag, PACKET& packet, void* body, const int64_t body_len);
		bool _sendv(const int64_t flag, PACKET& packet, Object* obj);
	private:
		TcpListener* _create_listener(String* type);
		static bool _on_channel(Channel* channel, const fd_t, const int64_t events, Object* ctx);
	private:
		Hash* m_listener_tb;
		Hash* m_conn_tb;
		Array* m_slave_list;
		Array* m_channel_list;
		Hash* m_route_tb;
		Hash* m_requestor_tb;
	
		// thread safe
		std::atomic<int64_t> m_success_counter;
		std::atomic<int64_t> m_fail_counter;

		// quit flag
		std::atomic<bool> m_close_flag;
	};
}

#endif
