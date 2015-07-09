/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	///*** Hash Impl ***///
	//** ctor & dtor **//
	Hash::Hash()
		: m_pSlotArray(0)
		, m_slot_count(0)
		, m_item_count(0){
	}
	Hash::~Hash(){
	}

	/** Object **/
	void Hash::init(){
		Super::init();
	}
	Object * Hash::clone(){
		Hash* new_obj =core::SafeNew<Hash>();
		HashIterator* it =dynamic_cast<HashIterator*>(this->iterator());
		while(it->next()){
			new_obj->set(it->getKey(), it->getValue());
		}
		return new_obj;
	}
	void Hash::finalize(){
		clear();
		Super::finalize();
	}

	/** Container **/
	int64_t Hash::size(){
		return m_item_count;
	}
	bool Hash::empty(){
		return size() == 0;
	}
	Iterator* Hash::iterator(){
		HashIterator* it =SafeNew<HashIterator>();
		it->bind(this);
		return it;
	}

	//** self **//
	Object* Hash::get(const int64_t key){
		if(!m_item_count) return 0;
		const int64_t code =Int64::CalcHashCode(key);
		const int64_t slot =_code_to_slot(code);
		if(slot < 0) return 0;
		// try find exist
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(Int64* num =dynamic_cast<Int64*>(node->key)){
				if(num->getValue() == key){
					return node->value;
				}
			}
			node =node->next;
		}
		return 0;
	}
	Object* Hash::get(const char* key){
		if(!key || !m_item_count) return 0;
		const int64_t code =String::CalcHashCode(key, -1);
		const int64_t slot =_code_to_slot(code);
		if(slot < 0) return 0;
		// try find exist
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(String* str =dynamic_cast<String*>(node->key)){
				if(strcmp(str->c_str(), key) == 0){
					return node->value;
				}
			}
			node =node->next;
		}
		return 0;
	}
	Object* Hash::get(Object* key){
		if(!key || !m_item_count) return 0;
		const int64_t slot =_key_to_slot(key);
		if(slot < 0) return 0;
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(Object::Equals(key, node->key)){
				return node->value;
			}
			node =node->next;
		}
		return 0;
	}

	//** set **//
	void Hash::set(const int64_t key, Object* val){
		set(SafeNew<Int64>(key), val);
	}
	void Hash::set(const char* key, Object* val){
		if(!key) return;
		set(StringTable::Get(key), val);
	}
	void Hash::set(Object* key, Object* val){
		if(!key) return;
		if(!m_slot_count){
			_try_rehash();
		}
		const int64_t slot =_key_to_slot(key);
		// try find exist
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(Object::Equals(key, node->key)){
				ASSIGN_POINTER(node->value, val);
				return;
			}
			node =node->next;
		}

		// try rehash
		_try_rehash();

		// new node
		PNODE new_node=reinterpret_cast<PNODE>(ALLOCATE(sizeof(NODE)));
		new_node->prev =0;
		if(m_pSlotArray[slot]) m_pSlotArray[slot]->prev =new_node;
		new_node->next =m_pSlotArray[slot];
		m_pSlotArray[slot] =new_node;
		m_item_count +=1;

		new_node->key =key->clone();
		if(new_node->key) new_node->key->retain();
		new_node->value =val;
		if(new_node->value) new_node->value->retain();
	}

	//** remove **//
	void Hash::remove(const int64_t key){
		if(!m_item_count) return;

		const int64_t code =Int64::CalcHashCode(key);
		const int64_t slot =_code_to_slot(code);
		if(slot < 0) return;
		// try find exist
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(Int64* num =dynamic_cast<Int64*>(node->key)){
				if(num->getValue() == key){
					_release_node(slot, node);
					break;
				}
			}
			node =node->next;
		}
	}
	void Hash::remove(const char* key){
		if(!key || !m_item_count) return;

		const int64_t code =String::CalcHashCode(key, -1);
		const int64_t slot =_code_to_slot(code);
		if(slot < 0) return;
		// try find exist
		PNODE node =m_pSlotArray[slot];
		while(node){
			if(String* str =dynamic_cast<String*>(node->key)){
				if(strcmp(str->c_str(), key) == 0){
					_release_node(slot, node);
					break;
				}
			}
			node =node->next;
		}
	}
	void Hash::remove(Object* key){
		if(!key || !m_slot_count) return;
		const int64_t slot =_key_to_slot(key);
		PNODE node =m_pSlotArray[slot];
		if(slot < 0) return;
		while(node){
			if(Object::Equals(key, node->key)){
				_release_node(slot, node);
				break;
			}
			node =node->next;
		}
	}
	void Hash::removeIf(bool (*pfn)(Object*, Object*)){
		if(!pfn) return;
		HashIterator* it =dynamic_cast<HashIterator*>(iterator());
		while(it->next()){
			if(pfn(it->getKey(), it->getValue())){
				it->remove();
			}
		}
	}
	void Hash::clear(){
		for(int64_t i=0; i<m_slot_count; ++i){
			PNODE node =m_pSlotArray[i];
			while(node){
				PNODE tmp =node->next;
				RELEASE_POINTER(node->key);
				RELEASE_POINTER(node->value);
				DEALLOCATE(node);
				node =tmp;
			}
		}
		DEALLOCATE(m_pSlotArray);
		m_pSlotArray =0;
		m_slot_count =0;
		m_item_count =0;
	}

	bool Hash::has(const int64_t key){
		return get(key) != 0;
	}
	bool Hash::has(const char* key){
		return get(key) != 0;
	}
	bool Hash::has(Object* key){
		return get(key) != 0;
	}

	void Hash::forEach(void (*pfn)(Object*, Object*, void*), void* userdata){
		if(!pfn) return;
		HashIterator* it =dynamic_cast<HashIterator*>(iterator());
		while(it->next()){
			pfn(it->getKey(), it->getValue(), userdata);
		}
	}
	void Hash::optimize(int64_t gap){
		if(gap < 32) gap =32;
		// save old
		PNODE* pOldeSlotArray =m_pSlotArray;
		const int64_t old_slot_count =m_slot_count;

		// new
		const int64_t n =m_item_count * 3;
		m_slot_count =n%gap ? (n/gap + 1)*gap : n;
		if(m_slot_count < gap){
			m_slot_count =gap;
		}
		if(m_slot_count == old_slot_count){
			return;
		}

		const int64_t s =sizeof(PNODE) * m_slot_count;
		m_pSlotArray =reinterpret_cast< PNODE* >(ALLOCATE(s));
		memset(m_pSlotArray, 0, (size_t)s);
		m_item_count =0;

		// copy
		for(int64_t i=0; i<old_slot_count; ++i){
			PNODE node =pOldeSlotArray[i];
			while(node){
				PNODE tmp =node->next;
				set(node->key, node->value);
				RELEASE_POINTER(node->key);
				RELEASE_POINTER(node->value);
				DEALLOCATE(node);
				node =tmp;
			}
		}
		DEALLOCATE(pOldeSlotArray);
	}
	//** space ctrl **//
	void Hash::_try_rehash(){
		if(m_slot_count==0 || (1+m_item_count)*2>m_slot_count){
			// save old
			PNODE* pOldeSlotArray =m_pSlotArray;
			const int64_t old_slot_count =m_slot_count;

			// new
			m_slot_count =m_slot_count ? m_slot_count*2 : 32;
			const int64_t s =sizeof(PNODE) * m_slot_count;
			m_pSlotArray =reinterpret_cast< PNODE* >(ALLOCATE(s));
			memset(m_pSlotArray, 0, (size_t)s);

			// copy
			m_item_count =0;
			for(int64_t i=0; i<old_slot_count; ++i){
				PNODE node =pOldeSlotArray[i];
				while(node){
					PNODE tmp =node->next;
					set(node->key, node->value);
					RELEASE_POINTER(node->key);
					RELEASE_POINTER(node->value);
					DEALLOCATE(node);
					node =tmp;
				}
			}
			DEALLOCATE(pOldeSlotArray);
		}
	}
	int64_t Hash::_code_to_slot(const int64_t code){
		if(m_slot_count <=0 ) return -1;
		int64_t c =code;
		if(c < 0) c =-c;
		return c % m_slot_count;
	}
	int64_t Hash::_key_to_slot(Object* key){
		if(!key) return -1;
		int64_t code =key->getHashCode();
		return _code_to_slot(code);
	}
	void Hash::_release_node(const int64_t slot, PNODE node){
		RELEASE_POINTER(node->key);
		RELEASE_POINTER(node->value);
		if(node->prev){
			node->prev->next =node->next;
		}
		else{
			m_pSlotArray[slot] =node->next;
		}
		if(node->next){
			node->next->prev =node->prev;
		}
		DEALLOCATE(node);
		m_item_count -=1;
	}

	//*** HashIterator ***//
	//** ctor & dtor **//
	HashIterator::HashIterator()
		: m_hash(0){
		bind(0);
	}
	HashIterator::HashIterator(Hash* hash)
		: m_hash(0){
		bind(hash);
	}
	HashIterator::~HashIterator(){
	}

	//** Object **/
	void HashIterator::init(){
		m_index =-1;
		m_cursor =0;
	}
	Object* HashIterator::clone(){
		HashIterator* it =SafeNew<HashIterator>();
		it->bind(m_hash);
		return it;
	}
	void HashIterator::finalize(){
		bind(0);
		Super::finalize();
	}

	/** Iterator **/
	bool HashIterator::next(){
		if(!m_hash) return false;
		if(m_cursor){
			m_cursor =m_cursor->next;
		}
		if(m_cursor){
			return true;
		}
		if(m_index < m_hash->m_slot_count){
			m_index +=1;
		}

		const int64_t cnt =m_hash->m_slot_count;
		for(; m_index<cnt; ++m_index){
			m_cursor =m_hash->m_pSlotArray[m_index];
			if(m_cursor) break;
		}
		return m_cursor != 0;
	}
	Object* HashIterator::getValue(){
		return m_cursor ?
			m_cursor->value : 0;
	}
	Container* HashIterator::getContainer(){
		return m_hash;
	}

	/** Self **/
	void HashIterator::bind(Hash* hash){
		ASSIGN_POINTER(m_hash, hash);
		m_index =-1;
		m_cursor =0;
	}
	void HashIterator::reset(){
		m_index =-1;
		m_cursor =0;
	}
	void HashIterator::remove(){
		if(!m_cursor) return;
		Hash::PNODE pre_cursor =m_cursor->prev;
		m_hash->remove(m_cursor->key);
		m_cursor =pre_cursor;
		if(!m_cursor){
			m_index -=1;
		}
	}
	Hash* HashIterator::getHash(){
		return m_hash;
	}
	Object* HashIterator::getKey(){
		return m_cursor ?
			m_cursor->key : 0;
	}
}
