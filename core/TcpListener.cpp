/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl TcpListener ***/
	DEFINE_CLASS_INFO(TcpListener)

	/** ctor & dtor **/
	TcpListener::TcpListener()
		: m_id(0)
		, m_sock(INVALID_FD)
		, m_path(0)
		, m_port(0)
		, m_deliver_begin(0)
		, m_deliver_range(0){
	}
	TcpListener::~TcpListener(){
		_close_sock();
	}
	/** Object **/
	void TcpListener::init(){
		Super::init();
	}
	void TcpListener::finalize(){
		_close_sock();
		CLEAN_POINTER(m_path);
		Super::finalize();
	}
	/** MonitorTarget **/
	bool TcpListener::reborn(){
		if(isLive()){
			return true;
		}
		if(m_port > 0){
			return listen(m_port);
		}
		else if(m_path){
			return listen(m_path->c_str());
		}
		else{
			ERROR("TcpListener reborn faield, both port and path is invalid");
			return false;
		}
	}
	void TcpListener::sucide(){
		_close_sock();
	}
	bool TcpListener::canReborn(){
		return m_port>0 || m_path;
	}
	bool TcpListener::isLive(){
		return m_sock != INVALID_FD;
	}
	bool TcpListener::onEvent(const fd_t fd, const uint64_t events){
		struct sockaddr_in client_addr;
		socklen_t addr_len =0;
		const int client_fd =::accept(m_sock, (struct sockaddr*)&client_addr, &addr_len);
		if(client_fd != INVALID_FD){
			TcpConnection* conn =create_connection();
			const char* client_host =::inet_ntoa(client_addr.sin_addr);
			const int32_t client_port =client_addr.sin_port;
			conn->attach(client_fd, client_host, client_port);

			// deliver
			return Network::Deliver(conn, m_deliver_begin, m_deliver_range);
		}
		else{
			const int errcode =get_last_error();
			if(errcode==EINTR || errcode==EAGAIN || errcode==EWOULDBLOCK){
				return true;
			}
			else{
				WARN("TcpListener accept failed, %s", get_last_error_desc());
				return true;
			}
		}
	}
	bool TcpListener::onAttachEvent(Monitor* monitor){
		if(!Super::onAttachEvent(monitor)){
			return false;
		}
		if(m_monitor->attachEvent(m_sock, MonitorTarget::EVT_READ, this)){
			Network::AddListener(this);
			return true;
		}
		else{
			return false;
		}
	}
	void TcpListener::onDetachEvent(){
		Super::onDetachEvent();
		if(!isLive()){
			return;
		}
		if(m_monitor){
			m_monitor->detachEvent(m_sock);
			Network::RemoveListener(getId());
		}
	}

	/** id **/
	int64_t TcpListener::getId(){
		return m_id;
	}
	void TcpListener::setId(const int64_t id){
		m_id =id;
	}

	/** socket operation **/
	bool TcpListener::listen(const char* path, const int32_t port){
		if(path){
			return listen(path);
		}
		else{
			return listen(port);
		}
	}
	bool TcpListener::listen(const int32_t port){
		if(port <= 0){
			return false;
		}
		m_port =port;
		struct sockaddr_in si;
		memset(&si, 0, sizeof(si));
		si.sin_family =AF_INET;
		si.sin_addr.s_addr =htonl(INADDR_ANY);
		si.sin_port =htons(m_port);
		return _listen(AF_INET, reinterpret_cast< struct sockaddr* >(&si), sizeof(si));
	}
	bool TcpListener::listen(const char* path){
		if(!path || strlen(path)==0){
			return false;
		}
		ASSIGN_POINTER(m_path, STR(path));
		CHECK_RETURN_VALUE(m_path, false);
		struct sockaddr_un sun;
		memset(&sun, 0, sizeof(sun));
		sun.sun_family =AF_UNIX;
		strcpy(sun.sun_path, m_path->c_str());
		return _listen(AF_UNIX, reinterpret_cast< struct sockaddr* >(&sun), SUN_LEN(&sun));
	}
	/** deliver **/
	void TcpListener::setDeliverBegin(const int64_t beg){
		m_deliver_begin =beg;
	}
	int64_t TcpListener::getDeliverBegin(){
		return m_deliver_begin;
	}

	void TcpListener::setDeliverRange(const int64_t range){
		m_deliver_range =range;
	}
	int64_t TcpListener::getDeliverRange(){
		return m_deliver_range;
	}
	/** getter **/
	bool TcpListener::good(){
		return m_sock != INVALID_FD;
	}
	fd_t TcpListener::getNativeHandle(){
		return m_sock;
	}
	/** gen conn id **/
	int64_t TcpListener::GenConnectionId(){
		static std::atomic<int64_t> id(CONNECTION_ID_GEN_BEGIN);
		id +=1;
		return id;
	}
	/** create conn **/
	TcpConnection* TcpListener::create_connection(){
		return SafeNew<TcpConnection>(GenConnectionId());
	}
	/** extra param **/
	bool TcpListener::setExtraParam(String* param){
		return true;
	}
	/** helper **/
	bool TcpListener::_listen(const int domain, struct sockaddr* addr, const int addr_len){
		// close old
		_close_sock();

		// new
		m_sock =::socket(domain, SOCK_STREAM, 0);
		if(m_sock == INVALID_FD){
			ERROR("network listen error, %s", get_last_error_desc());
			return false;
		}

		// set opt
		const int sock_flag =1;
		if(0 != ::setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &sock_flag, sizeof(sock_flag))){
			ERROR("network listen error, %s", get_last_error_desc());
			return false;
		}

		// bind
		if(0 != ::bind(m_sock, addr, addr_len)){
			ERROR("network listen error, %s", get_last_error_desc());
			return false;
		}

		// listen
		const int result =::listen(m_sock, BACKLOG);
		if(0 == result){
			return true;
		}
		else{
			ERROR("network listen error, %s", get_last_error_desc());
			_close_sock();
			return false;
		}
	}
	void TcpListener::_close_sock(){
		if(m_sock != INVALID_FD){
			close_fd(m_sock);
			m_sock =INVALID_FD;
		}
	}
}
