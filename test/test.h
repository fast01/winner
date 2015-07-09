/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_TEST_H__
#define H_TEST_H__

#include <stdio.h>
#include "../core/core.h"

namespace test{
	void run_test();
}

#include "test_memory.h"
#include "test_bytes.h"
#include "test_bit_set.h"
#include "test_string.h"
#include "test_array.h"
#include "test_list.h"
#include "test_hash.h"
#include "test_cycle_buffer.h"
#include "test_encode_decode.h"
#include "test_protocol.h"
#include "test_lua.h"
#include "test_url.h"
#include "test_file_system.h"

#endif
