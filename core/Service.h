/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_SERVICE_H__
#define H_CORE_SERVICE_H__

/*
 * Service is partial thread safe. but i think you shuold obey these rules:
 *     1. Methods like "get*" can called in multi-thread.
 *     2. Methods like "reply, request" can called in multi-thread.
 *     3. Methods like "Current" is special, it is associated with a thread local object.
 *     4. Methods like "SetCurrent, load, unload" can only called by framework.
 *     5. Methods non-public can only called by framework.
 */


namespace core{
	/** Service **/
	class Service: public Requestor{
		typedef Requestor Super;
		enum{
			UPDATE_INTERVAL_SECS =1,
			LOCAL_STORAGE_SIZE =1024*1024	
		};
	public:
		enum{
			STATE_INIT,
			STATE_RUNNING,
			STATE_DEAD,
		};
	protected:
		Service();
		virtual ~Service();
	public:
		virtual void init();
		virtual void finalize();
	public:
		static Service* Current();
		static void SetCurrent(Service* service);
	public:
		virtual int64_t getId();
		virtual bool reply(PACKET& packet, void* body, const int64_t body_len);
		virtual bool sendv(const PMEMORY_SLICE slice, const int64_t n);
	public:
		String* getName();
		String* getDesc();
		String* getPath();
		int64_t getState();
		int64_t getStartTime();
		int64_t getNow();
		int64_t getUpdateCount();
		int64_t getLastUpdateTime();
	public:
		bool load(const int64_t id, const char* path);
		void unload();
		bool request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
	protected:
		virtual bool load_module(const int64_t id, const char* path);
		virtual void unload_module();
	protected:
		virtual bool on_load();
		virtual void on_update(const int64_t now);
		virtual void on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
		virtual void on_unload();
	protected:
		bool push_message(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
		const char* name();
	private:
		static void _on_startup(Object* param);
		static bool _on_channel(BytesChannel* channel, const fd_t, const int64_t events, Object* ctx);
	private:
		void _on_startup_internal();
		bool _on_channel_internal(const fd_t fd, const int64_t events);
		void _on_clean_channel();
	protected:
		// basic
		String* m_name;
		String* m_desc;
		String* m_path;
		int64_t m_id;

		// load
		int64_t m_load_id;
		String* m_load_path;
		
		// state
		std::atomic<int64_t> m_state;
		int64_t m_start_time;
		int64_t m_now;
		int64_t m_update_count;
		int64_t m_last_update_time;
		char m_data[LOCAL_STORAGE_SIZE];

		// thread
		Thread* m_thread;

		// channel
		BytesChannel* m_channel;

		// bytes
		Bytes* m_bytes;
	};
}

#endif
