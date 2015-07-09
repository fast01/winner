/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ERROR_H__
#define H_CORE_ERROR_H__

#include "core.h"

namespace core{
	int64_t get_last_error(){
		return errno;
	}
	thread_local static char g_error_info[128] ={0};
	const char* get_last_error_desc(){
		if(const char* sz =strerror_r(errno, g_error_info, 127)){
			return sz;
		}
		return "strerror_r call error";
	}
}

#endif
