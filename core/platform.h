/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_PLATFORM_H__
#define H_PLATFORM_H__

extern "C"{
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/eventfd.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <netdb.h>
#include <iconv.h>
#include <execinfo.h>
#include <ucontext.h>
}
#include "build_in_type.hpp"
using namespace core;

/*** functional ***/
void OutputDebug(const char* fmt, ...);
#define PLATFORM_NAME "linux"

/*** init & clean ***/
void init_platform();
void clean_platform();

#endif
