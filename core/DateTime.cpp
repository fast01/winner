/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/* define funcs */
	time_t stable_time(){
		thread_local static time_t t=0;
		const time_t now =time(0);
		if(now > t){
			t =now;
		}
		return t;
	}
	/** impl DateTime  **/
	int64_t DateTime::Now(){
		return stable_time();
	}
}
