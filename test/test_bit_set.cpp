/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_bit_set(){
		OPH();
		::core::BitSet* bs =SafeNew<::core::BitSet>();
		ASSERT(bs->count() == 0);
		for(uint64_t i=0; i<1024; ++i){
			bs->set(i);
		}
		ASSERT(bs->count() == 1024);

		bs->flip(100);
		ASSERT(bs->test(100) == false);

		bs->flip(100);
		ASSERT(bs->test(100));

		bs->clear(64);
		ASSERT(bs->test(64) == false);

		bs->set(63);
		ASSERT(bs->test(63));

		bs->replace(192, 64, 126);
		ASSERT(bs->fetch(192, 64) == 126);

		bs->replace(192, 15, 126);
		ASSERT(bs->fetch(192, 15) == 126);

		bs->replace(15, 15, 126);
		ASSERT(bs->fetch(15, 15) == 126);

		bs->replace(128, 51, 99);
		ASSERT(bs->fetch(128, 51) == 99);

		bs->replace(12, 52, 199);
		ASSERT(bs->fetch(12, 52) == 199);

		// bs->clear();
		bs->replace(62, 4, 1);
		ASSERT(bs->fetch(62, 2) == 1);
		ASSERT(bs->fetch(64, 2) == 0);
		ASSERT(bs->fetch(62, 4) == 1);

		bs->clear();
		ASSERT(bs->count() == 0);

		bs->set(1);
		bs->set(3);
		ASSERT(bs->count() == 2);
		
		ASSERT(bs->toString()->is("0101"));

		bs->fromString(STR("0101"));
		ASSERT(bs->test(0) == false);
		ASSERT(bs->test(1));
		ASSERT(bs->test(2) == false);
		ASSERT(bs->test(3));
	}
}
