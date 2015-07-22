/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** CycleBuffer impl ***/
	/** ctor & dtor **/
	CycleBuffer::CycleBuffer()
		: m_data(0)
		, m_size(0)
		, m_read_cursor(0)
		, m_write_cursor(0)
		, m_capacity(0){
	}
	CycleBuffer::~CycleBuffer(){
	}

	/** Object **/
	void CycleBuffer::finalize(){
		DEALLOCATE(m_data);
		m_size=0;
		m_read_cursor =0;
		m_write_cursor =0;
		m_capacity =0;
		Super::finalize();
	}
	Object* CycleBuffer::clone(){
		CycleBuffer* cb =SafeNew<CycleBuffer>();
		cb->m_size =m_size;
		cb->m_read_cursor =m_read_cursor;
		cb->m_write_cursor =m_write_cursor;
		cb->m_capacity =m_capacity;
		cb->m_data =reinterpret_cast< char* >(ALLOCATE(m_capacity));
		memcpy(cb->m_data, m_data, m_capacity);
		return cb;
	}

	/** self **/
	bool CycleBuffer::setCapacity(const int64_t capacity){
		return _set_capacity(capacity);
	}
	bool CycleBuffer::push(const char* p, const int64_t s){
		return _push(p, s);
	}
	bool CycleBuffer::pushv(const MEMORY_SLICE* mem, const int64_t n){
		return _pushv(mem, n);
	}

	bool CycleBuffer::pop(char* p, const int64_t s){
		return _pop(p, s);
	}
	int64_t CycleBuffer::tryPop(char* p, const int64_t s){
		return _try_pop(p, s);
	}

	bool CycleBuffer::pick(char* p, const int64_t s){
		return _pick(p, s);
	}
	int64_t CycleBuffer::tryPick(char* p, const int64_t s){
		return _try_pick(p, s);
	}

	bool CycleBuffer::skip(const int64_t s){
		return _pop(0, s);
	}
	bool CycleBuffer::clear(){
		const int64_t s =m_size;
		return _pop(0, s);
	}
	bool CycleBuffer::align(){
		return _align();
	}
	int64_t CycleBuffer::flushIn(PFN_FLUSH_IN pfn, void* ctx){
		return _flush_in(pfn, ctx);
	}
	bool CycleBuffer::flushOut(PFN_FLUSH_OUT pfn, void* ctx){
		return _flush_out(pfn, ctx);
	}

	/** getter **/
	int64_t CycleBuffer::getReadCursor(){
		return m_read_cursor;
	}
	int64_t CycleBuffer::getWriteCursor(){
		return m_write_cursor;
	}
	int64_t CycleBuffer::getSize(){
		return m_size;
	}
	int64_t CycleBuffer::getCapacity(){
		return m_capacity;
	}
	char* CycleBuffer::c_str(){
		return m_data;
	}
	void* CycleBuffer::data(){
		return m_data;
	}

	/** private **/
	bool CycleBuffer::_set_capacity(int64_t capacity){
		if(capacity < 0) capacity =0;
		capacity =((capacity%32) ? ((capacity/32 + 1)*32) : capacity);
		if(capacity<m_size){
			return false;	
		}
		if(capacity == 0){
			DEALLOCATE(m_data);
			m_data =0;
		}
		else{
			char* data =reinterpret_cast< char* >(ALLOCATE(capacity));
			if(m_size > 0){
				const int64_t tail =m_capacity-m_read_cursor;
				if(tail >= m_size){
					memcpy(data, m_data+m_read_cursor, m_size);	
				}
				else{
					memcpy(data, m_data+m_read_cursor, tail);	
					memcpy(data+tail, m_data, m_size-tail);	
				}
			}
			DEALLOCATE(m_data);
			m_data =data;
		}
		m_capacity =capacity;
		m_read_cursor =0;
		m_write_cursor =m_size;
		if(m_capacity > 0){
			m_write_cursor %=m_capacity;
		}
		return true;
	}
	bool CycleBuffer::_push(const char* p, const int64_t s){
		if(!p || s<=0) return false;
		if(m_size+s > m_capacity){
			if(!_set_capacity(m_size+s)){
				return false;
			}
		}
		const int64_t tail =m_capacity - m_write_cursor;
		if(tail >= s){
			memcpy(m_data+m_write_cursor, p, s);
		}
		else{
			memcpy(m_data+m_write_cursor, p, tail);
			memcpy(m_data, p+tail, s-tail);
		}
		m_write_cursor +=s;
		if(m_capacity > 0){
			m_write_cursor %=m_capacity;
		}
		m_size +=s;
		return true;
	}
	bool CycleBuffer::_pushv(const MEMORY_SLICE* mem, const int64_t n){
		// prepare capacity
		if(!mem || n<=0) return false;
		int64_t s =0;
		for(int64_t i=0; i<n; ++i){
			if(mem[i].ptr==0 || mem[i].size<=0){
				continue;
			}
			s +=mem[i].size;
		}
		if(m_size+s > m_capacity){
			if(!_set_capacity(m_size+s)){
				return false;
			}
		}

		// push
		for(int64_t i=0; i<n; ++i){
			if(mem[i].ptr==0 || mem[i].size<=0){
				continue;
			}
			_push(mem[i].ptr, mem[i].size);
		}
		return true;
	}
	bool CycleBuffer::_pick(char* p, const int64_t s){
		if(s<0) return false;
		if(s == 0) return true;
		if(m_size < s) return false;
		if(!p) return true;
		const int64_t tail =m_capacity - m_read_cursor;
		if(tail >= s){
			memcpy(p, m_data+m_read_cursor, s);	
		}
		else{
			memcpy(p, m_data+m_read_cursor, tail);	
			memcpy(p+tail, m_data, s-tail);	
		}
		return true;
	}
	bool CycleBuffer::_pop(char* p, const int64_t s){
		if(s == 0){
			return true;
		}
		if(_pick(p, s)){
			m_read_cursor += s;
			if(m_capacity > 0){
				m_read_cursor %= m_capacity;
			}
			m_size -=s;
			if(m_size == 0){
				m_read_cursor =0;
				m_write_cursor =0;
			}
			return true;
		}
		return false;
	}
	int64_t CycleBuffer::_try_pick(char* p, int64_t s){
		if(!p || s<=0) return 0;
		if(s > m_size) s =m_size;
		_pick(p, s);
		return s;
	}
	int64_t CycleBuffer::_try_pop(char* p, int64_t s){
		if(!p || s<=0) return 0;
		if(s > m_size) s =m_size;
		_pop(p, s);
		return s;
	}
	bool CycleBuffer::_align(){
		if(m_size == 0) return true;
		if(m_read_cursor == 0) return true;
		char* dt =reinterpret_cast< char* >(ALLOCATE(m_size));
		const int64_t size =m_size;
		_pop(dt, size);
		_push(dt, size);
		DEALLOCATE(dt);
		return true;
	}
	int64_t CycleBuffer::_flush_in(PFN_FLUSH_IN pfn, void* ctx){
		if(!pfn){
			return -1;
		}
		if(!_align()){
			return -1;
		}
		const int64_t n =pfn(m_data, m_size, ctx);
		_pop(0, n);
		return n;
	}
	int64_t CycleBuffer::_flush_out(PFN_FLUSH_OUT pfn, void* ctx){
		if(!pfn){
			return -1;
		}
		const int64_t n =pfn(m_data, m_read_cursor, m_size, m_capacity, ctx);
		if(m_size == 0){
			m_read_cursor =0;
			m_write_cursor =0;
		}
		return n;
	}
}
