#ifndef H_AUTO_GEN_CODE_PROTOCOL_H__
#define H_AUTO_GEN_CODE_PROTOCOL_H__
#include "../../core/core.h"

namespace protocol{
/*** id ***/
enum{ ID=17 };
/*** register ***/
bool register_protocol();
/*** creator ***/
typedef ::core::ProtocolBase* (*PFN_PROTOCOL_CREATOR)();
::core::ProtocolBase* create_protocol(const int64_t id);
::core::ProtocolBase* create_protocol_from_file(const int64_t id, const char* path);
::core::ProtocolBase* create_protocol_from_bytes(const int64_t id, ::core::Bytes* bytes);
::core::ProtocolBase* create_protocol_from_lua_file(const int64_t id, const char* path);

/*** predeclare ***/
class S2FirstRequest;
class S2FirstRespond;
class S2SecondRequest;
class S2SecondRespond;
class S3FirstRequest;
class S3FirstRespond;
class S3SecondRequest;
class S3SecondRespond;
class S5FirstRequest;
class S5FirstRespond;
class S5SecondRequest;
class S5SecondRespond;
class S4FirstRequest;
class S4FirstRespond;
class S4SecondRequest;
class S4SecondRespond;
class Family;
class Game;
class Human;

/*** enum ***/
enum{
	PROTOCOL____,
	PROTOCOL_S2_FIRST_REQUEST=1001,
	PROTOCOL_S2_FIRST_RESPOND=1002,
	PROTOCOL_S2_SECOND_REQUEST=1003,
	PROTOCOL_S2_SECOND_RESPOND=1004,
	PROTOCOL_S3_FIRST_REQUEST=1005,
	PROTOCOL_S3_FIRST_RESPOND=1006,
	PROTOCOL_S3_SECOND_REQUEST=1007,
	PROTOCOL_S3_SECOND_RESPOND=1008,
	PROTOCOL_S5_FIRST_REQUEST=1009,
	PROTOCOL_S5_FIRST_RESPOND=1010,
	PROTOCOL_S5_SECOND_REQUEST=1011,
	PROTOCOL_S5_SECOND_RESPOND=1012,
	PROTOCOL_S4_FIRST_REQUEST=1013,
	PROTOCOL_S4_FIRST_RESPOND=1014,
	PROTOCOL_S4_SECOND_REQUEST=1015,
	PROTOCOL_S4_SECOND_RESPOND=1016,
	PROTOCOL_FAMILY=1017,
	PROTOCOL_GAME=1018,
	PROTOCOL_HUMAN=1019,
	PROTOCOL_MAX_ID =1019
};

/*** class define ***/
/** define  S2FirstRequest **/
class S2FirstRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S2FirstRequest();
	virtual ~S2FirstRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S2FirstRespond **/
class S2FirstRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S2FirstRespond();
	virtual ~S2FirstRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S2SecondRequest **/
class S2SecondRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S2SecondRequest();
	virtual ~S2SecondRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S2SecondRespond **/
class S2SecondRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S2SecondRespond();
	virtual ~S2SecondRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S3FirstRequest **/
class S3FirstRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S3FirstRequest();
	virtual ~S3FirstRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S3FirstRespond **/
class S3FirstRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S3FirstRespond();
	virtual ~S3FirstRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S3SecondRequest **/
class S3SecondRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S3SecondRequest();
	virtual ~S3SecondRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S3SecondRespond **/
class S3SecondRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S3SecondRespond();
	virtual ~S3SecondRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S5FirstRequest **/
class S5FirstRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S5FirstRequest();
	virtual ~S5FirstRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S5FirstRespond **/
class S5FirstRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S5FirstRespond();
	virtual ~S5FirstRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S5SecondRequest **/
class S5SecondRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S5SecondRequest();
	virtual ~S5SecondRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S5SecondRespond **/
class S5SecondRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S5SecondRespond();
	virtual ~S5SecondRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S4FirstRequest **/
class S4FirstRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S4FirstRequest();
	virtual ~S4FirstRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S4FirstRespond **/
class S4FirstRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S4FirstRespond();
	virtual ~S4FirstRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  S4SecondRequest **/
class S4SecondRequest: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S4SecondRequest();
	virtual ~S4SecondRequest();
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
	/* field Param1 */
	DECLARE_PROPERTY(int64_t, Param1);
	void _init_field_Param1();
	/* field Param2 */
	DECLARE_PROPERTY(boolean_t, Param2);
	void _init_field_Param2();
	/* field Param3 */
	DECLARE_PROPERTY(::core::String*, Param3);
	void _init_field_Param3();
};
/** define  S4SecondRespond **/
class S4SecondRespond: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	S4SecondRespond();
	virtual ~S4SecondRespond();
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
	/* field Error */
	DECLARE_PROPERTY(int64_t, Error);
	void _init_field_Error();
	/* field Result1 */
	DECLARE_PROPERTY(int64_t, Result1);
	void _init_field_Result1();
	/* field Result2 */
	DECLARE_PROPERTY(boolean_t, Result2);
	void _init_field_Result2();
	/* field Result3 */
	DECLARE_PROPERTY(::core::String*, Result3);
	void _init_field_Result3();
};
/** define  Family **/
class Family: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	Family();
	virtual ~Family();
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
	/* field Father */
	DECLARE_PROPERTY(Human*, Father);
	void _init_field_Father();
	/* field Mother */
	DECLARE_PROPERTY(Human*, Mother);
	void _init_field_Mother();
	/* field Children */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, Human*, Children);
	void _init_field_Children();
};
/** define  Game **/
class Game: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	Game();
	virtual ~Game();
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
	/* field Id */
	DECLARE_PROPERTY(int64_t, Id);
	void _init_field_Id();
	/* field Name */
	DECLARE_PROPERTY(::core::String*, Name);
	void _init_field_Name();
	/* field Desc */
	DECLARE_PROPERTY(::core::String*, Desc);
	void _init_field_Desc();
};
/** define  Human **/
class Human: public ::core::ProtocolBase{
	typedef ::core::ProtocolBase Super;
	SUPPORT_NEWABLE
public:
	Human();
	virtual ~Human();
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
	/* field Id */
	DECLARE_PROPERTY(int64_t, Id);
	void _init_field_Id();
	/* field Name */
	DECLARE_PROPERTY(::core::String*, Name);
	void _init_field_Name();
	/* field Desc */
	DECLARE_PROPERTY(::core::String*, Desc);
	void _init_field_Desc();
	/* field Book */
	DECLARE_PROPERTY_DYNAMIC_ARRAY(::core::Array*, ::core::String*, Book);
	void _init_field_Book();
	/* field Favorite */
	DECLARE_PROPERTY_FIXED_ARRAY(Game*, Favorite, 3);
	void _init_field_Favorite();
};
}
#endif
