/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** TcpConnection Impl ***/
	/** ctor & dtor **/
	TcpConnection::TcpConnection(const int64_t id)
		: m_id(id)
		, m_flag(0)
		, m_sock(INVALID_FD)
		, m_type(TYPE_UNKNOWN)
		, m_state(STATE_DISCONNECT)
		, m_attach_host(0)
		, m_attach_port(0)
		, m_host(0)
		, m_port(0)
		, m_path(0)
		, m_send_buffer(0)
		, m_recv_buffer(0)
		, m_last_reconnect_time(0)
		, m_requestor(0){
	}
	TcpConnection::~TcpConnection(){
	}
	/** Object **/
	void TcpConnection::init(){
		Super::init();
		m_send_buffer =SafeNew<BytesChannel>();
		m_send_buffer->retain();	

		m_recv_buffer =SafeNew<CycleBuffer>();
		m_recv_buffer->retain();

		ASSIGN_POINTER(m_requestor, SafeNew<TcpConnectionRequestor>(m_id, m_send_buffer));
	}
	void TcpConnection::finalize(){
		_clean();

		m_send_buffer->setPushable(false);
		CLEAN_POINTER(m_send_buffer);
		CLEAN_POINTER(m_recv_buffer);

		CLEAN_POINTER(m_attach_host);
		CLEAN_POINTER(m_path);
		CLEAN_POINTER(m_host);
		CLEAN_POINTER(m_requestor);
		Super::finalize();
	}
	/** id **/
	int64_t TcpConnection::getId(){
		return m_id;
	}
	/** MonitorTarget **/
	bool TcpConnection::reborn(){
		if(isLive()){
			return true;
		}
		return _ensure_connect();
	}
	void TcpConnection::sucide(){
		disconnect();
	}
	bool TcpConnection::canReborn(){
		return isActive() && getEventFD()!=INVALID_FD;
	}
	bool TcpConnection::isLive(){
		return m_sock!=INVALID_FD && getEventFD()!=INVALID_FD;
	}
	bool TcpConnection::onEvent(const fd_t fd, const uint64_t events){
		if(fd == m_sock){
			if(events & MonitorTarget::EVT_READ){
				return onRecv();
			}
			if(events & MonitorTarget::EVT_WRITE){
				return onSend();
			}
			return false;
		}
		else if(fd == getEventFD()){
			m_send_buffer->unsignal();
			return onSend();
		}
		else{
			ERROR("TcpConnection occurs error, wtf unknwon fd %lld event", (long long)fd);
			return false;
		}
	}
	bool TcpConnection::onAttachEvent(Monitor* monitor){
		if(!Super::onAttachEvent(monitor)){
			return false;
		}
		if(m_monitor->attachEvent(m_sock, MonitorTarget::EVT_READ, this)
			&& m_monitor->attachEvent(getEventFD(), MonitorTarget::EVT_READ, this)){
			Network::AddConnection(this);
			return true;
		}
		else{
			onDetachEvent();
			return false;
		}
	}
	void TcpConnection::onDetachEvent(){
		Super::onDetachEvent();
		if(!isLive()){
			return;
		}
		if(m_monitor){
			m_monitor->detachEvent(m_sock);
			m_monitor->detachEvent(getEventFD());
			Network::RemoveConnection(getId());
		}
	}
	/** TcpConnection **/
	bool TcpConnection::connect(const char* host, const int32_t port){
		// check arg
		if(!host || !port || strlen(host)==0){
			ERROR("call %s failed, invalid args", __func__);
			return false;
		}
		// check state
		if(m_state != STATE_DISCONNECT){
			ERROR("call %s failed, state is not STATE_DISCONNECT", __func__);
			return false;
		}
		// clean
		_clean();
		// set var and connect
		m_type =TYPE_TCP;
		ASSIGN_POINTER(m_host, String::NewString(host));
		m_port =port;
		RELEASE_POINTER(m_path);
		return _connect_tcp();
	}
	bool TcpConnection::connect(const char* path){
		// check arg
		if(!path){
			ERROR("call %s failed, invalid args", __func__);
			return false;
		}
		const int len =strlen(path);
		if(len==0 || len>PATH_MAX){
			ERROR("call %s failed, invalid args", __func__);
			return false;
		}
		// check state
		if(m_state != STATE_DISCONNECT){
			ERROR("call %s failed, state is not STATE_DISCONNECT", __func__);
			return false;
		}
		// clean
		_clean();
		// set var and connect
		m_type =TYPE_UNIX;
		ASSIGN_POINTER(m_path, String::NewString(path));
		RELEASE_POINTER(m_host);
		m_port =0;
		return _connect_unix();
	}
	void TcpConnection::disconnect(){
		_clean();
	}
	bool TcpConnection::attach(const int client_sock, const char* client_host, const int32_t client_port){
		// clean
		_clean();
		m_type =TYPE_ACCEPTED;
		CLEAN_POINTER(m_path);
		CLEAN_POINTER(m_host);
		m_port =0;

		// check arg
		if(client_sock==INVALID_FD){
			ERROR("call %s failed, invalid args", __func__);
			return false;
		}

		// nonblock
		if(!_set_sock_nonblock(client_sock)){
			_clean();
			ERROR("call %s failed(set socket nonblock), %s", __func__, get_last_error_desc());
			return false;
		}

		// set var
		m_sock =client_sock;
		m_state  =STATE_CONNECT;
		ASSIGN_POINTER(m_attach_host, String::NewString(client_host));
		m_attach_port =client_port;
		return true;
	}
	void TcpConnection::detach(){
		m_sock =INVALID_FD;
		_clean();
	}
	void TcpConnection::setLastReconnectTime(const int64_t t){
		m_last_reconnect_time =t;
	}
	int64_t TcpConnection::getLastReconnectTime(){
		return m_last_reconnect_time;
	}
	bool TcpConnection::send(const char* data, const int64_t data_len){
		return m_send_buffer->push(data, data_len);
	}
	bool TcpConnection::sendv(const MEMORY_SLICE* slice, const int64_t n){
		const bool ok =m_send_buffer->pushv(slice, n);
		return ok;
	}
	void TcpConnection::setReady(){
		m_state =STATE_CONNECT;
		INFO("connection %lld connect(%s:%lld/%s)", (long long)m_id, (m_host?m_host->c_str():"NA"), (long long)m_port, (m_path?m_path->c_str():"NA"));
	}
	void TcpConnection::setAuthed(){
		m_state =STATE_AUTHED;
		INFO("connection %lld authed(%s:%lld/%s)", (long long)m_id, (m_host?m_host->c_str():"NA"), (long long)m_port, (m_path?m_path->c_str():"NA"));
	}
	void TcpConnection::setFlag(const uint64_t flag){
		m_flag =flag;
	}
	bool TcpConnection::isActive(){
		return m_type != TYPE_ACCEPTED;
	}
	bool TcpConnection::isPassive(){
		return !isActive();
	}

	/** query **/
	fd_t TcpConnection::getSocketFD(){
		return m_sock;
	}
	fd_t TcpConnection::getEventFD(){
		return m_send_buffer->getEventFD();
	}
	int32_t TcpConnection::getState(){
		return m_state;
	}
	int32_t TcpConnection::getType(){
		return m_type;
	}
	String* TcpConnection::getAttachHost(){
		return m_attach_host;
	}
	int32_t TcpConnection::getAttachPort(){
		return m_attach_port;
	}
	String* TcpConnection::getHost(){
		return m_host;
	}
	int32_t TcpConnection::getPort(){
		return m_port;
	}
	String* TcpConnection::getPath(){
		return m_path;
	}
	CycleBuffer* TcpConnection::getRecvBuffer(){
		return m_recv_buffer;
	}
	BytesChannel* TcpConnection::getSendBuffer(){
		return m_send_buffer;
	}
	int64_t TcpConnection::getFlag(){
		return m_flag;
	}
	Requestor* TcpConnection::toRequestor(){
		if(m_state < STATE_CONNECT){
			ERROR("call %s failed, disconnected", __func__);
			return 0;
		}
		return m_requestor;
	}

	/** event **/
	bool TcpConnection::onRecv(){
		char data[RECV_BUFFER_SIZE] ={0};
		while(1){
			const int n =::recv(m_sock, &data, sizeof(data), 0);
			if(n < 0){
				const int errcode =get_last_error();
				if(errcode==EINTR || errcode==EAGAIN || errcode==EWOULDBLOCK){
					return true;
				}
				else{
					ERROR("call %s failed(recv), %s", __func__, get_last_error_desc());
					return false;
				}
			}
			else if(n > 0){
				m_recv_buffer->push(data, n);
				return m_recv_buffer->flushIn(&_on_recv, this) >= 0;
			}
			else{
				ERROR("call %s failed(recv), disconnected", __func__);
				return false;
			}
		}
		return true;
	}
	bool TcpConnection::onSend(){
		return m_send_buffer->flushOut(&_on_send, this) >= 0;
	}
	void TcpConnection::onError(){
		_clean();
	}
	/** helper **/
	bool TcpConnection::ParseIpPort(String* host, char szip[32], int32_t& port){
		// parse ip:port
		int num[5] ={0};
		if(5 == sscanf(host->c_str(), "%d.%d.%d.%d:%d", num, num+1, num+2, num+3, num+4)){
			if((num[0]>=0 && num[0]<=255)
			&& (num[1]>=0 && num[1]<=255)
			&& (num[2]>=0 && num[2]<=255)
			&& (num[3]>=0 && num[3]<=255)
			&& (num[4]>=0 && num[4]<=65535)){
				// check
				char sztmp[64] ={0};
				sprintf(sztmp, "%d.%d.%d.%d:%d", num[0], num[1], num[2], num[3], num[4]);
				if(0 != strcmp(sztmp, host->c_str())){
					ERROR("parse ip:port failed, invalid net addr `%s`", host->c_str());
					return false;
				}

				// set
				sprintf(szip, "%d.%d.%d.%d", num[0], num[1], num[2], num[3]);
				port =num[4];
				return true;
			}
			else{
				ERROR("parse ip:port failed, invalid net addr `%s`", host->c_str());
				return false;
			}
		}
		
		// prepare hints
		struct addrinfo hints;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		// hints.ai_protocol = 0;
		hints.ai_socktype = SOCK_STREAM;

		// get addr info
		struct addrinfo *res =0;
		const int e =getaddrinfo(host->c_str(), "http", &hints, &res);
		if(e == -1){
			ERROR("parse ip:port failed, getaddrinfo error");
			return false;
		}
		
		// use first
		bool ok =false;
		struct addrinfo* cur =0;
		for (cur=res; cur; cur=cur->ai_next) {
			struct sockaddr_in *addr = (struct sockaddr_in *)cur->ai_addr;

			// parse ip
			if(!inet_ntop(AF_INET, &addr->sin_addr, szip, 16)){
				ERROR("parse ip:port failed, inet_ntop error");
				break;
			}

			// port
			port =ntohs(addr->sin_port);
			
			ok =true;
			break;
		}

		// free addr info
		freeaddrinfo(res);

		return ok;
	}
	/** private **/
	int64_t TcpConnection::on_auth(char* data, const int64_t s){
		setAuthed();
		return on_recv(data, s);
	}
	int64_t TcpConnection::on_recv(char* data, const int64_t s){
		int64_t c =0; // cursor
		while(c < s){
			// prepare packet
			PACKET packet;
			int64_t n =decode_packet(data + c, s - c, packet);
			if(n < 0){
				break;
			}
			const int64_t packet_len =packet.size;

			// process packet(packet perhaps modified by dispatcher manager)
			if(packet.option & OPT_REQUEST){
				DispatcherManager::Request(toRequestor(), packet, data+c+n, packet.size-n);
			}
			else if(packet.option & OPT_RESPOND){
				DispatcherManager::Reply(packet, data+c+n, packet.size-n);
			}
			else{
				WARN("TcpConnection %lld request/respond service %lld fail, option %llu is not OPT_REQUEST or OPT_RESPOND", (long long)m_id, (long long)(packet.to), (unsigned long long)(packet.option));
			}

			// move cursor
			c +=packet_len;
		}
		return c;
	}
	int64_t TcpConnection::on_send(char* data, int64_t& cursor, int64_t& size, const int64_t capacity){
		int64_t len =0;
		uint64_t evts =MonitorTarget::EVT_READ;
		while(size > 0){
			/*
			if(m_state == STATE_CONNECTING){
				evts |= MonitorTarget::EVT_WRITE;
				DEBUG("connecting");
				break;
			}
			*/

			// calc send size
			int64_t send_size =capacity - cursor;
			if(send_size > size){
				send_size =size;
			}

			// send
			const int n =::send(this->m_sock, data+cursor, send_size, 0);
			if(n == -1){
				const int errcode =get_last_error();
				if(errcode!=EAGAIN && errcode!=EWOULDBLOCK && errcode!=EINTR){
					ERROR("call %s failed(send), %s", __func__, get_last_error_desc());
					return -1;
				}
				evts |= MonitorTarget::EVT_WRITE;
				break;
			}
			cursor +=n;
			size -=n;
			len +=n;
			cursor %= capacity;
		}
		if(m_monitor){
			m_monitor->modifyEvent(m_sock, evts, this);
		}
		return len;
	}
	/** private **/
	bool TcpConnection::_connect_tcp(){
		CHECK_RETURN_VALUE(m_host && m_port>0, false);
		struct sockaddr_in si;
		memset(&si, 0, sizeof(si));
		si.sin_family =AF_INET;
		si.sin_addr.s_addr =inet_addr(m_host->c_str());
		si.sin_port =htons(m_port);
		return _connect(AF_INET, reinterpret_cast< struct sockaddr* >(&si), sizeof(si));
	}
	bool TcpConnection::_connect_unix(){
		CHECK_RETURN_VALUE(m_path, false);
		struct sockaddr_un sun;
		memset(&sun, 0, sizeof(sun));
		sun.sun_family =AF_UNIX;
		strcpy(sun.sun_path, m_path->c_str());
		return _connect(AF_UNIX, reinterpret_cast< struct sockaddr* >(&sun), SUN_LEN(&sun));
	}
	bool TcpConnection::_connect(const int domain, struct sockaddr* addr, const int addr_len){
		// new
		m_sock =socket(domain, SOCK_STREAM, 0);
		if(m_sock == INVALID_FD){
			ERROR("fail to call socket, %s", get_last_error_desc());
			return false;
		}
		// set nonblock
		if(!_set_sock_nonblock(m_sock)){
			ERROR("fail to call socket, %s", get_last_error_desc());
			close_fd(m_sock);
			m_sock =INVALID_FD;
			return false;
		}
		// connect
		const int result =::connect(m_sock, addr, addr_len);
		if(0 == result){
			setReady();
		}
		else{
			const int errcode =get_last_error();
			if(errcode == EINPROGRESS){
				m_state =STATE_CONNECTING;
				INFO("connection %lld connecting(%s:%lld/%s)", (long long)m_id, (m_host?m_host->c_str():"NA"), (long long)m_port, (m_path?m_path->c_str():"NA"));
			}
			else{
				WARN("connect error, %s", get_last_error_desc());
				close_fd(m_sock);
				m_sock =INVALID_FD;
			}
		}
		// return
		if(m_sock == INVALID_FD){
			return false;
		}
		else{
			m_recv_buffer->clear();
			m_send_buffer->clear();
			return true;
		}
	}
	void TcpConnection::_clean(){
		if(m_sock != INVALID_FD){
			close_fd(m_sock);
			m_sock =INVALID_FD;
			INFO("connection %lld disconnect(%s:%lld/%s)", (long long)m_id, (m_host?m_host->c_str():"NA"), (long long)m_port, (m_path?m_path->c_str():"NA"));
		}
		m_state =STATE_DISCONNECT;
		CLEAN_POINTER(m_attach_host);
		m_attach_port =0;

		m_recv_buffer->clear();
		m_send_buffer->clear();
	}
	bool TcpConnection::_set_sock_nonblock(int sock){
		const int flags = ::fcntl(sock, F_GETFL, 0);
		if(-1 == flags){
			ERROR("fail to call fcntl, %s", get_last_error_desc());
			return false;
		}
		if(-1 == ::fcntl(sock, F_SETFL, flags | O_NONBLOCK)){
			ERROR("fail to call fcntl, %s", get_last_error_desc());
			return false;
		}
		return true;
	}
	bool TcpConnection::_ensure_connect(){
		if(m_state == STATE_DISCONNECT){
			if(canReborn()){
				if(m_type==TYPE_TCP){
					return _connect_tcp();
				}
				else if(m_type==TYPE_UNIX){
					return _connect_unix();
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		else{
			return true;
		}
	}
	int64_t TcpConnection::_on_recv(char* data, const int64_t s, void* ctx){
		TcpConnection* self =reinterpret_cast< TcpConnection* >(ctx);
		if(self->m_state == STATE_CONNECTING){
			self->setReady();
		}
		if(self->m_state == STATE_CONNECT){
			if(self->isActive()){
				self->setAuthed();
				return self->on_recv(data, s);
			}
			else{
				return self->on_auth(data, s);
			}
		}
		else{
			return self->on_recv(data, s);
		}
	}
	int64_t TcpConnection::_on_send(char* data, int64_t& cursor, int64_t& size, const int64_t capacity, void* ctx){
		TcpConnection* self =reinterpret_cast< TcpConnection* >(ctx);
		return self->on_send(data, cursor, size, capacity);
	}
}
