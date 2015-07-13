/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_SETUP_H__
#define H_CORE_SETUP_H__

// comment next line to disable valgrind check
#define VALGRIND_CHECK_ENABLE 1

// special the first generated connection id
#define CONNECTION_ID_GEN_BEGIN 100000

// special http connection keep live time 
#define HTTP_CONNECTION_KEEP_LIVE_TIME 10

// special lua version
#define LUA_SCRIPT_VERSION 530

// memory barrier
#define memory_barrier() __asm__ __volatile__("": : :"memory")


#endif
