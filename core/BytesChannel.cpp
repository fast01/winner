/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** BytesChannel impl ***/
	/** ctor & dtor **/
	BytesChannel::BytesChannel()
		: m_cycle_buffer(0)
		, m_event_fd(INVALID_FD)
		, m_pushable(false)
		, m_on_event(0)
		, m_ctx(0){
	}
	BytesChannel::~BytesChannel(){
	}

	/** Object **/
	void BytesChannel::init(){
		Super::init();
		_create_fd();
		ASSIGN_POINTER(m_cycle_buffer, SafeNew<CycleBuffer>());
	}
	Object* BytesChannel::clone(){
		return this;
	}
	void BytesChannel::finalize(){
		_close_fd();
		CLEAN_POINTER(m_cycle_buffer);
		Super::finalize();
	}

	/** MonitorTarget **/
	bool BytesChannel::reborn(){
		return false;
	}
	void BytesChannel::sucide(){
	}
	bool BytesChannel::isLive(){
		return m_event_fd != INVALID_FD;
	}
	bool BytesChannel::canReborn(){
		return false;
	}
	bool BytesChannel::onEvent(const fd_t fd, const uint64_t events){
		// DEBUG("BytesChannel::onEvent");
		return m_on_event && m_on_event(this, fd, events, m_ctx);
	}
	bool BytesChannel::onAttachEvent(Monitor* monitor){
		// check
		if(!Super::onAttachEvent(monitor)){
			return false;
		}
		return m_monitor && m_monitor->attachEvent(m_event_fd, MonitorTarget::EVT_READ, this);
	}
	void BytesChannel::onDetachEvent(){
		Super::onDetachEvent();
		if(isLive() && m_monitor){
			m_monitor->detachEvent(m_event_fd);
		}
	}

	/** self **/
	bool BytesChannel::setCapacity(const int64_t capacity){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->setCapacity(capacity);
	}
	bool BytesChannel::push(const char* p, const int64_t s){
		if(_push(p, s)){
			return signal();
		}
		else{
			return false;
		}
	}
	bool BytesChannel::pushv(const PMEMORY_SLICE mem, const int64_t n){
		if(_pushv(mem, n)){
			return signal();
		}
		else{
			return false;
		}
	}
	bool BytesChannel::pick(char* p, const int64_t s){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->pick(p, s);
	}
	bool BytesChannel::pop(char* p, const int64_t s){
		_read_event_fd();
		return _pop(p, s);
	}
	bool BytesChannel::skip(const int64_t s){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->skip(s);
	}
	bool BytesChannel::clear(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->clear();
	}

	/** getter **/
	int64_t BytesChannel::getReadCursor(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->getReadCursor();
	}
	int64_t BytesChannel::getWriteCursor(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->getWriteCursor();
	}
	int64_t BytesChannel::getSize(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->getSize();
	}
	int64_t BytesChannel::getCapacity(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->getCapacity();
	}
	char* BytesChannel::c_str(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->c_str();
	}
	void* BytesChannel::data(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->data();
	}
	fd_t BytesChannel::getEventFD(){
		return m_event_fd;
	}

	/** flush **/
	int64_t BytesChannel::flushIn(CycleBuffer::PFN_FLUSH_IN pfn, void* ctx){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->flushIn(pfn, ctx);
	}
	bool BytesChannel::flushOut(CycleBuffer::PFN_FLUSH_OUT pfn, void* ctx){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->flushOut(pfn, ctx);
	}
	/** BytesChannel **/
	void BytesChannel::setPushable(const bool y){
		std::lock_guard<std::mutex> guard(m_mutex);
		m_pushable =y;
	}
	bool BytesChannel::canPush(){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_pushable && m_event_fd!=INVALID_FD;	
	}
	bool BytesChannel::good(){
		return m_event_fd!=INVALID_FD;
	}
	void BytesChannel::setEventCallback(PFN_ON_EVENT pfn, Object* ctx){
		std::lock_guard<std::mutex> guard(m_mutex);
		m_on_event =pfn;
		ASSIGN_POINTER(m_ctx, ctx);
	}
	bool BytesChannel::signal(){
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
			else if(errnum==EAGAIN || errnum==EWOULDBLOCK){
				continue;
			}
			else{
				ERROR("call %s failed, WTF:errno %d, %s", __FUNCTION__, errnum, get_last_error_desc());	
				return false;
			}
		}
	}
	bool BytesChannel::unsignal(){
		int64_t count =0;
		::read(m_event_fd, &count, sizeof(count));
		return count == 1;
	}
	/** private **/
	bool BytesChannel::_create_fd(){
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
	void BytesChannel::_close_fd(){
		if(m_event_fd != INVALID_FD){
			::close(m_event_fd);
			m_event_fd =INVALID_FD;
			setPushable(false);
		}
	}
	void BytesChannel::_read_event_fd(){
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
	bool BytesChannel::_push(const char* p, const int64_t s){
		std::lock_guard<std::mutex> guard(m_mutex);
		if(!m_pushable){ return false; }
		return m_cycle_buffer->push(p, s);
	}
	bool BytesChannel::_pushv(const PMEMORY_SLICE mem, const int64_t n){
		std::lock_guard<std::mutex> guard(m_mutex);
		if(!m_pushable){ return false; }
		return m_cycle_buffer->pushv(mem, n);
	}
	bool BytesChannel::_pop(char* p, const int64_t s){
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_cycle_buffer->pop(p, s);
	}
}
