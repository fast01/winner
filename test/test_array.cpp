/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_array(){
		OPH();
		DEBUG("testing array ......");

		Array* a =SafeNew<Array>();
		for(int64_t i=0; i<14; ++i){
			a->push_back(SafeNew<Int32>());
		}
		for(int64_t i=0; i<14; ++i){
			a->pop_back();
		}
		ASSERT(a->empty());
		
		// common array
		{
			Array* arr =SafeNew<Array>();

			// push_back, size, pop_front, front
			for(int i=0; i<100; ++i){
				arr->push_back(String::Format("%d", i));
			}
			for(int i=0; i<100; ++i){
				CHECK_EXIT(((String*)(arr->front()))->is(String::Format("%d", i)), 1);
				arr->pop_front();
			}
			CHECK_EXIT(arr->size()==0, 1);

			// push_front, size, pop_back, back
			for(int i=99; i>=0; --i){
				arr->push_front(String::Format("%d", i));
			}
			CHECK_EXIT(arr->size()==100, 1);
			for(int i=99; i>=0; --i){
				CHECK_EXIT(((String*)(arr->back()))->is(String::Format("%d", i)), 1);
				arr->pop_back();
			}
			CHECK_EXIT(arr->size()==0, 1);

			// insert, remove
			for(int i=0; i<100; ++i){
				arr->push_back(SafeNew<Int64, int64_t>(i));
			}
			arr->push_front(SafeNew<Int64, int64_t>(-1));
			arr->push_back(SafeNew<Int64, int64_t>(100));
			for(int i=0; i<102; ++i){
				CHECK_EXIT(((Int64*)(arr->get(i)))->getValue() == i-1, 1);
			}
			arr->insert(50, SafeNew<Int64, int64_t>(9999));
			CHECK_EXIT(((Int64*)(arr->get(50)))->getValue() == 9999, 1);
			CHECK_EXIT(((Int64*)(arr->get(51)))->getValue() == 49, 1);
			CHECK_EXIT(((Int64*)(arr->get(49)))->getValue() == 48, 1);
			arr->remove(102);
			arr->remove(50);
			arr->remove(0);
			for(int i=0; i<100; ++i){
				CHECK_EXIT(((Int64*)(arr->front()))->getValue() == i, 1);
				arr->pop_front();
			}
			CHECK_EXIT(arr->size()==0, 1);
		}

		// int64 array
		{
			Int64Array* arr =SafeNew<Int64Array>();

			// push_back, size, pop_front, front
			for(int i=0; i<100; ++i){
				arr->push_back(i);
			}
			for(int i=0; i<100; ++i){
				CHECK_EXIT(arr->front() == i, 1);
				arr->pop_front();
			}
			CHECK_EXIT(arr->size()==0, 1);

			// push_front, size, pop_back, back
			for(int i=99; i>=0; --i){
				arr->push_front(i);
			}
			CHECK_EXIT(arr->size()==100, 1);
			for(int i=99; i>=0; --i){
				CHECK_EXIT(arr->back() == i, 1);
				arr->pop_back();
			}
			CHECK_EXIT(arr->size()==0, 1);

			// insert, remove
			for(int i=0; i<100; ++i){
				arr->push_back(i);
			}
			arr->push_front(-1);
			arr->push_back(100);
			for(int i=0; i<102; ++i){
				CHECK_EXIT(arr->get(i) == i-1, 1);
			}
			arr->insert(50, 9999);
			CHECK_EXIT(arr->get(50) == 9999, 1);
			CHECK_EXIT(arr->get(51) == 49, 1);
			CHECK_EXIT(arr->get(49) == 48, 1);
			arr->remove(102);
			arr->remove(50);
			arr->remove(0);
			for(int i=0; i<100; ++i){
				CHECK_EXIT(arr->front() == i, 1);
				arr->pop_front();
			}
			CHECK_EXIT(arr->size()==0, 1);
		}
	}
}
