/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void run_test(){
		OPH();
		DEBUG("Begin test!!!!!!");

		test_memory();
		test_bytes();
		test_bit_set();
		test_string();
		test_array();
		test_hash();
		test_cycle_buffer();
		test_encode_decode();
		test_protocol();
		test_lua();
		test_url();
		test_file_system();

		DEBUG("End test......");
	}
}
