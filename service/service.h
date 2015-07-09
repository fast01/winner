/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_SERVICE_H__
#define H_SERVICE_H__

#include "../core/core.h"
#include "../gen/protocol/protocol.h"

namespace service{
	void register_service();
}

#include "./service_ids.h"
#include "./s2_callback/s2_callback.h"
#include "./s3_coroutine/s3_coroutine.h"

#endif
