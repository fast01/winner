/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_TYPES_H__
#define H_CORE_TYPES_H__

namespace core{
	/** MEMORY SLICE **/
	typedef struct tagMEMORY_SLICE{
		char* ptr;
		int64_t size;
	}MEMORY_SLICE, *PMEMORY_SLICE;
}

#endif
