/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_cycle_buffer(){
		OPH();
		DEBUG("testing cycle buffer ......");
		char data[1024];
		for(uint64_t i=0; i<sizeof(data); ++i){
			data[i] =i;
		}

		CycleBuffer *cb =SafeNew<CycleBuffer>();
		cb->setCapacity(10240);
		cb->push(data, sizeof(data));

		char data1[1024] ={0};
		cb->pop(data1, 1024);
		CHECK_EXIT(0==memcmp(data1, data, sizeof(data)), 1);


		memset(data1, 0, sizeof(data1));
		cb->push(data, sizeof(data));
		cb->pop(data1, 512);
		cb->pop(data1+512, 512);
		CHECK_EXIT(0==memcmp(data1, data, sizeof(data)), 1);
	}
}
