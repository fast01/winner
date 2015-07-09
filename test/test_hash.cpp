/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_hash(){
		OPH();
		DEBUG("testing hash ......");
		
		Hash* hash =SafeNew<Hash>();

		// simple set, get, remove, size
		hash->set("name", STR("fool"));
		hash->set("age", SafeNew<Int64, int64_t>(1987));
		CHECK_EXIT(hash->size()==2, 1);
		CHECK_EXIT(((String*)(hash->get("name")))->is("fool"), 1);
		CHECK_EXIT(((Int64*)(hash->get("age")))->getValue()==1987, 1);
		hash->remove("name");
		CHECK_EXIT(hash->size()==1, 1);
		CHECK_EXIT(hash->has("name")==false, 1);

		// hard set, get, remove size
		hash->clear();
		CHECK_EXIT(hash->size()==0, 1);
		for(int64_t i=0; i<100; ++i){
			hash->set(i, SafeNew<Int64, int64_t>(i));
		}
		CHECK_EXIT(hash->size()==100, 1);

		for(int64_t i=0; i<100; ++i){
			CHECK_EXIT(((Int64*)(hash->get(i)))->getValue()==i, 1);
		}
		hash->remove(50);
		CHECK_EXIT(hash->size()==99, 1);
		for(int64_t i=0; i<100; ++i){
			if(i == 50){
				CHECK_EXIT(hash->has(50)==false, 1);
			}
			else{
				CHECK_EXIT(((Int64*)(hash->get(i)))->getValue()==i, 1);
			}
		}
		
		// iterator
		hash->clear();
		CHECK_EXIT(hash->size()==0, 1);

		for(int64_t i=0; i<100; ++i){
			hash->set(i, SafeNew<Int64, int64_t>(i));
		}
		HashIterator* it =(HashIterator*)hash->iterator();
		int64_t sum =0;
		while(it->next()){
			sum +=((Int64*)(it->getValue()))->getValue();
		}
		CHECK_EXIT(sum == 4950, 1);
		it->reset();
		sum =0;
		while(it->next()){
			sum +=((Int64*)(it->getValue()))->getValue();
		}
		CHECK_EXIT(sum == 4950, 1);

		it->reset();
		it->next();
		it->next();
		it->next();
		it->next();
		it->next();
		it->next();
		it->remove();
		sum =0;
		while(it->next()){
			sum +=((Int64*)(it->getValue()))->getValue();
		}
		CHECK_EXIT(sum == 4935, 1);

		it->reset();
		sum =0;
		while(it->next()){
			sum +=((Int64*)(it->getValue()))->getValue();
		}
		CHECK_EXIT(sum == 4945, 1);
	}
}
