/******************************************************************************
  * Copyright (C) 别怀山(foolbie).  All rights reserved.
  *
  * Permission is hereby granted, free of charge, to any person obtaining
  * a copy of this software and associated documentation files (the
  * "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish,
  * distribute, sublicense, and/or sell copies of the Software, and to
  * permit persons to whom the Software is furnished to do so, subject to
  * the following conditions:
  *
  * The above copyright notice and this permission notice shall be
  * included in all copies or substantial portions of the Software.
  *
  * 注：对于任何商业公司，在使用该software期间，执行“固定加班制度”超过一个月，上述任何授权都将被无条件收回。
  * “固定加班制度”指公司成文或者不成文规定员工在正常下班后需要加班到指定时间。
  *
  * 代码是我们的第二生命。
  *
  ******************************************************************************/

/******************************************************************************
  * Author info.
  * 中文名: 别怀山
  * name  : foolbie
  * email : lygbhs@126.com;1216917088@qq.com
  * qq    : 1216917088
  *
  ******************************************************************************/

#ifndef H_CORE_H__
#define H_CORE_H__

namespace core{
}

#include "setup.h"

/** C **/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>

/** C++ **/
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <atomic>
#include <typeinfo>

/** libs **/
#include <mysql.h>
#ifdef VALGRIND_CHECK_ENABLE
#include <valgrind/valgrind.h>
#endif
#include <openssl/md5.h>
#include <openssl/sha.h>
/*
#include <google/profiler.h>
#include <google/tcmalloc.h>
*/
#include "../include/lua/lua.hpp"
#include "../include/tinyxml/tinyxml.h"
#include "../include/cJSON/cJSON.h"

/** misc **/
#include "SpinLock.hpp"
#include "platform.h"
#include "types.h"
#include "ids.hpp"
#include "ErrorCode.h"
#include "BitOp.hpp"
#include "packet.hpp"
#include "macro.hpp"
#include "encode.h"
#include "last_error.h"
#include "util.h"
#include "interface_ids.h"
#include "lua_compatible.hpp"

/** default lock type **/
typedef SpinLock DEFAULT_LOCK_TYPE;

/* base class */
#include "Object.h"
#include "Boolean.h"
#include "Compareable.hpp"
#include "String.h"
#include "Pointer.h"
#include "DateTime.h"
#include "BinaryCoder.hpp"
#include "Bytes.h"
#include "number.h"
#include "BitSet.h"
#include "Pair.h"
#include "FileSystem.h"
#include "Timer.h"
#include "Url.h"
#include "Error.h"
#include "RangeId.h"
#include "Requestor.h"
#include "MonitorTarget.h"
#include "CycleBuffer.h"
#include "BytesChannel.h"
#include "PipeBuffer.h"
#include "Channel.h"
#include "RpcInfo.h"
#include "Thread.h"
#include "Monitor.h"
#include "Dispatcher.h"
#include "DispatcherManager.h"

/* container */
#include "Container.hpp"
#include "Array.h"
#include "List.h"
#include "Hash.h"
#include "PODArray.h"

/* class info */
#include "FieldInfoBase.h"
#include "FieldInfo.hpp"
#include "ClassInfo.h"
#include "ObjectFactory.h"

/* encode */
#include "Md5.h"
#include "Sha.h"
#include "Endian.h"
#include "CharsetConvert.h"
#include "XmlForLua.h"
#include "JsonForLua.h"
#include "Base64.h"
#include "UrlEncode.h"

/* db */
#include "MysqlForLua.h"

/* protocol */
#include "ProtocolBase.h"
#include "ProtocolManager.h"

/* functional class */
#include "Logger.h"
#include "ObjectPool.h"
#include "StringTable.h"

#include "Invoker.h"
#include "Event.h"

#include "Command.h"
#include "CommandDesc.h"
#include "Service.h"
#include "CommandService.h"
#include "ServiceManager.h"

/* actor */
#include "Component.h"
#include "Observer.h"
#include "Actor.h"
#include "ObserverManager.h"
#include "ActorManager.h"

/* network */
#include "TcpConnectionRequestor.h"
#include "TcpConnection.h"
#include "TcpListener.h"
#include "Network.h"

/* lua service */
#include "LuaService.h"

/* log service */
#include "LogService.h"

/* callback service */
#include "CallbackRpcInfo.h"
#include "CallbackGroupRpcInfo.h"
#include "CallbackCommandDesc.h"
#include "CallbackService.h"

/* coroutine service */
#include "Coroutine.h"
#include "CoroutineRpcInfo.h"
#include "CoroutineCommandDesc.h"
#include "CoroutinePool.h"
#include "CoroutineService.h"
#include "ParallelNode.h"
#include "Parallel.h"

/* CService */
#include "CService.h"

/* HttpService */
#include "HtmlTemplate.h"
#include "HttpCookie.h"
#include "HttpRequest.h"
#include "HttpRespond.h"
#include "HttpClient.h"
#include "HttpClientForLua.h"
#include "HttpConnection.h"
#include "HttpListener.h"
#include "HttpService.h"

/* local */
#include "ThreadLocal.h"
#include "ProcessLocal.h"

/* ApplicationBase */
#include "ApplicationBase.h"

/* template */
#include "template.hpp"
#include "tostring.hpp"
#include "fromstring.hpp"
#include "tolua.hpp"
#include "fromlua.hpp"
#include "tobytes.hpp"
#include "frombytes.hpp"
#include "lua_helper.hpp"

/* gen code */
#include "gen/protocol/core_protocol.h"
#include "gen/cpplua/core_cpplua.h"

#endif
