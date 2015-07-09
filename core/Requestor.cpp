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
		CLASS_FUNC func[2] ={
			{ "reply", &_Reply},
			{ "replyByObject", &_ReplyByObject},
		};
		return register_class_to_lua(L, "Core", "Requestor", MAKE_LUA_METATABLE_NAME(core::Requestor), 0,
			0, 0,
			2, func,
			0, 0
		);
	}
}
