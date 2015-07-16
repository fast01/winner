/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ERROR_CODE_H__
#define H_CORE_ERROR_CODE_H__

namespace core{
	class String;
	class ErrorCode{
	public:
		static String* Code2Desc(const int64_t code);
	public:
		enum{
			OK = 0,
			ERROR = 1,
			SYSTEM_ERROR = 2,
			SYSTEM_BUSY = 3,
			INVALID_ARG = 4,
			UNKNOWN_COMMAND = 5,
			TIMEOUT = 6,
			INVALID_BINARY_DATA = 7,
			NOT_EXIST = 8,
			ALREADY_EXIST = 9,
			NOT_ENOUGH = 10,
			EMPTY = 11,
			CANCELLED = 12,
			NOT_CONFIG = 13,
			COROUTINE_CLEAN = 14,
			COROUTINE_SIGN_MISSMATCH =15,
		};
	};
}

#endif
