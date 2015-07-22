/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LUA_HELPER_H__
#define H_CORE_LUA_HELPER_H__

namespace core{
	/** Top helper **/
	class LuaTopHelper{
	public:
		LuaTopHelper(lua_State* L)
			: m_L(L)
			, m_top(0){
			if(m_L){
				m_top =lua_gettop(m_L);
			}
		}
		~LuaTopHelper(){
			if(m_L){
				lua_settop(m_L, m_top);
			}
		}
	private:
		lua_State* m_L;
		int m_top;
	};

	/** push_object_to_lua **/
	template<typename T>
	inline bool push_object_to_lua(lua_State* L, typename std::remove_pointer< T >::type* obj){
		if(!obj){
			lua_pushnil(L);
			return true;
		}
		/** create userdata **/
		void** ud =(void**)lua_newuserdata(L, sizeof(void*));
		if(!ud){
			return false;
		}
		Object* ptr =dynamic_cast< Object* >(obj);
		*ud =ptr;

		/** set metatable **/
		lua_getglobal(L, obj->getLuaMetatableName());
		if(lua_istable(L, -1)){
			lua_setmetatable(L, -2);
			obj->retain();
			return true;
		}
		else{
			lua_pop(L, 2);
			return false;
		}
	}
	/** get_object_from_lua **/
	template<typename T>
	inline bool get_object_from_lua(lua_State* L, const int idx, typename std::remove_pointer< T >::type*& obj){
		if(!L){
			return false;
		}
		if(!lua_isuserdata(L, idx)){
			return false;
		}
		void** ud =(void**)lua_touserdata(L, idx);
		if(!ud){
			return false;
		}
		Object* ptr =reinterpret_cast< Object* >(*ud);
		obj =dynamic_cast< typename std::remove_pointer< T >::type* >(ptr);
		return true;
	}

	/** ensure **/
	inline bool ensure_lua_global_table(lua_State* L, const char* name){
		// check and prepare
		if(!L || !name) return false;
		const int len =strlen(name);
		if(len <= 0) return false;
		Bytes* bs =SafeNew<Bytes>();
		bs->set(name, len + 1);

		char* sz =bs->c_str();
		char* saveptr =0;

		// process
		char* token =strtok_r(sz, ".", &saveptr);
		if(!token){
			return false;
		}
		lua_getglobal(L, token);
		if(!lua_istable(L, -1)){
			lua_pop(L, 1);
			lua_createtable(L, 0, 0);
			lua_pushvalue(L, -1);
			lua_setglobal(L, token);
		}
		while((token =strtok_r(0, ".", &saveptr))){
			lua_getfield(L, -1, token);
			if(!lua_istable(L, -1)){
				lua_pop(L, 1);
				lua_createtable(L, 0, 0);
				lua_pushvalue(L, -1);
				lua_setfield(L, -3, token);
			}
			lua_remove(L, -2);
		}
		return true;
	}

	/** get_lua_global_var **/
	inline bool get_lua_global_var(lua_State* L, const char* name){
		// check and prepare
		if(!L || !name) return false;
		const int len =strlen(name);
		if(len <= 0) return false;
		Bytes* bs =SafeNew<Bytes>();
		bs->set(name, len + 1);

		char* sz =bs->c_str();
		char* saveptr =0;

		// process
		char* token =strtok_r(sz, ".", &saveptr);
		if(!token){
			return false;
		}
		lua_getglobal(L, token);
		while((token =strtok_r(0, ".", &saveptr))){
			if(!lua_istable(L, -1)){
				lua_pop(L, 1);
				lua_pushnil(L);
				return false;
			}
			else{
				lua_getfield(L, -1, token);
				lua_remove(L, -2);
			}
		}
		return !lua_isnil(L, -1);
	}

	/** packet **/
	inline bool get_packet_from_lua(lua_State* L, const int64_t idx, PACKET& packet){
		LuaTopHelper lth(L);
		if(lua_istable(L, idx) == 0){
			return false;
		}
		memset(&packet, 0, sizeof(packet));
		// from
		lua_getfield(L, idx, "from");
		if(lua_isnumber(L, -1)){
			packet.from =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		// to
		lua_getfield(L, idx, "to");
		if(lua_isnumber(L, -1)){
			packet.to =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		// who
		lua_getfield(L, idx, "who");
		if(lua_isnumber(L, -1)){
			packet.who =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		// sn
		lua_getfield(L, idx, "sn");
		if(lua_isnumber(L, -1)){
			packet.sn =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		// command
		lua_getfield(L, idx, "command");
		if(lua_isnumber(L, -1)){
			packet.command =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		// option
		lua_getfield(L, idx, "option");
		if(lua_isnumber(L, -1)){
			packet.option =static_cast< uint64_t >(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		return true;
	}
	inline bool push_packet_to_lua(lua_State* L, const PACKET& packet){
		// push packet
		lua_createtable(L, 0, 6);
		lua_pushnumber(L, (double)packet.from);
		lua_setfield(L, -2, "from");

		lua_pushnumber(L, (double)packet.to);
		lua_setfield(L, -2, "to");

		lua_pushnumber(L, (double)packet.who);
		lua_setfield(L, -2, "who");

		lua_pushnumber(L, (double)packet.sn);
		lua_setfield(L, -2, "sn");

		lua_pushnumber(L, (double)packet.command);
		lua_setfield(L, -2, "command");

		lua_pushnumber(L, (double)packet.option);
		lua_setfield(L, -2, "option");

		return true;
	}

	/** register class to lua **/
	typedef struct tagCLASS_FUNC{
		const char* name;
		int (*pfn)(lua_State *L);
	}CLASS_FUNC, *PCLASS_FUNC;

	typedef struct tagCLASS_ENUM{
		const char* name;
		int64_t value;
	}CLASS_ENUM, *PCLASS_ENUM;

	inline bool register_class_to_lua(lua_State* L, const char* module_name, const char* cls_name, const char* metatable_name
			, int (*pfn_gc)(lua_State*)
			, const int64_t static_func_num, const CLASS_FUNC* pstatic_func
			, const int64_t non_static_func_num, const CLASS_FUNC* pnon_static_func
			, const int64_t enum_num, const CLASS_ENUM* penum){
		if(!L || !module_name || !cls_name){
			return false;
		}
		LuaTopHelper tpl(L);
		lua_checkstack(L, 4);

		// ensure module
		lua_getglobal(L, module_name);
		if(lua_istable(L, -1) == 0){
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setglobal(L, module_name);
		}

		// ensure class
		lua_getfield(L, -1, cls_name);
		if(lua_istable(L, -1) == 0){
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, cls_name);
		}

		// static func
		for(int64_t i=0; i<static_func_num; ++i){
			lua_pushcfunction(L, pstatic_func[i].pfn);
			lua_setfield(L, -2, pstatic_func[i].name);
		}
		// enum
		for(int64_t i=0; i<enum_num; ++i){
			lua_pushnumber(L, static_cast< double >(penum[i].value));
			lua_setfield(L, -2, penum[i].name);
		}

		// metatable
		if(metatable_name){
			// new
			lua_getfield(L, -1, "metatable");
			if(lua_istable(L, -1) == 0){
				lua_pop(L, 1);
				lua_newtable(L);
				lua_pushvalue(L, -1);
				lua_setfield(L, -3, "metatable");
			}
			// none static func
			for(int64_t i=0; i<non_static_func_num; ++i){
				lua_pushcfunction(L, pnon_static_func[i].pfn);
				lua_setfield(L, -2, pnon_static_func[i].name);
			}

			// register to global
			lua_pushvalue(L, -1);
			lua_setglobal(L, metatable_name);

			// associate with self
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			// gc
			if(pfn_gc){
				lua_pushcfunction(L, pfn_gc);
			}
			else{
				lua_pushcfunction(L, Object::GC_FOR_LUA);
			}
			lua_setfield(L, -2, "__gc");
		}
		return true;
	}
}

#endif
