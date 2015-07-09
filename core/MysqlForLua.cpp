/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** MysqlForLua ***/
	DEFINE_SUPPORT_SCRIPT(MysqlForLua, "core::MysqlForLua")

	/** ctor & dtor **/
	MysqlForLua::MysqlForLua()
		: m_mysql(0){
	}
	MysqlForLua::~MysqlForLua(){
	}

	/** Object **/
	void MysqlForLua::init(){
		Super::init();
	}
	void MysqlForLua::finalize(){
		if(m_mysql){
			mysql_close(m_mysql);
			m_mysql =0;
		}
		Super::finalize();
	}
	/** support lua **/
	int MysqlForLua::_New(lua_State* L){
		MysqlForLua* obj =SafeNew<MysqlForLua>();
		if(push_object_to_lua< MysqlForLua >(L, obj)){
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "new MysqlForLua Object error");
			return 2;
		}
	}
	int MysqlForLua::_Connect(lua_State* L){
		//// prepare args
		/// check arg
		if(lua_gettop(L) < 4){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		// user
		const char* user =0;
		if(lua_isstring(L, 2)){
			user =lua_tostring(L, 2);
		}
		// passwd
		const char* passwd =0;
		if(lua_isstring(L, 3)){
			passwd =lua_tostring(L, 3);
		}
		// host/path
		const char* host_or_path =0;
		if(lua_isstring(L, 4)){
			host_or_path =lua_tostring(L, 4);
		}
		int port =0;
		if((lua_gettop(L)>=5) && (lua_isnumber(L, 5))){
			port =lua_tonumber(L, 5);
		}
		//// disconnect
		if(self->m_mysql){
			mysql_close(self->m_mysql);
		}
		//// init new
		self->m_mysql =mysql_init(0);
		if(!self->m_mysql){
			lua_pushnil(L);
			lua_pushstring(L, "mysql_init error");
			return 2;
		}
		//// connect
		if(port > 0){
			if(!mysql_real_connect(self->m_mysql, host_or_path, user, passwd, 0, port, 0, 0)){
				lua_pushnil(L);
				lua_pushfstring(L, "mysql_connect %s:%d error, %s", host_or_path, (int)port, self->_mysql_error());
				mysql_close(self->m_mysql);
				self->m_mysql =0;
				return 2;
			}
		}
		else{
			if(!mysql_real_connect(self->m_mysql, 0, user, passwd, 0, 0, host_or_path, 0)){
				lua_pushnil(L);
				lua_pushfstring(L, "mysql_connect %s error, %s", host_or_path, self->_mysql_error());
				mysql_close(self->m_mysql);
				self->m_mysql =0;
				return 2;
			}
		}
		//// set utf8
		if(mysql_query(self->m_mysql, "set charset utf8;")){
			mysql_close(self->m_mysql);
			self->m_mysql =0;
			lua_pushnil(L);
			lua_pushstring(L, "fail to set charset utf8");
			return 2;
		}
		lua_pushboolean(L, 1);
		return 1;
	}
	int MysqlForLua::_Disconnect(lua_State* L){
		//// prepare args
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		//// disconnect
		if(self->m_mysql){
			mysql_close(self->m_mysql);
			self->m_mysql =0;
		}
		lua_pushboolean(L, 1);
		return 1;
	}
	int MysqlForLua::_Good(lua_State* L){
		//// prepare args
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		//// check good
		lua_pushboolean(L, self->m_mysql ? 1 : 0);
		return 1;
	}
	int MysqlForLua::_Query(lua_State* L){
		//// prepare args
		if(lua_gettop(L) < 2){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		// what
		if(0 == lua_isstring(L, 2)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg what");
			return 2;
		}
		const char* what =lua_tostring(L, 2);
		ASSERT(what);
		//// check
		if(!self->m_mysql){
			lua_pushnil(L);
			lua_pushstring(L, "MysqlForLua not connected");
			return 2;
		}
		//// query
		DEBUG(what);
		if(mysql_query(self->m_mysql, what)){
			lua_pushnil(L);
			lua_pushfstring(L, "%s", self->_mysql_error());
			return 2;
		}
		//// process result
		MYSQL_RES *result =mysql_store_result(self->m_mysql);
		if(!result){
			lua_pushnil(L);
			lua_pushfstring(L, "%s", self->_mysql_error());
			return 2;
		}
		const int64_t num_fields =mysql_num_fields(result);
		MYSQL_FIELD* fields =mysql_fetch_fields(result);
		if(!fields || num_fields<=0){
			mysql_free_result(result);
			lua_pushnil(L);
			lua_pushfstring(L, "%s", self->_mysql_error());
			return 2;
		}
		const int64_t num_rows =mysql_num_rows(result);
		lua_createtable(L, (int)num_rows, 0);
		int index =1;
		while(MYSQL_ROW row =mysql_fetch_row(result)){
			lua_createtable(L, 0, num_fields);
			for(int64_t i=0; i<num_fields; ++i){
				const int type =fields[i].type;
				switch(type){
				case FIELD_TYPE_TINY:
				case FIELD_TYPE_SHORT:
				case FIELD_TYPE_LONG:
				case FIELD_TYPE_INT24:
				case FIELD_TYPE_LONGLONG:
				case FIELD_TYPE_FLOAT:
				case FIELD_TYPE_DOUBLE:
					if(row[i]){
						float64_t v =0;
						if(string_to_float64(row[i], v)){
							lua_pushnumber(L, v);
							lua_setfield(L, -2, fields[i].name);
						}
						else{
							mysql_free_result(result);
							lua_pop(L, 2);
							lua_pushnil(L);
							lua_pushfstring(L, "the value of mysql field %s is not a number, value is %s", fields[i].name, row[i]);
							return 2;
						}
					}
					break;
				case FIELD_TYPE_STRING:
				case FIELD_TYPE_VAR_STRING:
					if(row[i]){
						lua_pushstring(L, row[i]);
						lua_setfield(L, -2, fields[i].name);
					}
					break;
				default:
					mysql_free_result(result);
					lua_pop(L, 2);
					lua_pushnil(L);
					lua_pushfstring(L, "the type of mysql field %s is not support, type is %d", fields[i].name, type);
					return 2;
				}
			}
			lua_rawseti(L, -2, index);
			index +=1;
		}
		mysql_free_result(result);
		return 1;
	}
	int MysqlForLua::_Exec(lua_State* L){
		//// prepare args
		if(lua_gettop(L) < 2){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		// what
		if(0 == lua_isstring(L, 2)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg what");
			return 2;
		}
		const char* what =lua_tostring(L, 2);
		ASSERT(what);
		//// check
		if(!self->m_mysql){
			lua_pushnil(L);
			lua_pushstring(L, "mysql not connected");
			return 2;
		}
		//// update
		DEBUG(what);
		if(mysql_query(self->m_mysql, what)){
			lua_pushnil(L);
			lua_pushfstring(L, "%s", self->_mysql_error());
			return 2;
		}
		lua_pushboolean(L, 1);
		return 1;
	}
	int MysqlForLua::_LastInsertId(lua_State* L){
		// prepare args
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		//// check
		if(!self->m_mysql){
			lua_pushnil(L);
			lua_pushstring(L, "mysql not connected");
			return 2;
		}
		// get insert id
		int64_t id =mysql_insert_id(self->m_mysql);
		lua_pushnumber(L, (float64_t)id);
		return 1;
	}
	int MysqlForLua::_AffectedRows(lua_State* L){
		// prepare args
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushstring(L, "missing arg");
			return 2;
		}
		// self
		MysqlForLua* self =0;
		if(!get_object_from_lua< MysqlForLua >(L, 1, self) || !self){
			lua_pushnil(L);
			lua_pushstring(L, "not a valid MysqlForLua Object");
			return 2;
		}
		//// check
		if(!self->m_mysql){
			lua_pushnil(L);
			lua_pushstring(L, "mysql not connected");
			return 2;
		}
		// get affected rows
		int64_t n =mysql_affected_rows(self->m_mysql);
		lua_pushnumber(L, (float64_t)n);
		return 1;
	}
	bool MysqlForLua::RegisterToLua(lua_State* L){
		CLASS_FUNC func[8] ={
			{ "New", &_New},
			{ "connect", &_Connect},
			{ "disconnect", &_Disconnect},
			{ "good", &_Good},
			{ "query", &_Query},
			{ "exec", &_Exec},
			{ "lastInsertId", &_LastInsertId},
			{ "affectedRows", &_AffectedRows},
		};
		return register_class_to_lua(L, "Core", "Mysql", MAKE_LUA_METATABLE_NAME(core::MysqlForLua), 0,
			1, func,
			7, func+1,
			0, 0
		);
	}
	const char* MysqlForLua::_mysql_error(){
		if(m_mysql){
			if(const char* err =mysql_error(m_mysql)){
				return err;
			}
			else{
				return "what the fuck";
			}
		}
		else{
			return "mysql not connected";
		}
	}
}
