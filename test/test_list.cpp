/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_list(){
		OPH();
		DEBUG("testing list ......");

		// push pop
		List* list =SafeNew<List>();
		for(int64_t i=0; i<14; ++i){
			Int64* v =SafeNew<Int64>(i);
			list->push_back(v);
			ASSERT(v->getValue() == i);
		}
		List* list1 =(List*)list->clone();
		int64_t x =0;
		List::PLIST_NODE node =list->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);
			ASSERT(v->getValue() == x++);
			node =node->next;
		}
		node =0;
		for(int64_t i=0; i<14; ++i){
			list->pop_back();
		}
		ASSERT(list->empty());

		// pop front
		list1->pop_front();
		x =1;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);
			ASSERT(v->getValue() == x++);
			node =node->next;
		}
		// push front
		list1->push_front(SafeNew<Int64>(0));
		x =0;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);
			ASSERT(v->getValue() == x++);
			node =node->next;
		}

		// pop back
		list1->pop_back();
		x =0;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);
			ASSERT(v->getValue() == x++);
			node =node->next;
		}
		ASSERT(x == 13);

		// push back
		list1->push_back(SafeNew<Int64>(13));
		x =0;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);
			ASSERT(v->getValue() == x++);
			node =node->next;
		}
		ASSERT(x == 14);

		// remove and insert after
		node =list1->front()->next;
		list1->remove(node);
		auto it =list1->insertAfter(list1->front(), SafeNew<Int64>(100));
		ASSERT(((Int64*)(it->data))->getValue() == 100);
		x =0;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);

			if(x == 1){
				ASSERT(v->getValue() == 100);
			}
			else{
				ASSERT(v->getValue() == x);
			}
			x++;
			node =node->next;
		}
		ASSERT(x == 14);

		// remove and insert before
		node =list1->front()->next;
		list1->remove(node);
		it =list1->insertBefore(list1->front()->next, SafeNew<Int64>(100));
		ASSERT(((Int64*)(it->data))->getValue() == 100);
		x =0;
		node =list1->front();
		while(node){
			Int64* v =dynamic_cast< Int64* >(node->data);
			ASSERT(v);

			if(x == 1){
				ASSERT(v->getValue() == 100);
			}
			else{
				ASSERT(v->getValue() == x);
			}
			x++;
			node =node->next;
		}
		ASSERT(x == 14);
	}
}
