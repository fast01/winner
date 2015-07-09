/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LAST_ERROR_H__
#define H_CORE_LAST_ERROR_H__

namespace core{
	class String;
	int64_t get_last_error();
	const char* get_last_error_desc();
}

#endif
