/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** Channel impl ***/
	/** ctor & dtor **/
	Channel::Channel()
		: m_container(0)
		, m_event_fd(INVALID_FD)
		, m_pushable(false)
		, m_on_event(0)
		, m_ctx(0){
	}
	Channel::~Channel(){
	}

	/** Object **/
	void Channel::init(){
		Super::init();
		_create_fd();
		ASSIGN_POINTER(m_container, SafeNew<Array>());
	}
	void Channel::finalize(){
		_close_fd();
		CLEAN_POINTER(m_container);
		Super::finalize();
	}
	Object* Channel::clone(){
		return this;
	}

	/** MonitorTarget **/
	bool Channel::reborn(){
		return false;
	}
	void Channel::sucide(){
	}
	bool Channel::isLive(){
		return m_event_fd != INVALID_FD;
	}
	bool Channel::canReborn(){
		return false;
	}
	bool Channel::onEvent(const fd_t fd, const uint64_t events){
		return m_on_event && m_on_event(this, fd, events, m_ctx);
	}
	bool Channel::onAttachEvent(Monitor* monitor){
		// check
		if(!Super::onAttachEvent(monitor)){
			return false;
		}
		return m_monitor && m_monitor->attachEvent(m_event_fd, MonitorTarget::EVT_READ, this);
	}
	void Channel::onDetachEvent(){
		Super::onDetachEvent();
		if(isLive() && m_monitor){
			m_monitor->detachEvent(m_event_fd);
		}
	}

	/** Channel **/
	bool Channel::push(Object* obj){
		if(_push(obj)){
			return signal();
		}
		else{
			return false;
		}
	}
	void Channel::pop(){
		_read_event_fd();
		_pop();
	}
	Object* Channel::front(){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		return m_container->front();
	}
	bool Channel::empty(){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		return m_container->empty();
	}
	int64_t Channel::size(){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		return m_container->size();
	}
	void Channel::setPushable(const bool y){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		m_pushable =y;
	}
	bool Channel::canPush(){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		return m_pushable && m_event_fd!=INVALID_FD;	
	}
	bool Channel::good(){
		return m_event_fd != INVALID_FD;
	}
	bool Channel::signal(){
		// signal
		uint64_t count =1;
		while(1){
			if(::write(m_event_fd, &count, sizeof(count)) == sizeof(count)){
				return true;
			}
			const int errnum = get_last_error();
			if(errnum == EINTR){
				continue;	
			}
			else if(errnum == EAGAIN){
				continue;
			}
			else{
				ERROR("%s WTF:errno %d, %s", __FUNCTION__, errnum, get_last_error_desc());	
				return false;
			}
		}
	}
	bool Channel::unsignal(){
		int64_t count =0;
		::read(m_event_fd, &count, sizeof(count));
		return count == 1;
	}
	void Channel::setEventCallback(PFN_ON_EVENT pfn, Object* ctx){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		m_on_event =pfn;
		ASSIGN_POINTER(m_ctx, ctx);
	}
	/** private **/
	bool Channel::_create_fd(){
		// close
		_close_fd();

		// create
		m_event_fd =eventfd(0, EFD_NONBLOCK | EFD_SEMAPHORE);
		if(m_event_fd != INVALID_FD){
			setPushable(true);
			return true;
		}
		else{
			ERROR("fail to create eventfd in %s", __FUNCTION__);	
			return false;
		}
	}
	void Channel::_close_fd(){
		if(m_event_fd != INVALID_FD){
			::close(m_event_fd);
			m_event_fd =INVALID_FD;
			setPushable(false);
		}
	}
	void Channel::_read_event_fd(){
		uint64_t count =0;
		while(1){
			if(::read(m_event_fd, &count, sizeof(count)) == sizeof(count)){
				break;
			}
			const int errnum = get_last_error();
			if(errnum == EINTR){
				continue;	
			}
			else if(errnum==EAGAIN || errnum==EWOULDBLOCK){
				continue;
			}
			else{
				ERROR("call %s failed, WTF:errno %d, %s", __FUNCTION__, errnum, get_last_error_desc());	
				break;
			}
		}
	}
	bool Channel::_push(Object* obj){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		if(!m_pushable){ return false; }
		m_container->push_back(obj);
		return true;
	}
	void Channel::_pop(){
		std::lock_guard<DEFAULT_LOCK_TYPE> guard(m_lock);
		m_container->pop_front();
	}
}
