/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/** Class Requestor **/
	DEFINE_SUPPORT_SCRIPT(Requestor, "core::Requestor")

	/** self **/
	bool Requestor::replyByObject(PACKET& packet, Object* obj){
		packet.option |= OPT_BODY_IS_OBJECT_POINTER;
		return reply(packet, reinterpret_cast< void* >(&obj), sizeof(Object*));
	}
	bool Requestor::send(const char* data, const int64_t data_len){
		const MEMORY_SLICE slice { const_cast< char* >(data), data_len };
		return sendv(&slice, 1);
	}
	int Requestor::_Send(lua_State* L){
		if(lua_gettop(L) < 2){
			lua_pushfstring(L, "fail call %s, invalid arg", __func__);
			lua_error(L);
			return 0;
		}
		Requestor* requestor =0;
		if(!get_object_from_lua<Requestor>(L, 1, requestor)){
			lua_pushfstring(L, "fail call %s, invalid Requestor", __func__);
			lua_error(L);
			return 0;
		}
		size_t len =0;
		const char* szcontent =lua_tolstring(L, 2, &len);
		if(!szcontent){
			lua_pushfstring(L, "fail call %s, invalid content", __func__);
			lua_error(L);
			return 0;
		}

		// call
		const bool ok =requestor->send(szcontent, len);
		lua_pushboolean(L, ok ? 1 : 0);
		return 1;
	}
	int Requestor::_Reply(lua_State* L){
		if(lua_gettop(L) < 3){
			lua_pushfstring(L, "fail call %s, invalid arg", __func__);
			lua_error(L);
			return 0;
		}
		Requestor* requestor =0;
		if(!get_object_from_lua<Requestor>(L, 1, requestor)){
			lua_pushfstring(L, "fail call %s, invalid Requestor", __func__);
			lua_error(L);
			return 0;
		}
		PACKET packet;
		if(!get_packet_from_lua(L, 2, packet)){
			lua_pushfstring(L, "fail call %s, invalid packet", __func__);
			lua_error(L);
			return 0;
		}
		Bytes* bs=0;
		if(!get_object_from_lua<Bytes>(L, 3, bs)){
			lua_pushfstring(L, "fail call %s, invalid body", __func__);
			lua_error(L);
			return 0;
		}

		// call
		requestor->reply(packet, bs->data(), bs->size());
		return 0;
	}
	int Requestor::_ReplyByObject(lua_State* L){
		if(lua_gettop(L) < 3){
			lua_pushfstring(L, "fail call %s, invalid arg", __func__);
			lua_error(L);
			return 0;
		}
		Requestor* requestor =0;
		if(!get_object_from_lua<Requestor>(L, 1, requestor)){
			lua_pushfstring(L, "fail call %s, invalid Requestor", __func__);
			lua_error(L);
			return 0;
		}
		PACKET packet;
		if(!get_packet_from_lua(L, 2, packet)){
			lua_pushfstring(L, "fail call %s, invalid packet", __func__);
			lua_error(L);
			return 0;
		}
		Object* obj=0;
		if(!get_object_from_lua<Object>(L, 3, obj)){
			lua_pushfstring(L, "fail call %s, invalid object", __func__);
			lua_error(L);
			return 0;
		}

		// call
		requestor->replyByObject(packet, obj);
		return 0;
	}
	bool Requestor::RegisterToLua(lua_State* L){
		CLASS_FUNC func[3] ={
			{ "send", &_Send},
			{ "reply", &_Reply},
			{ "replyByObject", &_ReplyByObject},
		};
		return register_class_to_lua(L, "Core", "Requestor", MAKE_LUA_METATABLE_NAME(core::Requestor), 0,
			0, 0,
			3, func,
			0, 0
		);
	}
}
