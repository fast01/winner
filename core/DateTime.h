/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_DATE_TIME_H__
#define H_CORE_DATE_TIME_H__

namespace core{
	/* stable time, never go back */
	time_t stable_time();

	/** class DateTime **/
	class DateTime{
	public:
		static int64_t Now();
	};
}

#endif
