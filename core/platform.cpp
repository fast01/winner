/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

void OutputDebug(const char* fmt, ...){
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	va_end(vl);
	printf("\n");
}
void init_platform(){
	// set time zone
	tzset();

	/*
	sigset_t set;
	sigfillset(&set);
	if(0 != sigprocmask(SIG_BLOCK, &set, 0)){
		perror("fail to call init_platform");
		abort();
	}
	*/
}
void clean_platform(){
}
