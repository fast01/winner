/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** singleton ***/
	DEFINE_PROCESS_LOCAL_SINGLETON(Network)

	/*** Network Impl ***/
	/** ctor & dtor **/
	Network::Network()
		: m_listener_tb(0)
		, m_conn_tb(0)
		, m_slave_list(0)
		, m_channel_list(0)
		, m_route_tb(0)
		, m_requestor_tb(0)
		, m_success_counter(0)
		, m_fail_counter(0)
		, m_close_flag(false){
	}
	Network::~Network(){
	}

	/** Object **/
	void Network::init(){
		Super::init();
		m_listener_tb =New<Hash>();
		m_listener_tb->retain();

		m_conn_tb =New<Hash>();
		m_conn_tb->retain();

		m_slave_list =New<Array>();
		m_slave_list->retain();

		_init_deliver_channel();

		m_route_tb =New<Hash>();
		m_route_tb->retain();

		m_requestor_tb =New<Hash>();
		m_requestor_tb->retain();
	}
	void Network::finalize(){
		OPH();
		_shutdown_slave();
		CLEAN_POINTER(m_listener_tb);
		CLEAN_POINTER(m_conn_tb);
		CLEAN_POINTER(m_slave_list);
		_destroy_deliver_channel();
		CLEAN_POINTER(m_route_tb);
		CLEAN_POINTER(m_requestor_tb);
		OutputDebug("network released [ok]");
		Super::finalize();
	}

	/** Dispatcher **/
	bool Network::request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		if(TcpConnectionRequestor* tcr =dynamic_cast< TcpConnectionRequestor* >(requestor)){
			_add_requestor(static_cast<int64_t>(packet.from), tcr);
		}
		return _sendv(SENDV_FLAG_REQUEST, packet, body, body_len);
	}
	bool Network::requestByObject(Requestor* requestor, PACKET& packet, Object* obj){
		if(TcpConnectionRequestor* tcr =dynamic_cast< TcpConnectionRequestor* >(requestor)){
			_add_requestor(static_cast<int64_t>(packet.from), tcr);
		}
		return _sendv(SENDV_FLAG_REQUEST, packet, obj);
	}
	bool Network::notify(PACKET& packet, void* body, const int64_t body_len){
		return _sendv(SENDV_FLAG_NOTIFY, packet, body, body_len);
	}
	bool Network::notifyByObject(PACKET& packet, Object* obj){
		return _sendv(SENDV_FLAG_NOTIFY, packet, obj);
	}
	bool Network::reply(PACKET& packet, void* body, const int64_t body_len){
		return _sendv(SENDV_FLAG_REPLY, packet, body, body_len);
	}
	bool Network::replyByObject(PACKET& packet, Object* obj){
		return _sendv(SENDV_FLAG_REPLY, packet, obj);
	}
	bool Network::isMatch(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		return m_requestor_tb->has(id) || m_route_tb->has(id);
	}

	/** route **/
	void Network::setRoute(const int64_t id, const int64_t beg_id, const int64_t end_id, const int64_t alg_type){
		if(id <= 0){
			WARN("dispatcher manager set route failed, id must > 0");
			return;
		}
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		RangeId* range_id =static_cast< RangeId* >(m_route_tb->get(id));
		if(!range_id){
			range_id =SafeNew<RangeId>();
			m_route_tb->set(id, range_id);
		}
		range_id->setRange(beg_id, end_id);
		range_id->setAlgrithm(alg_type);
	}
	int64_t Network::getRouteTarget(const int64_t id, const int64_t from, const int64_t who){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(RangeId* range_id =static_cast< RangeId* >(m_route_tb->get(id))){
			return range_id->getId(from, who);
		}
		return 0;
	}
	bool Network::getRouteRange(const int64_t id, int64_t &id_beg, int64_t &id_end){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(RangeId* range_id =static_cast< RangeId* >(m_route_tb->get(id))){
			id_beg =range_id->getBeginId();
			id_end =range_id->getEndId();
			return true;
		}
		return false;
	}

	/** load & loop **/
	bool Network::load(const char* path){
		// load config
		using namespace core::protocol;
		NetworkConfig* cfg =(NetworkConfig*)protocol::create_protocol_from_lua_file(PROTOCOL_NETWORK_CONFIG, path);
		if(!cfg){
			DEBUG("load network fail, cfg load error");
			return false;
		}

		// dump config
		Bytes* str =SafeNew<Bytes>();
		cfg->appendToString(str, 0);
		DEBUG(str->toString()->c_str());

		// prepare master
		Channel* channel =SafeNew<Channel>();
		if(!channel->good()){
			DEBUG("load network fail, new Channel error");
			return false;
		}
		channel->setEventCallback(_on_channel, 0);
		Monitor* monitor =Monitor::Instance();
		if(!monitor){
			DEBUG("load network fail, master monitor is null");
			return false;
		}
		monitor->setId(0);
		if(!monitor->monitor(channel)){
			DEBUG("load network fail, master monitor channel error");
			return false;
		}
		_append_deliver_channel(channel);

		// start thread
		const int64_t slave_count =cfg->getSlaveCount();
		if(!_create_slave(slave_count)){
			DEBUG("load network fail, create slave error");
			return false;
		}

		// listen
		const int64_t listener_count =cfg->getListenerCount();
		for(int64_t i=0; i<listener_count; ++i){
			if(NetworkListenerConfig* listener_cfg =cfg->getListener(i)){
				// prepare config
				String* type =listener_cfg->getType();

				String* path =listener_cfg->getPath();
				const int32_t port =listener_cfg->getPort();
				const char* szpath =path? path->c_str() : 0;

				const int64_t runner_beg =listener_cfg->getRunnerBegin();
				const int64_t runner_range =listener_cfg->getRunnerRange();

				const int64_t deliver_begin =listener_cfg->getDeliverBegin();
				const int64_t deliver_range =listener_cfg->getDeliverRange();

				// create listener
				TcpListener* listener =_create_listener(type);
				if(!listener){
					WARN("listener type `%s` is unknown", type ? type->c_str() : "nil");
					continue;
				}
				listener->setDeliverBegin(deliver_begin);
				listener->setDeliverRange(deliver_range);

				// listen
				bool ok =false;
				if(port){
					ok =listener->listen(port);
				}
				else{
					ok =listener->listen(szpath);
				}

				// deliver
				if(ok){
					ok =deliver(listener, runner_beg, runner_range);
				}

				// log
				if(ok){
					INFO("listen path `%s`, port `%d` success", szpath?szpath:"", (int)port);
				}
				else{
					ERROR("listen path `%s`, port `%d` failed", szpath?szpath:"", (int)port);
				}
			}
		}

		/* connect remote */
		for(int64_t i=0; i<cfg->getRemoteCount(); ++i){
			// prepare config
			NetworkConnectionConfig* remote_cfg =cfg->getRemote(i);
			const int64_t id =remote_cfg->getId();
			if(id >= CONNECTION_ID_GEN_BEGIN){
				ERROR("remote connection id %lld is invalid, can't >= CONNECTION_ID_GEN_BEGIN", (long long)id);
				continue;
			}
			String* host =remote_cfg->getHost();
			const char* szhost =host ? host->c_str() : 0;
			const int32_t port =remote_cfg->getPort();

			const int64_t runner_beg =remote_cfg->getRunnerBegin();
			const int64_t runner_range =remote_cfg->getRunnerRange();
			if(hasConnection(id)){
				WARN("remote connection id %lld duplicate", (long long)id);
				continue;
			}

			// new remote conn
			TcpConnection* remote =SafeNew<TcpConnection>(id);

			// connect
			bool ok =false;
			if(port){
				ok =remote->connect(szhost, port);
			}
			else{
				ok =remote->connect(szhost);
			}

			// deliver
			if(ok){
				ok =deliver(remote, runner_beg, runner_range);
			}

			// log
			if(ok){
				INFO("connect %lld host `%s`, port `%d` success", (long long)id, szhost?szhost:"", (int)port);
			}
			else{
				ERROR("connect %lld host `%s`, port `%d` failed", (long long)id, szhost?szhost:"", (int)port);
			}
		}
		// route
		for(int64_t i=0; i<cfg->getRouteCount(); ++i){
			NetworkRouteConfig* route_cfg =static_cast< NetworkRouteConfig* >(cfg->getRoute(i));
			if(route_cfg->getIdService()>0 && route_cfg->getIdBegin()>0 && route_cfg->getIdEnd()>route_cfg->getIdBegin()){
				setRoute(route_cfg->getIdService(), route_cfg->getIdBegin(), route_cfg->getIdEnd(), route_cfg->getAlgrithm());
			}
			else{
				ERROR("network route config invalid");
			}
		}
		return true;
	}
	bool Network::update(const int64_t now){
		if(m_close_flag){
			return false;
		}
		Monitor* monitor =Monitor::Instance();
		if(!monitor){
			ERROR("%s error, master monitor is null", __func__);
			return false;
		}
		if(!monitor->watch(0)){
			m_close_flag =true;
			return false;
		}
		return true;
	}
	void Network::close(){
		m_close_flag =true;
	}
	/** listener **/
	void Network::addListener(TcpListener* listener){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_listener_tb->set(listener->getId(), listener);
	}
	void Network::removeListener(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_listener_tb->remove(id);
	}
	bool Network::hasListener(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		return m_listener_tb->has(id);
	}
	/** connection manager **/
	bool Network::broadcastToConnection(const int64_t flag, const PMEMORY_SLICE slice, const int64_t n){
		if(!slice || n<=0) return false;
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		HashIterator* it =static_cast<HashIterator*>(m_conn_tb->iterator());
		while(it->next()){
			TcpConnection* conn =static_cast< TcpConnection* >(it->getValue());
			if(flag == BROADCAST_FLAG_ALL){
				conn->sendv(slice, n);
			}
			else if(flag == BROADCAST_FLAG_ACTIVE){
				if(conn->isActive()){
					conn->sendv(slice, n);
				}
			}
			else if(flag == BROADCAST_FLAG_PASSIVE){
				if(conn->isPassive()){
					conn->sendv(slice, n);
				}
			}
			else if(flag == conn->getFlag()){
				conn->sendv(slice, n);
			}
		}
		return true;
	}
	bool Network::sendToConnection(const int64_t id, const int64_t from, const int64_t who, const PMEMORY_SLICE slice, const int64_t n){
		if(!slice || n<=0) return false;
		if(TcpConnection* conn =retainConnection(id, from, who)){
			const bool ok =conn->sendv(slice, n);
			conn->release();
			return ok;
		}
		else{
			return false;
		}
	}
	bool Network::sendToConnectionDirect(const int64_t id, const PMEMORY_SLICE slice, const int64_t n){
		if(!slice || n<=0) return false;
		if(TcpConnection* conn =retainConnection(id)){
			const bool ok =conn->sendv(slice, n);
			conn->release();
			return ok;
		}
		else{
			return false;
		}
	}
	bool Network::hasConnection(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		return m_conn_tb->has(id);
	}
	void Network::addConnection(TcpConnection* conn){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_conn_tb->set(conn->getId(), conn);
	}
	TcpConnection* Network::retainConnection(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(TcpConnection* conn =static_cast<TcpConnection*>(m_conn_tb->get(id))){
			conn->retain();
			return conn;
		}
		return 0;
	}
	TcpConnection* Network::retainConnection(const int64_t id, const int64_t from, const int64_t who){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		// get real id
		int64_t real_id =0;
		RangeId* range_id =static_cast< RangeId* >(m_route_tb->get(id));
		if(range_id){
			real_id =range_id->getId(from, who);
			DEBUG("real_id is %lld", real_id);
		}

		// get conn
		if(real_id > 0){
			if(TcpConnection* conn =static_cast< TcpConnection* >(m_conn_tb->get(real_id))){
				conn->retain();
				return conn;	
			}
			else{
				// retry
				if(false == range_id->isConsistent()){
					const int64_t beg_id =range_id->getBeginId();
					const int64_t end_id =range_id->getEndId();
					for(int64_t i=real_id+1; i<end_id; ++i){
						if((conn =static_cast< TcpConnection* >(m_conn_tb->get(i)))){
							conn->retain();
							return conn;
						}
					}
					for(int64_t i=beg_id; i<real_id; ++i){
						if((conn =static_cast< TcpConnection* >(m_conn_tb->get(i)))){
							conn->retain();
							return conn;
						}
					}
				}
				WARN("call %s failed, connection %lld from %lld who %lld not ready", __func__, (long long)real_id, (long long)from, (long long)who);
				return 0;
			}
		}
		else{
			WARN("call %s failed, connection %lld from %lld who %lld not ready", __func__, (long long)real_id, (long long)from, (long long)who);
			return 0;
		}
	}
	void Network::removeConnection(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_conn_tb->remove(id);
	}
	/** deliver **/
	bool Network::deliver(MonitorTarget* target, int64_t beg, int64_t end){
		if(!target){
			return false;
		}
		if(Channel* channel =static_cast< Channel* >(_retain_deliver_channel(beg, end))){
			const bool ok =channel->push(SafeNew<Pair>(target, SafeNew<Int64>(1)));
			channel->release();
			return ok;
		}
		else{
			ERROR("network deliver failed, begin %lld, end %lld", (long long)beg, (long long)end);
			return false;
		}
	}
	void Network::abandon(MonitorTarget* target){
		// prepare
		if(!target){
			return;
		}
		const int64_t monitor_id =target->getMonitorId();
		if(Channel* channel =static_cast< Channel* >(_retain_deliver_channel(monitor_id))){
			channel->push(SafeNew<Pair>(target, SafeNew<Int64>(0)));
			channel->release();
		}
		else{
			ERROR("network deliver failed, monitor id %lld", (long long)monitor_id);
		}
	}
	/** util**/
	void Network::Close(){
		if(auto self =Instance()){
			self->close();
		}
	}
	bool Network::Deliver(MonitorTarget* target, int64_t beg, int64_t end){
		if(auto self =Instance()){
			return self->deliver(target, beg, end);
		}
		return false;
	}
	void Network::AddConnection(TcpConnection* conn){
		if(auto self =Instance()){
			self->addConnection(conn);
		}
	}
	TcpConnection* Network::RetainConnection(const int64_t id){
		if(auto self =Instance()){
			return self->retainConnection(id);
		}
		return 0;
	}
	TcpConnection* Network::RetainConnection(const int64_t id, const int64_t from, const int64_t who){
		if(auto self =Instance()){
			return self->retainConnection(id, from, who);
		}
		return 0;
	}
	void Network::RemoveConnection(const int64_t id){
		if(auto self =Instance()){
			self->removeConnection(id);
		}
	}
	void Network::AddListener(TcpListener* listener){
		if(auto self =Instance()){
			self->addListener(listener);
		}
	}
	void Network::RemoveListener(const int64_t id){
		if(auto self =Instance()){
			self->removeListener(id);
		}
	}
	/** slave **/
	bool Network::_create_slave(const int64_t slave_count){
		m_success_counter =0;
		m_fail_counter =0;
		// start thread
		for(int64_t i=1; i<=slave_count; ++i){
			// prepare
			Channel* channel =SafeNew<Channel>();
			if(!channel->good()){
				break;
			}
			Thread* th =SafeNew<Thread>();
			channel->setEventCallback(_on_channel, 0);
			channel->push(SafeNew<Int64>(i)); // push monitor id

			// push
			_append_deliver_channel(channel);
			m_slave_list->push_back(th);

			// start
			th->startup(_slave_loop, channel);
		}
		// check thread success/fail counter
		while(m_slave_list->size() != (m_success_counter+m_fail_counter)){
			sleep_us(1);
		}
		// check success counter
		if(m_success_counter != slave_count){
			_shutdown_slave();
			return false;
		}
		return true;
	}
	void Network::_shutdown_slave(){
		const int64_t n =m_slave_list->size();
		for(int64_t i=0; i<n; ++i){
			Thread* th =static_cast< Thread* >(m_slave_list->get(i));
			Channel* channel =static_cast< Channel* >(_retain_deliver_channel(i+1));
			ASSERT(channel);
			channel->push(SafeNew<Int64>(0));
			channel->release();
			th->join();
		}
		_clear_deliver_channel();
		m_slave_list->clear();
		m_success_counter =0;
		m_fail_counter =0;

		DEBUG("network slave shutdowned");
	}
	void Network::_slave_loop(Object* param){
		Network* self =Network::Instance();
		if(!self){
			ERROR("%s failed, network is null", __func__);
			return;
		}
		Monitor* monitor =Monitor::Instance();
		if(!monitor){
			ERROR("%s failed, master monitor is null", __func__);
			return;
		}
		Channel* channel =static_cast< Channel* >(param);
		Int64* monitor_id =static_cast< Int64* >(channel->front());
		monitor->setId(monitor_id->getValue());
		channel->pop();

		if(monitor->monitor(channel)){
			DEBUG("success call %s @%lld", __func__, (long long)monitor->getId());
			self->m_success_counter +=1;
			while(monitor->watch(0));
		}
		else{
			ERROR("fail to call %s @%lld, monitor error", __func__, (long long)monitor->getId());
			self->m_fail_counter +=1;
		}
		DEBUG("network slave thread died");
	}
	/** deliver channel **/
	void Network::_init_deliver_channel(){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		ASSIGN_POINTER(m_channel_list, New<Array>());
	}
	void Network::_clear_deliver_channel(){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_channel_list->clear();
	}
	void Network::_destroy_deliver_channel(){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		CLEAN_POINTER(m_channel_list);
	}
	void Network::_append_deliver_channel(Channel* chan){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_channel_list->push_back(chan);
	}
	Channel* Network::_retain_deliver_channel(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(Channel* chan =static_cast< Channel* >(m_channel_list->get(id))){
			chan->retain();
			return chan;
		}
		return 0;
	}
	Channel* Network::_retain_deliver_channel(int64_t beg, int64_t end){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		const int64_t channel_cnt =m_channel_list->size();
		if(channel_cnt == 0){
			return 0;
		}
		// calc monitor id
		if(beg < 0){ beg =0; }
		if(beg >= channel_cnt){ beg =channel_cnt-1; }
		if(end <= beg){ end =beg+1; }
		if(end > channel_cnt){ end =channel_cnt; }
		const int64_t monitor_id =beg + rand() % (end-beg);
		// retain channel
		if(Channel* chan =static_cast< Channel* >(m_channel_list->get(monitor_id))){
			chan->retain();
			return chan;
		}
		return 0;
	}
	/** requestor **/
	void Network::_add_requestor(const int64_t id, TcpConnectionRequestor* requestor){
		if(requestor && id>0){
			std::lock_guard<LOCK_TYPE> guard(m_lock);
			m_requestor_tb->set(id, requestor);
		}
	}
	void Network::_del_requestor(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_requestor_tb->remove(id);
	}
	TcpConnectionRequestor* Network::_retain_requestor(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		if(TcpConnectionRequestor* requestor =static_cast< TcpConnectionRequestor* >(m_requestor_tb->get(id))){
			requestor->retain();
			return requestor;
		}
		else{
			return 0;
		}
	}
	/** sendv **/
	bool Network::_sendv(const int64_t flag, PACKET& packet, void* body, const int64_t body_len){
		packet.option &= (~static_cast<uint64_t>(OPT_BODY_IS_OBJECT_POINTER));
		if(flag==SENDV_FLAG_REQUEST || flag==SENDV_FLAG_NOTIFY){
			// prepare packet header
			packet.option &=(~static_cast<uint64_t>(OPT_RESPOND));
			packet.option |=static_cast<uint64_t>(OPT_REQUEST);
			packet.size =static_cast<uint64_t>(body_len);
			char packet_bs[PACKET_ENCODE_MAX_SIZE] ={0};
			const int64_t packet_len =encode_packet(packet_bs, PACKET_ENCODE_MAX_SIZE, packet);
			ASSERT(packet_len > 0);

			// prepare mem slice
			MEMORY_SLICE slice[2] ={
				reinterpret_cast< char* >(packet_bs), packet_len,
				reinterpret_cast< char* >(body), body_len
			};

			// send
			return sendToConnection(static_cast<int64_t>(packet.to), static_cast<int64_t>(packet.from), static_cast<int64_t>(packet.who), slice, 2);
		}
		else if(flag == SENDV_FLAG_REPLY){
			packet.option &=(~static_cast<uint64_t>(OPT_REQUEST));
			packet.option |=static_cast<uint64_t>(OPT_RESPOND);
			if(TcpConnectionRequestor* requestor =_retain_requestor(static_cast<int64_t>(packet.to))){
				const bool ok =requestor->reply(packet, body, body_len);
				requestor->release();
				return ok;
			}
			else{
				return false;
			}
		}
		else{
			ERROR("network sendv failed, unknown flag %lld", (long long)flag);
			return false;
		}
	}
	bool Network::_sendv(const int64_t flag, PACKET& packet, Object* obj){
		static thread_local char bs[CACHE_SIZE] ={0};
		int64_t len =CACHE_SIZE;
		char* data =object_to_bytes(obj, bs, len);
		if(len < 0){
			return false;
		}
		const bool ret =_sendv(flag, packet, data, len);
		if(data && data != bs){
			DEALLOCATE(data);
		}
		return ret;
	}
	/** misc **/
	TcpListener* Network::_create_listener(String* type){
		return static_cast< TcpListener* >(ObjectFactory::Create(type));
	}
	bool Network::_on_channel(Channel* channel, const fd_t, const int64_t events, Object* ctx){
		Monitor* monitor =Monitor::Instance();
		if(!monitor){
			ERROR("%s error, monitor is null", __func__);
			return false;
		}
		while(Object* front =channel->front()){
			if(Pair* pair =dynamic_cast< Pair* >(front)){
				MonitorTarget* target =static_cast< MonitorTarget* >(pair->getFirst());	
				Int64* op =static_cast< Int64* >(pair->getSecond());	
				if(op && op->getValue()){
					monitor->monitor(target);
				}
				else{
					monitor->demonitor(target);
				}
			}
			else if(Int64* quit_flag =dynamic_cast< Int64* >(front)){
				UNUSED(quit_flag);
				monitor->close();
			}
			else{
				WARN("network channel recv unknown object");
			}
			channel->pop();
		}
		return true;
	}
}
