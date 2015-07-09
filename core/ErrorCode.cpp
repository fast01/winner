/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	String* ErrorCode::Code2Desc(const int64_t code){
		return String::Format("%lld", (long long)code);
	}
}
