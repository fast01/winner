/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"
namespace core{
	/** Class ProtocolBaset **/
	DEFINE_SUPPORT_SCRIPT(ProtocolBase, "core::ProtocolBase")

	/** self **/
	bool ProtocolBase::RegisterToLua(lua_State* L){
		return register_class_to_lua(L, "Core", "ProtocolBase", MAKE_LUA_METATABLE_NAME(core::ProtocolBase), 0,
			0, 0,
			0, 0,
			0, 0
		);
	}
}
