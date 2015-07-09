/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_MYSQL_FOR_LUA_H__
#define H_CORE_MYSQL_FOR_LUA_H__

namespace core{
	/** MysqlForLua **/
	class MysqlForLua : public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_SUPPORT_SCRIPT
	private:
		MysqlForLua();
		virtual ~MysqlForLua();
	public:
		virtual void init();
		virtual void finalize();
	private:
		static int _New(lua_State* L);
		static int _Connect(lua_State* L);
		static int _Disconnect(lua_State* L);
		static int _Good(lua_State* L);
		static int _Query(lua_State* L);
		static int _Exec(lua_State* L);
		static int _LastInsertId(lua_State* L);
		static int _AffectedRows(lua_State* L);
	public:
		static bool RegisterToLua(lua_State* L);
	private:
		const char* _mysql_error();
	private:
		MYSQL* m_mysql;
	};
}

#endif
