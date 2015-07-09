/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** singleton ***/
	DEFINE_PROCESS_LOCAL_SINGLETON(ProtocolManager)

	/** impl ProtocolManager  **/
	ProtocolManager::ProtocolManager()
		: m_creator_table(0){
	
	}
	ProtocolManager::~ProtocolManager(){
	
	}

	/** Object **/
	void ProtocolManager::init(){
		Super::init();
		m_creator_table =New< Hash >();
		m_creator_table->retain();
	}
	void ProtocolManager::finalize(){
		CLEAN_POINTER(m_creator_table);
		Super::finalize();
	}

	/** self **/
	bool ProtocolManager::registerProtocolGroup(const int64_t group_id, PFN_CREATE_PROTOCOL pfn){
		if(group_id<=0 || pfn==0){
			return false;
		}
		Pointer* pt =SafeNew<Pointer>();
		pt->setValue(reinterpret_cast< void* >(pfn));
		m_creator_table->set(group_id, pt);
		return true;
	}
	ProtocolBase* ProtocolManager::createProtocol(const int64_t group_id, const int64_t protocol_id){
		if(Pointer* pt =dynamic_cast< Pointer* >(m_creator_table->get(group_id))){
			PFN_CREATE_PROTOCOL pfn =reinterpret_cast< PFN_CREATE_PROTOCOL >(pt->getValue());
			return pfn(protocol_id);
		}
		return 0;
	}
	bool ProtocolManager::RegisterProtocolGroup(const int64_t group_id, PFN_CREATE_PROTOCOL pfn){
		return Instance()->registerProtocolGroup(group_id, pfn);
	}
	ProtocolBase* ProtocolManager::CreateProtocol(const int64_t group_id, const int64_t protocol_id){
		return Instance()->createProtocol(group_id, protocol_id);
	}
	int ProtocolManager::_TableToBytes(lua_State* L){
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg", __func__);
			return 2;
		}
		const int64_t group_id =luaL_checkinteger(L, 1);
		const int64_t proto_id =luaL_checkinteger(L, 2);
		ProtocolBase* proto =Instance()->createProtocol(group_id, proto_id);
		if(!proto){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] create protocol error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		if(!proto->fromLua(L, 3)){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] decode from lua error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		Bytes* bs =proto->toBytes(0);
		if(!bs){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] encode to bytes error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		push_object_to_lua< Bytes >(L, bs);
		return 1;
	}
	int ProtocolManager::_TableToObject(lua_State* L){
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg", __func__);
			return 2;
		}
		const int64_t group_id =luaL_checkinteger(L, 1);
		const int64_t proto_id =luaL_checkinteger(L, 2);
		ProtocolBase* proto =Instance()->createProtocol(group_id, proto_id);
		if(!proto){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] create protocol error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		if(!proto->fromLua(L, 3)){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] decode from lua error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		push_object_to_lua< ProtocolBase >(L, proto);
		return 1;
	}
	int ProtocolManager::_BytesToTable(lua_State* L){
		// check arg
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg", __func__);
			return 2;
		}
		const int64_t group_id =luaL_checkinteger(L, 1);
		const int64_t proto_id =luaL_checkinteger(L, 2);
		Bytes* bs =0;
		if(!get_object_from_lua< Bytes* >(L, 3, bs) || !bs){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg #3", __func__);
			return 2;
		}

		// create proto
		ProtocolBase* proto =Instance()->createProtocol(group_id, proto_id);
		if(!proto){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] create protocol error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		if(!proto->fromBytes(bs)){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] unmarshal error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		if(!proto->toLua(L)){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, [group_id %lld, proto_id %lld] encode to lua error", __func__, (long long)group_id, (long long)proto_id);
			return 2;
		}
		return 1;
	}

	int ProtocolManager::_ObjectToTable(lua_State* L){
		// check arg
		if(lua_gettop(L) < 1){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg", __func__);
			return 2;
		}
		ProtocolBase* proto =0;
		if(!get_object_from_lua< ProtocolBase* >(L, 1, proto) || !proto){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, invalid arg #3", __func__);
			return 2;
		}

		// create proto
		if(!proto->toLua(L)){
			lua_pushnil(L);
			lua_pushfstring(L, "fail to call %s, ProtocolBase to lua table error", __func__);
			return 2;
		}
		return 1;
	}

	bool ProtocolManager::RegisterToLua(lua_State* L){
		CLASS_FUNC func[4] ={
			{ "TableToBytes", &_TableToBytes},
			{ "TableToObject", &_TableToObject},
			{ "BytesToTable", &_BytesToTable},
			{ "ObjectToTable", &_ObjectToTable},
		};
		return register_class_to_lua(L, "Core", "ProtocolManager", 0, 0,
			4, func,
			0, 0,
			0, 0
		);
	}
}
