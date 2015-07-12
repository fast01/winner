/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** Service impl ***/
	/** ctor & dtor **/
	Service::Service()
		: m_name(0)
		, m_desc(0)
		, m_path(0)
		, m_id(0)
		, m_load_id(0)
		, m_load_path(0)
		, m_state(STATE_INIT)
		, m_start_time(0)
		, m_now(0)
		, m_update_count(0)
		, m_last_update_time(0)
		, m_thread(0)
		, m_channel(0){
	}
	Service::~Service(){
	}

	/** current **/
	static thread_local Service* g_current_service =0;
	Service* Service::Current(){
		return g_current_service;
	}
	void Service::SetCurrent(Service* service){
		g_current_service =service;
	}

	/* Object */
	void Service::init(){
		Super::init();
		ASSIGN_POINTER(m_name, STR(""));
		ASSIGN_POINTER(m_desc, STR(""));
		ASSIGN_POINTER(m_path, STR(""));
	}
	void Service::finalize(){
		CLEAN_POINTER(m_name);
		CLEAN_POINTER(m_desc);
		CLEAN_POINTER(m_path);
		CLEAN_POINTER(m_load_path);

		CLEAN_POINTER(m_thread);
		CLEAN_POINTER(m_channel);
		Super::finalize();
	}

	/** Requestor **/
	int64_t Service::getId(){
		return m_id;	
	}
	bool Service::reply(PACKET& packet, void* body, const int64_t body_len){
		if(packet.to != (uint64_t)getId()){
			ERROR("fail to call %s, packet.to %lld missmatch id", __func__, (long long)packet.to);
			return false;
		}
		packet.option &= (~static_cast<uint64_t>(OPT_REQUEST));
		packet.option |= static_cast<uint64_t>(OPT_RESPOND);
		return push_message(0, packet, body, body_len);
	}
	bool Service::sendv(const PMEMORY_SLICE slice, const int64_t n){
		FATAL("service not support sendv");
		return false;
	}

	/** basic **/
	String* Service::getName(){
		return m_name;
	}
	String* Service::getDesc(){
		return m_desc;	
	}
	String* Service::getPath(){
		return m_path;	
	}

	/** state **/
	int64_t Service::getState(){
		return m_state;	
	}
	int64_t Service::getStartTime(){
		return m_start_time;
	}
	int64_t Service::getNow(){
		return m_now;
	}
	int64_t Service::getUpdateCount(){
		return m_update_count;
	}
	int64_t Service::getLastUpdateTime(){
		return m_last_update_time;
	}

	/** load unload suspend resume **/
	bool Service::load(const int64_t id, const char* path){
		ASSERT(m_state == STATE_INIT);
		ASSIGN_POINTER(m_thread, SafeNew<Thread>());
		BytesChannel* channel =SafeNew<BytesChannel>();
		if(!channel->good()){
			ERROR("servuce load failed, new BytesChannel error");
			return false;
		}
		ASSIGN_POINTER(m_channel, channel);
		m_channel->setPushable(false);

		m_load_id =id;
		ASSIGN_POINTER(m_load_path, String::New(path));

		if(m_thread->startup(_on_startup, this)){
			while(1){
				const int64_t st =m_state;
				if(st == STATE_RUNNING){
					return true;
				}
				else if(st == STATE_DEAD){
					m_thread->join();
					CLEAN_POINTER(m_thread);
					return false;
				}
				else{
					sleep_us(1);
				}
			}
			return true;
		}
		else{
			CLEAN_POINTER(m_thread);
			return false;
		}
	}
	void Service::unload(){
		ASSERT(m_state == STATE_RUNNING);

		PACKET packet;
		memset(&packet, 0, sizeof(PACKET));
		packet.option =OPT_ZOMBIE;

		push_message(0, packet, 0, 0);
		m_thread->join();
	}
	bool Service::request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		if(requestor == this){
			ERROR("fail to call %s, can't request self", __func__);
			return false;
		}
		packet.option &= (~static_cast<uint64_t>(OPT_RESPOND));
		packet.option |= static_cast<uint64_t>(OPT_REQUEST);
		return push_message(requestor, packet, body, body_len);
	}

	/** module operation **/
	bool Service::load_module(const int64_t id, const char* path){
		if(id < 0){
			WARN("Service::load_module error, id < 0");
			return false;
		}
		else if(id > 0){
			m_id =id;
		}
		ASSIGN_POINTER(m_path, path ? STR(path) : STR(""));
		return true;
	}
	void Service::unload_module(){
	}

	/** message operation **/
	bool Service::push_message(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		packet.size =sizeof(PACKET) + sizeof(Requestor*) + body_len;
		MEMORY_SLICE slice[3] ={
			{ reinterpret_cast< char* >(&packet), sizeof(PACKET) },
			{ reinterpret_cast< char* >(&requestor), sizeof(Requestor*) },
			{ reinterpret_cast< char* >(body), body_len }
		};
		RETAIN_POINTER(requestor);
		if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
			ASSERT(static_cast<uint64_t>(body_len) >= sizeof(Object*));
			Object* obj =*reinterpret_cast< Object** >(body);
			RETAIN_POINTER(obj);
		}
		return m_channel->pushv(slice, 3);
	}
	/** sugar **/
	const char* Service::name(){
		return m_name ? m_name->c_str() : "NA";
	}

	/** event **/
	bool Service::on_load(){
		return true;
	}
	void Service::on_update(const int64_t now){
	}
	void Service::on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len){
	}
	void Service::on_unload(){
	}

	/** helper **/
	void Service::_on_startup(Object* param){
		Service* service =static_cast< Service* >(param);
		service->retain();
		service->_on_startup_internal();
		service->release();
	}
	bool Service::_on_channel(BytesChannel* channel, const fd_t fd, const int64_t events, Object* ctx){
		Service* self =static_cast< Service* >(ctx);
		return self->_on_channel_internal(fd, events);
	}
	void Service::_on_clean_channel(){
		const int64_t min_size =sizeof(PACKET)+sizeof(Requestor*)+sizeof(Object*);
		uint32_t packet_size =0;
		while(m_channel->pick(reinterpret_cast< char* >(&packet_size), sizeof(packet_size))){
			ASSERT(m_channel->getSize() >= static_cast<int64_t>(packet_size));
			// pop data
			const int64_t s =MIN(packet_size, min_size);
			m_channel->pop(m_data, s);

			// PACKET
			int64_t offset =0;
			PACKET& packet =*reinterpret_cast< PACKET* >(m_data);
			offset +=sizeof(PACKET);

			// Requestor
			Requestor* requestor =*reinterpret_cast< Requestor** >(m_data+offset);
			offset +=sizeof(Requestor*);
			RELEASE_POINTER(requestor);

			// Object
			if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
				ASSERT(packet_size >= min_size);
				Object* object =*reinterpret_cast< Object** >(m_data+offset);
				RELEASE_POINTER(object);
			}

			// skip
			m_channel->skip(packet_size-s);
		}
		m_channel->clear();
	}
	/** internal **/
	void Service::_on_startup_internal(){
		OPH();
		// monitor channel
		Monitor* monitor =Monitor::Instance();
		if(!monitor->monitor(m_channel)){
			ERROR("service monitor channel failed");
			m_state =STATE_DEAD;
			return;
		}

		// load module
		bool ok =false;
		if(load_module(m_load_id, m_load_path ? m_load_path->c_str() : 0)){
			if(m_id <= 0){
				ERROR("service id can't <= 0");
				ERROR("service %s call on_load failed", name());
				unload_module();
			}
			else{
				if(on_load()){
					ok =true;
				}
				else{
					ERROR("service %s call on_load failed", name());
					unload_module();
				}
			}
		}
		else{
			ERROR("service %s load_module failed", name());
		}
		
		// run
		if(ok){
			// prepare
			m_channel->setEventCallback(_on_channel, this);
			m_channel->setPushable(true);
			m_state =STATE_RUNNING;
			m_start_time =stable_time();

			// loop
			Service::SetCurrent(this);
			while(monitor->watch(0)){
				m_now =stable_time();
				if((m_now - m_last_update_time) >= UPDATE_INTERVAL_SECS){
					on_update(m_now);
					m_last_update_time =m_now;
					m_update_count +=1;
				}

			}
			Service::SetCurrent(0);

			// clean
			_on_clean_channel();
			m_channel->setEventCallback(0, 0);
			m_channel->setPushable(false);

			// unload module
			on_unload();
			unload_module();
		}

		// dead
		m_state =STATE_DEAD;
	}
	bool Service::_on_channel_internal(const fd_t fd, const int64_t events){
		// process message
		packet_size_t packet_size =0;
		while(m_channel->pick(reinterpret_cast< char* >(&packet_size), sizeof(packet_size))){
			OPH();
			// recv data
			char* recv_data =m_data;
			if(packet_size > (uint32_t)sizeof(m_data)){
				recv_data =reinterpret_cast< char* >(ALLOCATE(packet_size));
			}
			// pop message
			if(m_channel->pop(recv_data, packet_size)){
				//// decode
				// packet
				int64_t offset =0;
				const PACKET& packet=*reinterpret_cast< PACKET* >(recv_data);
				offset +=sizeof(PACKET);
				// requestor
				Requestor* requestor =*reinterpret_cast< Requestor** >(recv_data + offset);
				offset +=sizeof(Requestor*);
				// object
				Object* obj =0;
				if(packet.option & OPT_BODY_IS_OBJECT_POINTER){
					ASSERT(packet_size >= offset+sizeof(Object*));
					obj =*reinterpret_cast< Object** >(recv_data + offset);
				}

				//// log
				INFO("service %s(%lld) start command %lld from %lld, who = %lld, packet size = %lld, sn = %lld, option = %lld", 
						name(), (long long)m_id, (long long)packet.command, (long long)packet.from, (long long)packet.who, (long long)packet.size, (long long)packet.sn, (long long)packet.option);

				//// call event
				if(packet.option & OPT_ZOMBIE){
					Monitor::Close();
				}
				else{
					on_message(requestor, packet, recv_data+offset, packet.size-offset);
				}

				//// release obj & requestor
				// if(requestor){
				//	DEBUG("%lld", requestor->getRefCount());
				// }	
				RELEASE_POINTER(obj);
				RELEASE_POINTER(requestor);
			}
			else{
				WARN("service %s(%lld) occurs invalid packet, just clear channel", name(), (long long)m_id);
				_on_clean_channel();
			}

			// clean recv_data
			if(recv_data != m_data){
				DEALLOCATE(recv_data);
			}
		}
		return true;
	}
}
