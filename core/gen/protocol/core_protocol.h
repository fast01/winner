#ifndef H_AUTO_GEN_CODE_CORE_PROTOCOL_H__
#define H_AUTO_GEN_CODE_CORE_PROTOCOL_H__
#include "../../core.h"

namespace core{
namespace protocol{
/*** id ***/
enum{ ID=1 };
/*** register ***/
bool register_protocol();
/*** creator ***/
typedef ::core::ProtocolBase* (*PFN_PROTOCOL_CREATOR)();
::core::ProtocolBase* create_protocol(const int64_t id);
::core::ProtocolBase* create_protocol_from_file(const int64_t id, const char* path);
::core::ProtocolBase* create_protocol_from_bytes(const int64_t id, ::core::Bytes* bytes);
::core::ProtocolBase* create_protocol_from_lua_file(const int64_t id, const char* path);

/*** predeclare ***/
class ActorConfig;
class ActorManagerConfig;
class DataServiceConfig;
class LogServiceConfig;
class LogTargetConfig;
class MysqlConfig;
class NetworkConfig;
class NetworkConnectionConfig;
class NetworkListenerConfig;
class NetworkRouteConfig;
class RedisConfig;
class ServiceConfig;
class ServiceManagerConfig;

/*** enum ***/
enum{
	PROTOCOL____,
	PROTOCOL_ACTOR_CONFIG=1,
	PROTOCOL_ACTOR_MANAGER_CONFIG=2,
	PROTOCOL_DATA_SERVICE_CONFIG=3,
	PROTOCOL_LOG_SERVICE_CONFIG=4,
	PROTOCOL_LOG_TARGET_CONFIG=5,
	PROTOCOL_MYSQL_CONFIG=6,
	PROTOCOL_NETWORK_CONFIG=7,
	PROTOCOL_NETWORK_CONNECTION_CONFIG=8,
	PROTOCOL_NETWORK_LISTENER_CONFIG=9,
	PROTOCOL_NETWORK_ROUTE_CONFIG=10,
	PROTOCOL_REDIS_CONFIG=11,
	PROTOCOL_SERVICE_CONFIG=12,
	PROTOCOL_SERVICE_MANAGER_CONFIG=13,
	PROTOCOL_MAX_ID =13
};

/*** class define ***/
/** define  ActorConfig **/
class ActorConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	ActorConfig();
	virtual ~ActorConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field TemplateName */
	DECLARE_PROPERTY(::core::String*, TemplateName);
	void _init_field_TemplateName();
	/* field ClassName */
	DECLARE_PROPERTY(::core::String*, ClassName);
	void _init_field_ClassName();
	/* field Component */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, ::core::String*, Component);
	void _init_field_Component();
	/* field Observer */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, ::core::String*, Observer);
	void _init_field_Observer();
};
/** define  ActorManagerConfig **/
class ActorManagerConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	ActorManagerConfig();
	virtual ~ActorManagerConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Actor */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, ActorConfig*, Actor);
	void _init_field_Actor();
};
/** define  DataServiceConfig **/
class DataServiceConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	DataServiceConfig();
	virtual ~DataServiceConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Mysql */
	DECLARE_PROPERTY(MysqlConfig*, Mysql);
	void _init_field_Mysql();
	/* field Redis */
	DECLARE_PROPERTY(RedisConfig*, Redis);
	void _init_field_Redis();
};
/** define  LogServiceConfig **/
class LogServiceConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	LogServiceConfig();
	virtual ~LogServiceConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Target */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, LogTargetConfig*, Target);
	void _init_field_Target();
};
/** define  LogTargetConfig **/
class LogTargetConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	LogTargetConfig();
	virtual ~LogTargetConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Type */
	DECLARE_PROPERTY(int64_t, Type);
	void _init_field_Type();
	/* field FileName */
	DECLARE_PROPERTY(::core::String*, FileName);
	void _init_field_FileName();
	/* field FileLifeTime */
	DECLARE_PROPERTY(int64_t, FileLifeTime);
	void _init_field_FileLifeTime();
	/* field FlushMode */
	DECLARE_PROPERTY(int64_t, FlushMode);
	void _init_field_FlushMode();
};
/** define  MysqlConfig **/
class MysqlConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	MysqlConfig();
	virtual ~MysqlConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Host */
	DECLARE_PROPERTY(::core::String*, Host);
	void _init_field_Host();
	/* field Port */
	DECLARE_PROPERTY(int64_t, Port);
	void _init_field_Port();
	/* field User */
	DECLARE_PROPERTY(::core::String*, User);
	void _init_field_User();
	/* field Passwd */
	DECLARE_PROPERTY(::core::String*, Passwd);
	void _init_field_Passwd();
};
/** define  NetworkConfig **/
class NetworkConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	NetworkConfig();
	virtual ~NetworkConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field SlaveCount */
	DECLARE_PROPERTY(int64_t, SlaveCount);
	void _init_field_SlaveCount();
	/* field Listener */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, NetworkListenerConfig*, Listener);
	void _init_field_Listener();
	/* field Remote */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, NetworkConnectionConfig*, Remote);
	void _init_field_Remote();
	/* field Route */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, NetworkRouteConfig*, Route);
	void _init_field_Route();
};
/** define  NetworkConnectionConfig **/
class NetworkConnectionConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	NetworkConnectionConfig();
	virtual ~NetworkConnectionConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Type */
	DECLARE_PROPERTY(::core::String*, Type);
	void _init_field_Type();
	/* field Id */
	DECLARE_PROPERTY(int64_t, Id);
	void _init_field_Id();
	/* field Host */
	DECLARE_PROPERTY(::core::String*, Host);
	void _init_field_Host();
	/* field Port */
	DECLARE_PROPERTY(int32_t, Port);
	void _init_field_Port();
	/* field RunnerBegin */
	DECLARE_PROPERTY(int64_t, RunnerBegin);
	void _init_field_RunnerBegin();
	/* field RunnerRange */
	DECLARE_PROPERTY(int64_t, RunnerRange);
	void _init_field_RunnerRange();
};
/** define  NetworkListenerConfig **/
class NetworkListenerConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	NetworkListenerConfig();
	virtual ~NetworkListenerConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Type */
	DECLARE_PROPERTY(::core::String*, Type);
	void _init_field_Type();
	/* field Id */
	DECLARE_PROPERTY(int64_t, Id);
	void _init_field_Id();
	/* field Path */
	DECLARE_PROPERTY(::core::String*, Path);
	void _init_field_Path();
	/* field Port */
	DECLARE_PROPERTY(int32_t, Port);
	void _init_field_Port();
	/* field DeliverBegin */
	DECLARE_PROPERTY(int64_t, DeliverBegin);
	void _init_field_DeliverBegin();
	/* field DeliverRange */
	DECLARE_PROPERTY(int64_t, DeliverRange);
	void _init_field_DeliverRange();
	/* field RunnerBegin */
	DECLARE_PROPERTY(int64_t, RunnerBegin);
	void _init_field_RunnerBegin();
	/* field RunnerRange */
	DECLARE_PROPERTY(int64_t, RunnerRange);
	void _init_field_RunnerRange();
};
/** define  NetworkRouteConfig **/
class NetworkRouteConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	NetworkRouteConfig();
	virtual ~NetworkRouteConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field IdService */
	DECLARE_PROPERTY(int64_t, IdService);
	void _init_field_IdService();
	/* field IdBegin */
	DECLARE_PROPERTY(int64_t, IdBegin);
	void _init_field_IdBegin();
	/* field IdEnd */
	DECLARE_PROPERTY(int64_t, IdEnd);
	void _init_field_IdEnd();
	/* field Algrithm */
	DECLARE_PROPERTY(int64_t, Algrithm);
	void _init_field_Algrithm();
public:
	enum{
		ALG_DEFAULT =0,
		ALG_USE_FROM_WHO =0,
		ALG_USE_WHO =1,
		ALG_USE_FROM =2,
		ALG_RANDOM =3,
		ALG_ROUND_ROBIN =4,
	};
};
/** define  RedisConfig **/
class RedisConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	RedisConfig();
	virtual ~RedisConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Host */
	DECLARE_PROPERTY(::core::String*, Host);
	void _init_field_Host();
	/* field Port */
	DECLARE_PROPERTY(int64_t, Port);
	void _init_field_Port();
};
/** define  ServiceConfig **/
class ServiceConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	ServiceConfig();
	virtual ~ServiceConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field IdService */
	DECLARE_PROPERTY(int64_t, IdService);
	void _init_field_IdService();
	/* field IdBegin */
	DECLARE_PROPERTY(int64_t, IdBegin);
	void _init_field_IdBegin();
	/* field IdEnd */
	DECLARE_PROPERTY(int64_t, IdEnd);
	void _init_field_IdEnd();
	/* field Algrithm */
	DECLARE_PROPERTY(int64_t, Algrithm);
	void _init_field_Algrithm();
public:
	enum{
		ALG_DEFAULT =0,
		ALG_USE_WHO =1,
		ALG_USE_FROM =2,
		ALG_USE_FROM_WHO =3,
		ALG_RANDOM =4,
	};
	/* field Path */
	DECLARE_PROPERTY(::core::String*, Path);
	void _init_field_Path();
};
/** define  ServiceManagerConfig **/
class ServiceManagerConfig: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	ServiceManagerConfig();
	virtual ~ServiceManagerConfig();
public:
	virtual void init();
	virtual void finalize();
public:
	static ::core::ProtocolBase* Creator();
public:
	virtual int64_t group();
	virtual int64_t id();
	virtual const char* name();
	virtual bool check();
	virtual void clean();
	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);
	virtual bool fromBytes(::core::Bytes* bytes);
	virtual bool toLua(lua_State* L);
	virtual bool fromLua(lua_State* L, const int64_t idx);
	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);
public:
	/* field Service */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, ServiceConfig*, Service);
	void _init_field_Service();
};
}
}
#endif
