/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

/** define **/
#define DEFINE_POD_ARRAY_CLASS(cls_name, type)	\
	cls_name::cls_name():m_data(0), m_size(0), m_capacity(0){}	\
	cls_name::~cls_name(){}	\
	bool cls_name::equals(Object* obj){	\
		if(const cls_name* o =dynamic_cast<cls_name*>(obj)){	\
			if(m_size==0 && o->m_size==0){ return true; }	\
			return ((m_size==o->m_size) ? (memcmp(o->m_data, m_data, (size_t)m_size*sizeof(type))==0) : false);	\
		}	\
		else{	\
			return false;	\
		}	\
	}	\
	Object* cls_name::clone(){	\
		cls_name* obj=SafeNew<cls_name>();	\
		if(m_capacity>0){	\
			const int64_t c =sizeof(type)*m_capacity;	\
			obj->m_data =(type*)ALLOCATE(c);	\
			if(m_size > 0){ \
				memcpy(obj->m_data, m_data, static_cast<size_t>(sizeof(type) * m_size));	\
			} \
		}	\
		obj->m_capacity =m_capacity;	\
		obj->m_size =m_size;	\
		return obj;	\
	}	\
	void cls_name::finalize(){ clear(); Super::finalize(); } \
	void cls_name::clear(){ DEALLOCATE(m_data); m_data =0; m_size =0; m_capacity =0; Super::finalize(); } \
	type cls_name::front(){ return at(0); }	\
	type cls_name::back(){ return at(m_size-1); }	\
	bool cls_name::empty(){ return m_size == 0; }	\
	int64_t cls_name::size(){ return m_size; }	\
	int64_t cls_name::capacity(){ return m_capacity; }	\
	type cls_name::at(const int64_t pos){ return (pos>=0 && pos<m_size) ? m_data[pos] : type(); }	\
	type cls_name::get(const int64_t pos){ return at(pos); }	\
	type* cls_name::data(){ return m_data; }	\
	void cls_name::set(const int64_t pos, type v){ if(pos>=0 && pos<m_size){ m_data[pos]=v; } }	\
	void cls_name::push_back(type v){ insert(m_size, v); }	\
	void cls_name::pop_back(){ remove(m_size-1); }	\
	void cls_name::push_front(type v){ insert(0, v); }	\
	void cls_name::pop_front(){ remove(0); }	\
	void cls_name::remove(const int64_t pos){ remove(pos, 1); }	\
	void cls_name::remove(const int64_t pos, int64_t cnt){	\
		if(pos<0 || pos>=m_size || cnt<=0) return;	\
		if(pos+cnt >= m_size){ m_size =pos; return; }	\
		const int64_t mv_cnt =m_size - pos - cnt;	\
		memmove(m_data+pos, m_data+pos+cnt, (size_t)(mv_cnt*sizeof(type)));	\
		m_size -=cnt;	\
	}	\
	void cls_name::insert(const int64_t pos, type v){ insert(pos, &v, 1); }	\
	void cls_name::insert(const int64_t pos, const type* v, const int64_t cnt){	\
		if(pos<0 || pos>m_size || !v || cnt<=0) return;	\
		reserve(m_size + cnt);	\
		const int64_t mv_cnt =m_size - pos;	\
		if(mv_cnt > 0) memmove(m_data+pos+cnt, m_data+pos, (size_t)(mv_cnt*sizeof(type)));	\
		memcpy(m_data+pos, v, (size_t)(cnt*sizeof(type)));	\
		m_size +=cnt;	\
	}	\
	void cls_name::one(){	\
		if(m_size > 0) memset(m_data, 0xFF, (size_t)(sizeof(type)*m_size));	\
	}	\
	void cls_name::zero(){	\
		if(m_size > 0) memset(m_data, 0, (size_t)(sizeof(type)*m_size));	\
	}	\
	void cls_name::resize(int64_t s){	\
		if(s < 0) s =0;	\
		reserve(s);	\
		if(s > 0)memset(m_data, 0, (size_t)(sizeof(type)*s));	\
		m_size =s;	\
	}	\
	void cls_name::reserve(int64_t c){	\
		if(c <= m_capacity) return;	\
		if(c%16) c = ((c/16)+1)*16;	\
		m_data =reinterpret_cast<type*>(REALLOCATE(m_data, c * sizeof(type)));	\
		m_capacity =c;	\
	}

namespace core{
	//** array **//
	DEFINE_POD_ARRAY_CLASS(BooleanArray, bool)
	DEFINE_POD_ARRAY_CLASS(Float32Array, float32_t)
	DEFINE_POD_ARRAY_CLASS(Float64Array, float64_t)
	DEFINE_POD_ARRAY_CLASS(Int8Array, int8_t)
	DEFINE_POD_ARRAY_CLASS(Int16Array, int16_t)
	DEFINE_POD_ARRAY_CLASS(Int32Array, int32_t)
	DEFINE_POD_ARRAY_CLASS(Int64Array, int64_t)
	DEFINE_POD_ARRAY_CLASS(Uint8Array, uint8_t)
	DEFINE_POD_ARRAY_CLASS(Uint16Array, uint16_t)
	DEFINE_POD_ARRAY_CLASS(Uint32Array, uint32_t)
	DEFINE_POD_ARRAY_CLASS(Uint64Array, uint64_t)
}
