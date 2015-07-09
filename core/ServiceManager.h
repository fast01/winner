/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_SERVICE_MANAGER_H__
#define H_CORE_SERVICE_MANAGER_H__

/*
 * ServiceManager is partial thread safe. but i think you shuold obey these rules:
 *     1. Methods like "loadService, unloadService, unloadServiceAfter, unloadAllService" should be called in one thread(for example: master/main thread)
 *     2. Methods like "request, requestByObject, notify, notifyByObject, reply, replyByObject, isMatch, retainService, retainServiceDirect, getServiceRange" can called in multi-thread.
 *     3. Methods like "update, load" should't be called, because framework call them in internal.
 */


namespace core{
	/** ServiceManager **/
	class ServiceManager: public Dispatcher{
		typedef Dispatcher Super;
		SUPPORT_NEWABLE
		DECLARE_PROCESS_LOCAL_SINGLETON(ServiceManager)
	public:
		enum{
			UNLOAD_SERVICE_MIN_TIMER =5 // 5 secs
		};
	protected:
		ServiceManager();
		virtual ~ServiceManager();
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
		void update(const int64_t now);
	public:
		int64_t loadService(const char* path, const int64_t id_srv, const int64_t id_beg, const int64_t id_end, const int64_t alg_type);
		void unloadService(const int64_t id);
		void unloadServiceAfter(const int64_t id, const int64_t secs);
		void unloadAllService();
	public:
		Service* retainService(const int64_t id, const int64_t from, const int64_t who);
		Service* retainServiceDirect(const int64_t id);
		bool getServiceRange(const int64_t id, int64_t &id_beg, int64_t &id_end, int64_t &alg_type);
	private:
		Service* _load_service(const int64_t id, const char* path);
		bool _add_service(Service* service);
		void _set_service_id(const int64_t id, RangeId* service_id);
		Int64Array* _get_need_unload_service_id();
		bool _get_and_del_service_range(const int64_t id, int64_t &id_beg, int64_t &id_end);
		Service* _retain_and_del_service_direct(const int64_t service_id);
		bool _can_load_service(const int64_t service_id);
		bool _sendv(const int64_t flag, Requestor* requestor, PACKET& packet, void* body, const int64_t body_len);
		bool _sendv(const int64_t flag, Requestor* requestor, PACKET& packet, Object* obj);
		char* _object_to_bytes(char* data, Object* obj, int64_t& len);
	private:
		Hash* m_route_tb;
		Hash* m_service_tb;
		Hash* m_unloading_service_tb;
		Bytes* m_bytes;
	};
}

#endif
