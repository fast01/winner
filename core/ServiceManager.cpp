/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** singleton ***/
	DEFINE_PROCESS_LOCAL_SINGLETON(ServiceManager)

	///*** ServiceManager Impl ***///
	//** ctor & dtor **//
	ServiceManager::ServiceManager()
		: m_route_tb(0)
		, m_service_tb(0)
		, m_unloading_service_tb(0){
	}
	ServiceManager::~ServiceManager(){
	}

	/** Object **/
	void ServiceManager::init(){
		Super::init();
		m_route_tb=New<Hash>();
		m_route_tb->retain();

		m_service_tb=New<Hash>();
		m_service_tb->retain();

		m_unloading_service_tb =New<Hash>();
		m_unloading_service_tb->retain();

		OutputDebug("service manager init [ ok ]");
	}
	void ServiceManager::finalize(){
		OPH();
		unloadAllService();
		CLEAN_POINTER(m_unloading_service_tb);
		CLEAN_POINTER(m_service_tb);
		CLEAN_POINTER(m_route_tb);
		OutputDebug("service manager released [ ok ]");
		Super::finalize();
	}

	/** Dispatcher **/
	bool ServiceManager::request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		return _sendv(SENDV_FLAG_REQUEST, requestor, packet, body, body_len);
	}
	bool ServiceManager::requestByObject(Requestor* requestor, PACKET& packet, Object* obj){
		return _sendv(SENDV_FLAG_REQUEST, requestor, packet, obj);
	}
	bool ServiceManager::notify(PACKET& packet, void* body, const int64_t body_len){
		return _sendv(SENDV_FLAG_NOTIFY, 0, packet, body, body_len);
	}
	bool ServiceManager::notifyByObject(PACKET& packet, Object* obj){
		static thread_local char bs[CACHE_SIZE] ={0};
		int64_t len =CACHE_SIZE;
		char* data =object_to_bytes(obj, bs, len);
		if(len < 0){
			return false;
		}
		const bool ret =_sendv(SENDV_FLAG_NOTIFY, 0, packet, data, len);
		if(data && data != bs){
			DEALLOCATE(data);
		}
		return ret;
	}
	bool ServiceManager::reply(PACKET& packet, void* body, const int64_t body_len){
		return _sendv(SENDV_FLAG_REPLY, 0, packet, body, body_len);
	}
	bool ServiceManager::replyByObject(PACKET& packet, Object* obj){
		return _sendv(SENDV_FLAG_REPLY, 0, packet, obj);
	}
	bool ServiceManager::isMatch(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		return m_service_tb->has(id) || m_route_tb->has(id);
	}

	/** load & update **/
	bool ServiceManager::load(const char* path){
		protocol::ServiceManagerConfig* cfg =(protocol::ServiceManagerConfig*)protocol::create_protocol_from_lua_file(protocol::PROTOCOL_SERVICE_MANAGER_CONFIG, path);
		if(!cfg){
			DEBUG("init service manager fail, cfg load error");
			return false;
		}
		// load service
		for(int64_t i=0; i<cfg->getServiceCount(); ++i){
			core::protocol::ServiceConfig* service =cfg->getService(i);
			loadService(service->getPath()->c_str(), service->getIdService(), service->getIdBegin(), service->getIdEnd(), service->getAlgrithm());
		}
		return true;
	}
	void ServiceManager::update(const int64_t now){
		Int64Array* arr =_get_need_unload_service_id();
		if(!arr){
			return;
		}
		const int64_t n =arr->size();
		for(int64_t i=0; i<n; ++i){
			unloadService(arr->get(i));
		}
	}
	/** service manager **/
	bool ServiceManager::getServiceRange(const int64_t id, int64_t &id_beg, int64_t &id_end, int64_t &alg_type){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(RangeId* service_id =dynamic_cast< RangeId* >(m_route_tb->get(id))){
			id_beg =service_id->getBeginId();
			id_end =service_id->getEndId();
			alg_type =service_id->getAlgrithm();
			return true;
		}
		else{
			return false;
		}
	}
	int64_t ServiceManager::loadService(const char* path, const int64_t id_srv, const int64_t id_beg, const int64_t id_end, const int64_t alg_type){
		if(!path) return -1;
		if(!_can_load_service(id_srv)){
			WARN("loading service %s failed, service id %lld, begin id %lld, end id %lld, algrithm %lld, reason is already exist or unloading", path, (long long)id_srv, (long long)id_beg, (long long)id_end, (long long)alg_type);
			return -1;
		}
		OPH();
		INFO("loading service %s, service id %lld, begin id %lld, end id %lld, algrithm %lld", path, (long long)id_srv, (long long)id_beg, (long long)id_end, (long long)alg_type);
		if(0==id_srv && 0==id_end && 0==id_beg){
			if(Service* service =_load_service(0, path)){
				if(_add_service(service)){
					service->release();

					// set service id range
					const int64_t id =service->getId();
					RangeId* service_id =SafeNew<RangeId>();
					service_id->setRange(id, id+1);
					service_id->setAlgrithm(alg_type);
					_set_service_id(id, service_id);
					return id;
				}
				else{
					service->unload();
					service->release();
					return -1;
				}
			}
			else{
				return -1;
			}
		}
		else if(id_srv>0 && id_beg>0 && id_beg<id_end){
			bool ok =true;
			for(int64_t id=id_beg; id<id_end; ++id){
				if(Service* service =_load_service(id, path)){
					if(_add_service(service)){
						service->release();
					}
					else{
						service->unload();
						service->release();
						ok =false;
						break;
					}
				}
				else{
					ok =false;
					break;
				}
			}
			RangeId* service_id =SafeNew<RangeId>();
			service_id->setRange(id_beg, id_end);
			service_id->setAlgrithm(alg_type);
			_set_service_id(id_srv, service_id);
			if(ok){
				return id_beg;
			}
			else{
				unloadService(id_beg);
				return -1;
			}
		}
		else{
			ERROR("load service failed, id invalid");
			return -1;
		}
	}
	void ServiceManager::unloadService(const int64_t id){
		OPH();
		int64_t id_beg =0;
		int64_t id_end =0;
		if(!_get_and_del_service_range(id, id_beg, id_end)){
			return;
		}
		for(int64_t i=id_beg; i<id_end; ++i){
			if(Service* service =_retain_and_del_service_direct(i)){
				service->unload();
				service->release();
			}
		}
	}
	void ServiceManager::unloadServiceAfter(const int64_t id, const int64_t secs){
		OPH();
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(m_route_tb->has(id)){
			m_unloading_service_tb->set(id, SafeNew< Int64 >(DateTime::Now() + MAX(secs, UNLOAD_SERVICE_MIN_TIMER)));
		}
	}
	void ServiceManager::unloadAllService(){
		OPH();
		// prepare service table
		Hash* srv_tb =0;
		{
			std::lock_guard<LOCK_TYPE> guard(m_lock);
			srv_tb =static_cast< Hash* >(m_service_tb->clone());
			m_service_tb->clear();
			m_route_tb->clear();
			m_unloading_service_tb->clear();
		}
		// unload
		HashIterator* it =static_cast< HashIterator* >(srv_tb->iterator());
		while(it->next()){
			Service* service =static_cast< Service* >(it->getValue());
			if(service){
				service->unload();
			}
		}
		srv_tb->clear();
	}
	Service* ServiceManager::retainService(const int64_t id, const int64_t from, const int64_t who){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(m_unloading_service_tb->has(id)){
			return 0;
		}
		// get real id
		int64_t real_id =0;
		RangeId* service_id =static_cast< RangeId* >(m_route_tb->get(id));
		if(service_id){
			real_id =service_id->getId(from, who);
		}

		// get service
		if(real_id > 0){
			if(Service* srv =static_cast< Service* >(m_service_tb->get(real_id))){
				srv->retain();
				return srv;
			}
			else{
				// retry
				if(false == service_id->isConsistent()){
					const int64_t beg_id =service_id->getBeginId();
					const int64_t end_id =service_id->getEndId();
					for(int64_t i=real_id+1; i<end_id; ++i){
						if((srv =static_cast< Service* >(m_service_tb->get(i)))){
							srv->retain();
							return srv;
						}
					}
					for(int64_t i=beg_id; i<real_id; ++i){
						if((srv =static_cast< Service* >(m_service_tb->get(i)))){
							srv->retain();
							return srv;
						}
					}
				}
				WARN("call %s failed, service %lld from %lld who %lld not ready", __func__, (long long)id, (long long)from, (long long)who);
				return 0;
			}
		}
		else{
			WARN("call %s failed, service %lld from %lld who %lld not ready", __func__, (long long)id, (long long)from, (long long)who);
			return 0;
		}
	}
	Service* ServiceManager::retainServiceDirect(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(Service* srv =dynamic_cast< Service* >(m_service_tb->get(id))){
			srv->retain();
			return srv;
		}
		return 0;
	}
	/** helper **/
	Service* ServiceManager::_load_service(const int64_t id, const char* path){
		String* str =String::NewString(path);
		if(str->hasSubfix(".lua")){
			Service* service =New<LuaService>();
			service->retain();
			if(!service->load(id, path)){
				service->release();
				OutputDebug("load service %s error", path);
				return 0;
			}
			OutputDebug("load service %s success", path);
			return service;
		}
		else if(str->hasSubfix(".so")){
			Service* service =New<CService>();
			service->retain();
			if(!service->load(id, path)){
				service->release();
				OutputDebug("load service %s error", path);
				return 0;
			}
			OutputDebug("load service %s success", path);
			return service;
		}
		else{
			const int64_t tmp =str->indexOf(":");
			if(tmp <= 0){
				OutputDebug("unsupport path %s", path);	
				return 0;
			}
			else{
				String* creator =str->subString(0, tmp);
				if(Service* service =static_cast< Service* >(ObjectFactory::Create(creator))){
					service->retain();
					if(!service->load(id, path)){
						OutputDebug("load service %s error", path);
						service->release();
						return 0;
					}
					OutputDebug("load service %s success", path);
					return service;
				}
				else{
					OutputDebug("unsupport path %s", path);	
				}
			}
		}
		return 0;
	}
	void ServiceManager::_set_service_id(const int64_t id, RangeId* service_id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_route_tb->set(id, service_id);
	}
	bool ServiceManager::_add_service(Service* service){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(m_service_tb->get(service->getId())){
			return false;
		}
		m_service_tb->set(service->getId(), service);
		return true;
	}
	Int64Array* ServiceManager::_get_need_unload_service_id(){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(m_unloading_service_tb->empty()){
			return 0;
		}
		const int64_t now =DateTime::Now();
		Int64Array* arr =SafeNew<Int64Array>();
		HashIterator* it =static_cast< HashIterator* >(m_unloading_service_tb->iterator());
		while(it->next()){
			const int64_t id_srv =static_cast< Int64* >(it->getKey())->getValue();
			const int64_t t =static_cast< Int64* >(it->getValue())->getValue();
			if(now >= t){
				arr->push_back(id_srv);
				it->remove();
			}
		}
		return arr;
	}
	bool ServiceManager::_get_and_del_service_range(const int64_t id, int64_t &id_beg, int64_t &id_end){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(RangeId* service_id =static_cast< RangeId* >(m_route_tb->get(id))){
			id_beg =service_id->getBeginId();
			id_end =service_id->getEndId();
			m_route_tb->remove(id);
			m_unloading_service_tb->remove(id);
			return true;
		}
		else{
			return false;
		}
	}
	Service* ServiceManager::_retain_and_del_service_direct(const int64_t service_id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(Service* service =static_cast< Service* >(m_service_tb->get(service_id))){
			service->retain();
			m_service_tb->remove(service_id);
			return service;
		}
		return 0;
	}
	bool ServiceManager::_can_load_service(const int64_t service_id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		return !m_unloading_service_tb->has(service_id) && !m_route_tb->has(service_id);
	}
	bool ServiceManager::_sendv(const int64_t flag, Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		// prepare service
		Service* service =0;
		if(flag==SENDV_FLAG_REQUEST || flag==SENDV_FLAG_NOTIFY){
			service =retainService(static_cast<int64_t>(packet.to), static_cast<int64_t>(packet.from), static_cast<int64_t>(packet.who));
		}
		else if(flag == SENDV_FLAG_REPLY){
			service =retainServiceDirect(static_cast<int64_t>(packet.to));
		}
		else{
			ERROR("service manager sendv failed, unknown flag %lld", (long long)flag);
		}
		if(!service){
			return false;
		}
		// send
		bool ok =false;
		switch(flag){
		case SENDV_FLAG_REQUEST:
		case SENDV_FLAG_NOTIFY:
			ok =service->request(requestor, packet, body, body_len); break;
		case SENDV_FLAG_REPLY:
			ok =service->reply(packet, body, body_len); break;
		}
		service->release();
		return ok;
	}
	bool ServiceManager::_sendv(const int64_t flag, Requestor* requestor, PACKET& packet, Object* obj){
		packet.option |= OPT_BODY_IS_OBJECT_POINTER;
		return _sendv(flag, requestor, packet, reinterpret_cast< void* >(&obj), sizeof(obj));
	}
}
