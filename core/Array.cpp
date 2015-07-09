/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** Array impl ***/
	/** ctor & dtor **/
	Array::Array()
		: m_pObjArray(0)
		, m_size(0)
		, m_capacity(0){
	}
	Array::~Array(){
	}

	/** Object **/
	void Array::finalize(){
		for(int64_t i=0; i<m_size; ++i){
			if(Object* o=at(i)){
				o->release();
			}
		}
		if(m_pObjArray){
			DEALLOCATE(m_pObjArray);
			m_pObjArray =0;
			m_size =0;
			m_capacity =0;
		}
		Super::finalize();
	}
	Object* Array::clone(){
		Array* arr =core::SafeNew<Array>();
		arr->reserve(m_capacity);
		for(int64_t i=0; i<m_size; ++i){
			if(m_pObjArray[i]){
				arr->m_pObjArray[i] =m_pObjArray[i]->clone();
				RETAIN_POINTER(arr->m_pObjArray[i]);
			}
			else{
				arr->m_pObjArray[i] =0;
			}
		}
		return arr;
	}

	/** Container **/
	int64_t Array::size(){
		return m_size;
	}
	bool Array::empty(){
		return m_size == 0;
	}
	Iterator* Array::iterator(){
		return SafeNew<ArrayIterator>(this);
	}

	/** length **/
	int64_t Array::Length(Array* arr){
		return arr ? arr->m_size : 0;
	}

	/** self **/
	Object* Array::at(const int64_t idx){
		return (idx>=0 && idx<m_size) ?
			m_pObjArray[idx] : 0;
	}
	Object* Array::get(const int64_t idx){
		return at(idx);
	}
	int64_t Array::capacity(){
		return m_capacity;
	}
	Object* Array::front(){
		return at(0);
	}
	Object* Array::back(){
		return at(m_size-1);
	}
	int64_t Array::firstIndexOf(Object* o){
		for(int64_t i=0; i<m_size; ++i){
			if(Object::Equals(o, m_pObjArray[i])){
				return i;
			}
		}
		return -1;
	}
	int64_t Array::lastIndexOf(Object* o){
		for(int64_t i=m_size-1; i>=0; --i){
			if(Object::Equals(o, m_pObjArray[i])){
				return i;
			}
		}
		return -1;
	}

	void Array::push_front(Object* o){
		insert(0, o);
	}
	void Array::pop_front(){
		remove(0);
	}
	void Array::push_back(Object* o){
		insert(m_size, o);
	}
	void Array::pop_back(){
		remove(m_size-1);
	}

	void Array::set(const int64_t pos, Object* o){
		if(pos>=0 && pos<m_size){
			ASSIGN_POINTER(m_pObjArray[pos], o);
		}
	}
	void Array::remove(const int64_t pos){
		if(pos<0 || pos>=m_size) return;
		RELEASE_POINTER(m_pObjArray[pos]);
		for(int64_t i=pos+1; i<m_size; i++){
			m_pObjArray[i-1] =m_pObjArray[i];
		}
		m_pObjArray[--m_size] =0;
	}
	void Array::insert(const int64_t pos, Object* o){
		if(!(pos>=0 && pos<=m_size)) return;
		resize(m_size+1);
		for(int64_t i=m_size-1; i>pos; i--){
			m_pObjArray[i] =m_pObjArray[i-1];
		}
		m_pObjArray[pos] =o;
		RETAIN_POINTER(o);
	}
	void Array::join(Array* arr){
		if(!arr || arr==this) return;
		const int64_t src_cnt =arr->m_size;
		reserve(m_size + src_cnt);
		for(int64_t i=0; i<src_cnt; ++i){
			push_back(arr->at(i));
		}
	}
	void Array::clear(){
		resize(0);
	}
	int64_t Array::sort(const PFN_OBJECT_LESS fn){
		if(m_size > 0){
			for(int64_t i=0; 1<m_size; ++i){
				if(false == IS_A(m_pObjArray[i], Compareable)){
					return -1;
				}
			}
			std::sort<Object**, LessObject>(m_pObjArray, m_pObjArray+m_size, LessObject(fn));
		}
		return 0;
	}
	void Array::removeEmpty(){
		for(int64_t i=m_size-1; i>=0; --i){
			if(m_pObjArray[i] == 0){
				remove(i);
			}
		}
	}
	void Array::removeEquals(Object* o){
		for(int64_t i=m_size-1; i>=0; --i){
			if(Object::Equals(o, m_pObjArray[i])){
				remove(i);
			}
		}
	}
	void Array::removeIf(bool (*pfn)(Object*)){
		if(!pfn) return;
		for(int64_t i=m_size-1; i>=0; --i){
			if(pfn(m_pObjArray[i])){
				remove(i);
			}
		}
	}
	void Array::forEach(void (*pfn)(const int64_t, Object*, void*), void* userdata){
		if(!pfn) return;
		for(int64_t i=m_size-1; i>=0; --i){
			pfn(i, at(i), userdata);
		}
	}

	//** space ctrl **//
	void Array::reserve(const int64_t tocapacity){
		int64_t c =tocapacity;
		if(c < 0) c=0;
		c =(c%32) ? (((c/32)+1)*32) : c;
		if(c <= m_capacity){
			return;
		}
		m_pObjArray =reinterpret_cast< Object** >(REALLOCATE(m_pObjArray, c*sizeof(Object*)));
		memset(m_pObjArray + m_capacity, 0, (size_t)(sizeof(Object*)*(c-m_capacity)));
		m_capacity =c;
	}
	void Array::resize(const int64_t tosize){
		int64_t s =tosize;
		if(s < 0) s =0;
		const int64_t old_size =m_size;
		reserve(s);
		for(int64_t i=s; i<old_size; ++i){
			CLEAN_POINTER(m_pObjArray[i]);
		}
		for(int64_t i=old_size; i<s; ++i){
			m_pObjArray[i] =0;
		}
		m_size =s;
	}
	void Array::optimize(int64_t gap){
		if(gap < 32) gap =32;
		// check and calc new capacity
		int64_t c =m_size;
		c =(c%gap) ? (((c/gap)+1)*gap) : c;
		if(c < gap){
			c =gap;
		}
		if(c == m_capacity){
			return;
		}

		// allocate and copy
		Object** pObjArray =reinterpret_cast< Object** >(ALLOCATE(c * sizeof(Object*)));
		memset(pObjArray, 0, (size_t)(c * sizeof(Object*)));
		if(m_size > 0){
			memcpy(pObjArray, m_pObjArray, (size_t)(m_size * sizeof(Object*)));
		}
		DEALLOCATE(m_pObjArray);
		m_pObjArray =pObjArray;
		m_capacity =c;
	}

	/** ArrayIterator **/
	/** ctor & dtor **/
	ArrayIterator::ArrayIterator()
		: m_array(0)
		, m_cursor(-1){
		bind(0);
	}
	ArrayIterator::ArrayIterator(Array* arr)
		: m_array(0)
		, m_cursor(-1){
		bind(arr);
	}
	/** Object **/
	void ArrayIterator::finalize(){
		bind(0);
		Super::finalize();
	}
	/** Iterator **/
	bool ArrayIterator::next(){
		if(m_array == 0 || m_array->empty()) return false;
		if(m_cursor < m_array->size()){
			m_cursor +=1;
		}
		return getValue() != 0;
	}
	Object* ArrayIterator::getValue(){
		return (m_array && (m_cursor>=0&&m_cursor<m_array->size())) ? m_array->at(m_cursor) : 0;
	}
	Container* ArrayIterator::getContainer(){
		return m_array;
	}
	/** Self **/
	void ArrayIterator::reset(){
		m_cursor =-1;
	}
	void ArrayIterator::remove(){
		if(!m_array || m_cursor<0 || m_cursor>=m_array->size()) return;
		m_array->remove(m_cursor);
		m_cursor -=1;
	}
	Array* ArrayIterator::getArray(){
		return m_array;
	}
	int64_t ArrayIterator::getCursor(){
		return m_cursor;
	}
	void ArrayIterator::bind(Array* arr){
		ASSIGN_POINTER(m_array, arr);
		m_cursor =-1;
	}
}
