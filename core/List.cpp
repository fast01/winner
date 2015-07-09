/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/*** List impl ***/
	/** ctor & dtor **/
	List::List()
		: m_head(0)
		, m_tail(0)
		, m_size(0){
	}
	List::~List(){
	}

	/** Object **/
	void List::finalize(){
		clear();
		Super::finalize();
	}

	/** ICloneable **/
	Object* List::clone(){
		List* l =core::SafeNew<List>();
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			l->push_back(node->data);
			node =next;
		}
		return l;
	}

	/** Container **/
	int64_t List::size(){
		return m_size;
	}
	bool List::empty(){
		return m_size == 0;
	}
	Iterator* List::iterator(){
		CHECK_EXIT(0, 1);
		return 0;
	}

	/** self **/
	List::PLIST_NODE List::front(){
		return m_head;
	}
	List::PLIST_NODE List::back(){
		return m_tail;
	}

	List::PLIST_NODE List::push_front(Object* o){
		return insertBefore(m_head, o);
	}
	void List::pop_front(){
		remove(m_head);
	}
	List::PLIST_NODE List::push_back(Object* o){
		return insertAfter(m_tail, o);
	}
	void List::pop_back(){
		remove(m_tail);
	}
	void List::remove(List::PLIST_NODE node){
		if(node == 0) return;
		if(node->prev){
			node->prev->next =node->next;
		}
		else{
			ASSERT(m_head == node);
			m_head =node->next;
			if(m_head && m_head->prev){
				m_head->prev =0;
			}
		}
		if(node->next){
			node->next->prev =node->prev;
		}
		else{
			ASSERT(m_tail == node);
			m_tail =node->prev;
			if(m_tail && m_tail->next){
				m_tail->next =0;
			}
		}
		m_size -=1;
		DELETE_POINTER(node->data);
		DEALLOCATE(node);
	}
	List::PLIST_NODE List::insertBefore(List::PLIST_NODE node_anc, Object* o){
		// new node
		RETAIN_POINTER(o);
		PLIST_NODE node =(PLIST_NODE)ALLOCATE(sizeof(LIST_NODE));
		node->prev =node->next =0;
		node->data =o;

		// insert
		if(0 == node_anc){
			ASSERT(m_size == 0);
			m_head =m_tail =node;
		}
		else{
			ASSERT(m_size > 0);
			if(node_anc->prev){
				node_anc->prev->next =node;
				node->prev =node_anc->prev;
			}
			else{
				m_head =node;
			}
			node->next =node_anc;
			node_anc->prev =node;
		}
		m_size +=1;
		return node;
	}
	List::PLIST_NODE List::insertAfter(List::PLIST_NODE node_anc, Object* o){
		// new node
		RETAIN_POINTER(o);
		PLIST_NODE node =(PLIST_NODE)ALLOCATE(sizeof(LIST_NODE));
		node->prev =node->next =0;
		node->data =o;

		// insert
		if(0 == node_anc){
			ASSERT(m_size == 0);
			m_head =m_tail =node;
		}
		else{
			ASSERT(m_size > 0);
			if(node_anc->next){
				node_anc->next->prev =node;
				node->next =node_anc->next;
			}
			else{
				m_tail =node;
			}
			node->prev =node_anc;
			node_anc->next =node;
		}
		m_size +=1;
		return node;
	}
	void List::join(List* l){
		if(!l) return;
		PLIST_NODE node =l->m_head;
		while(node){
			PLIST_NODE next =node->next;
			push_back(node->data);
			node =next;
		}
	}
	void List::clear(){
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			DELETE_POINTER(node->data);
			DEALLOCATE(node);
			node =next;
		}
		m_head =m_tail =0;
		m_size =0;
	}
	void List::removeEmpty(){
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			if(node->data == 0){
				remove(node);
			}
			node =next;
		}
	}
	void List::removeIf(bool (*pfn)(Object*)){
		if(!pfn) return;
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			if(pfn(node->data)){
				remove(node);
			}
			node =next;
		}
	}
	void List::removeEquals(Object* o){
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			if(Object::Equals(node->data, o)){
				remove(node);
			}
			node =next;
		}
	}
	void List::forEach(void (*pfn)(Object*, void* userdata), void *userdata){
		if(!pfn) return;
		PLIST_NODE node =m_head;
		while(node){
			PLIST_NODE next =node->next;
			pfn(node->data, userdata);
			node =next;
		}
	}
}
