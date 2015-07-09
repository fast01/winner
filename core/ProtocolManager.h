/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_PROTOCOL_MANAGER_H__
#define H_PROTOCOL_MANAGER_H__

namespace core{
/*
	define ProtocolManager
*/
class ProtocolManager: public Object{
	typedef Object Super;
	SUPPORT_NEWABLE
	DECLARE_PROCESS_LOCAL_SINGLETON(ProtocolManager)
public:
	typedef ProtocolBase* (*PFN_CREATE_PROTOCOL)(const int64_t id);
protected:
	ProtocolManager();
	virtual ~ProtocolManager();
public:
	virtual void init();
	virtual void finalize();
public:
	bool registerProtocolGroup(const int64_t group_id, PFN_CREATE_PROTOCOL pfn);
	ProtocolBase* createProtocol(const int64_t group_id, const int64_t protocol_id);
public:
	static bool RegisterProtocolGroup(const int64_t group_id, PFN_CREATE_PROTOCOL pfn);
	static ProtocolBase* CreateProtocol(const int64_t group_id, const int64_t protocol_id);
public:
	static bool RegisterToLua(lua_State* L);
private:
	static int _TableToBytes(lua_State* L);
	static int _TableToObject(lua_State* L);
	static int _BytesToTable(lua_State* L);
	static int _ObjectToTable(lua_State* L);
public:
	Hash* m_creator_table;
};
}
#endif
