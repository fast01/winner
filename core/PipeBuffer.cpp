/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** PipeBuffer impl ***/
	/** ctor & dtor **/
	PipeBuffer::PipeBuffer()
		: CycleBuffer()
		, m_is_closed(false){
	}
	PipeBuffer::~PipeBuffer(){
	}

	/** Object **/
	void PipeBuffer::init(){
		Super::init();
		Super::setCapacity(PIPE_DEFAULT_CAPACITY);
		m_is_closed =false;
	}
	Object* PipeBuffer::clone(){
		return 0;
	}
	void PipeBuffer::finalize(){
		m_is_closed =true;
		Super::finalize();
	}

	/** self **/
	bool PipeBuffer::setCapacity(const int64_t capacity){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::setCapacity(capacity);
	}
	bool PipeBuffer::push(const char* p, const int64_t s){
		std::unique_lock<std::mutex> lock(m_mutex);
		const bool ok =Super::push(p, s);
		m_cv.notify_one();
		return ok;
	}
	bool PipeBuffer::pushv(const PMEMORY_SLICE mem, const int64_t n){
		std::unique_lock<std::mutex> guard(m_mutex);
		const bool ok =Super::pushv(mem, n);
		m_cv.notify_one();
		return ok;
	}
	bool PipeBuffer::pick(char* p, const int64_t s){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::pick(p, s);
	}
	bool PipeBuffer::pop(char* p, const int64_t s){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::pop(p, s);
	}
	bool PipeBuffer::skip(const int64_t s){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::skip(s);
	}
	bool PipeBuffer::clear(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::clear();
	}
	void PipeBuffer::lock(){
		m_mutex.lock();
	}
	void PipeBuffer::unlock(){
		m_mutex.unlock();
	}

	/** getter **/
	int64_t PipeBuffer::getReadCursor(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::getReadCursor();
	}
	int64_t PipeBuffer::getWriteCursor(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::getWriteCursor();
	}
	int64_t PipeBuffer::getSize(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::getSize();
	}
	int64_t PipeBuffer::getCapacity(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return Super::getCapacity();
	}
	char* PipeBuffer::c_str(){
		std::unique_lock<std::mutex> guard(m_mutex);
		return Super::c_str();
	}
	void* PipeBuffer::data(){
		std::unique_lock<std::mutex> guard(m_mutex);
		return Super::data();
	}

	/** close state **/
	void PipeBuffer::close(){
		std::unique_lock<std::mutex> lock(m_mutex);
		m_is_closed =true;
	}
	bool PipeBuffer::isClosed(){
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_is_closed;
	}

	/** event **/
	void PipeBuffer::signal(){
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.notify_one();
	}
	bool PipeBuffer::wait(const int64_t secs){
		std::unique_lock<std::mutex> lock(m_mutex);
		if(Super::getSize() > 0){
			return true;
		}
		if(secs > 0){
			const std::cv_status status =m_cv.wait_for(lock, std::chrono::seconds(secs));
			return status == std::cv_status::no_timeout;
		}
		else if(secs < 0){
			const std::cv_status status =m_cv.wait_for(lock, std::chrono::seconds(WAIT_TIMEOUT));
			return status == std::cv_status::no_timeout;
		}
		else{
			m_cv.wait(lock);
			return true;
		}
	}
}
