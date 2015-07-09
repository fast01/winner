/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_bytes(){
		OPH();
		DEBUG("testing bytes ......");
		Bytes* bytes =SafeNew<Bytes>();

		// write
		ToBytes<bool>(bytes, true);
		ToBytes<bool>(bytes, false);

		ToBytes< String* >(bytes, String::NewString("hello world"));

		for(int i=0; i<100; i++){
			ToBytes<int64_t>(bytes, i);
			ToBytes<int64_t>(bytes, 1000 + i);
		}

		// read
		bool true_val =false;
		bool false_val =false;
		CHECK_EXIT(FromBytes(bytes, true_val) && true_val==true, 1);
		CHECK_EXIT(FromBytes(bytes, false_val) && false_val==false, 1);

		String* str_val =0;
		CHECK_EXIT(FromBytes(bytes, str_val) && str_val->is("hello world"), 1);
		
		for(int i=0; i<100; i++){
			int64_t int_val =0;
			CHECK_EXIT(FromBytes(bytes, int_val) && int_val==i, 1);
			CHECK_EXIT(FromBytes(bytes, int_val) && int_val==i+1000, 1);
		}
	}
}
