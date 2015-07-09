#include "core_protocol.h"

namespace core{
namespace protocol{
/*** register ***/
static PFN_PROTOCOL_CREATOR g_protocol_creator[PROTOCOL_MAX_ID];
bool register_protocol(){
	// Need't set as zero, because g_protocol_creator will be set zero when program started, but i have compulsion!!!
	memset(g_protocol_creator, 0, sizeof(g_protocol_creator));
	g_protocol_creator[1 - 1] =ActorConfig::Creator; 
	g_protocol_creator[2 - 1] =ActorManagerConfig::Creator; 
	g_protocol_creator[3 - 1] =DataServiceConfig::Creator; 
	g_protocol_creator[4 - 1] =LogServiceConfig::Creator; 
	g_protocol_creator[5 - 1] =LogTargetConfig::Creator; 
	g_protocol_creator[6 - 1] =MysqlConfig::Creator; 
	g_protocol_creator[7 - 1] =NetworkConfig::Creator; 
	g_protocol_creator[8 - 1] =NetworkConnectionConfig::Creator; 
	g_protocol_creator[9 - 1] =NetworkListenerConfig::Creator; 
	g_protocol_creator[10 - 1] =NetworkRouteConfig::Creator; 
	g_protocol_creator[11 - 1] =RedisConfig::Creator; 
	g_protocol_creator[12 - 1] =ServiceConfig::Creator; 
	g_protocol_creator[13 - 1] =ServiceManagerConfig::Creator; 
	return ::core::ProtocolManager::Instance()->registerProtocolGroup(ID, &create_protocol);
}
/*** create protocol ***/
::core::ProtocolBase* create_protocol(const int64_t id){
	if(id>=1 && id<=PROTOCOL_MAX_ID){ return g_protocol_creator[id-1](); }
	return 0;
}
::core::ProtocolBase* create_protocol_from_bin_file(const int64_t id, const char* path){
	if(::core::Bytes* bs =::core::read_bytes(path)){ return create_protocol_from_bytes(id, bs); }
	else { return 0; }
}
::core::ProtocolBase* create_protocol_from_bytes(const int64_t id, ::core::Bytes* bytes){
	if(!bytes){ return 0; }
	if(::core::ProtocolBase* proto =create_protocol(id)){
		if(proto->fromBytes(bytes)){
			return proto;
		}
		else{
			ERROR("fail to call %s, unmarsh bytes error", __FUNCTION__);
			return 0;
		}
	}
	else{
		ERROR("fail to call %s, protocol %lld not exist", __FUNCTION__, (long long)id);
		return 0;
	}
}
::core::ProtocolBase* create_protocol_from_lua_file(const int64_t id, const char* path){
	lua_State* L =luaL_newstate();
	if(!L){
		ERROR("fail to call %s, lua_newstate return 0", __FUNCTION__);
		return 0;
	}
	luaL_openlibs(L);
	if(0 != luaL_dofile(L, path)){
		ERROR("fail to call %s, %s", __FUNCTION__, lua_tostring(L, -1));
		lua_close(L);
		return 0;
	}
	lua_getglobal(L, "Config");
	if(!lua_istable(L, -1)){
		ERROR("fail to call %s, Config not a table", __FUNCTION__);
		lua_close(L);
		return 0;
	}
	if(::core::ProtocolBase* proto =create_protocol(id)){
		if(proto->fromLua(L, -1)){
			lua_close(L);
			return proto;
		}
		else{
			ERROR("fail to call %s, unmarsh lua error", __FUNCTION__);
			lua_close(L);
			return 0;
		}
	}
	else{
		ERROR("fail to call %s, protocol %lld not exist", __FUNCTION__, (long long)id);
		lua_close(L);
		return 0;
	}
}
/*** tab func ***/
static const char* tab(int64_t cnt){
	return ::core::String::Repeat('	', cnt)->c_str();
}
/*** class impl ***/
/** class ActorConfig **/
/** impl ActorConfig **/
ActorConfig::ActorConfig(){
}
ActorConfig::~ActorConfig(){
}
void ActorConfig::init(){
	Super::init();
	_init_field_TemplateName();
	_init_field_ClassName();
	_init_field_Component();
	_init_field_Observer();
}
void ActorConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* ActorConfig::Creator(){
	return ::core::SafeNew<ActorConfig>();
}
void ActorConfig::clean(){
	// clean TemplateName 
	CLEAN_POINTER(m_TemplateName);
	// clean ClassName 
	CLEAN_POINTER(m_ClassName);
	// clean Component 
	CLEAN_POINTER(m_Component);
	// clean Observer 
	CLEAN_POINTER(m_Observer);
}
int64_t ActorConfig::group(){
	return 1;
}
int64_t ActorConfig::id(){
	return 1;
}
const char* ActorConfig::name(){
	return "ActorConfig";
}
bool ActorConfig::check(){
	//// check TemplateName 
	// check value
	//// check ClassName 
	// check value
	//// check Component 
	// check value
	//// check Observer 
	// check value
	return true;
}
::core::Bytes* ActorConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// TemplateName
	::core::ToBytes< ::core::String* >(bytes, m_TemplateName);
	/// ClassName
	::core::ToBytes< ::core::String* >(bytes, m_ClassName);
	/// Component
	const int64_t Component_cnt =(m_Component ? (m_Component->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Component_cnt);
	for(int64_t i=0; i<Component_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Component->get(i));
		::core::ToBytes< ::core::String* >(bytes, item);
	}
	/// Observer
	const int64_t Observer_cnt =(m_Observer ? (m_Observer->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Observer_cnt);
	for(int64_t i=0; i<Observer_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Observer->get(i));
		::core::ToBytes< ::core::String* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool ActorConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// TemplateName
	if(just_init){
		_init_field_TemplateName();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_TemplateName)){ clean(); return false; }
		RETAIN_POINTER(m_TemplateName);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ActorConfig from bytes");
			return false;
		}
	}
	/// ClassName
	if(just_init){
		_init_field_ClassName();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_ClassName)){ clean(); return false; }
		RETAIN_POINTER(m_ClassName);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ActorConfig from bytes");
			return false;
		}
	}
	/// Component
	if(just_init){
		_init_field_Component();
	}
	else{
		if(m_Component == 0){
			m_Component =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Component);
		}
		int64_t Component_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Component_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Component_cnt; ++i){
			::core::String* item =0;
			if(!::core::FromBytes< ::core::String* >(bytes, item)){ clean(); return false; }
			m_Component->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ActorConfig from bytes");
			return false;
		}
	}
	/// Observer
	if(just_init){
		_init_field_Observer();
	}
	else{
		if(m_Observer == 0){
			m_Observer =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Observer);
		}
		int64_t Observer_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Observer_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Observer_cnt; ++i){
			::core::String* item =0;
			if(!::core::FromBytes< ::core::String* >(bytes, item)){ clean(); return false; }
			m_Observer->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ActorConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool ActorConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// TemplateName
	::core::ToLua< ::core::String* >(L, m_TemplateName);
	lua_setfield(L, -2, "TemplateName");
	// ClassName
	::core::ToLua< ::core::String* >(L, m_ClassName);
	lua_setfield(L, -2, "ClassName");
	// Component
	const int64_t Component_cnt =(m_Component ? (m_Component->size()) : 0);
	lua_createtable(L, (int)Component_cnt, 0);
	for(int64_t i=0; i<Component_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Component->get(i));
		::core::ToLua< ::core::String* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Component");
	// Observer
	const int64_t Observer_cnt =(m_Observer ? (m_Observer->size()) : 0);
	lua_createtable(L, (int)Observer_cnt, 0);
	for(int64_t i=0; i<Observer_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Observer->get(i));
		::core::ToLua< ::core::String* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Observer");
	return true;
}
bool ActorConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// TemplateName
	lua_getfield(L, idx, "TemplateName");
	if(lua_isnil(L, -1)){
		_init_field_TemplateName();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_TemplateName)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_TemplateName);
	}
	lua_settop(L, old_top);
	// ClassName
	lua_getfield(L, idx, "ClassName");
	if(lua_isnil(L, -1)){
		_init_field_ClassName();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_ClassName)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_ClassName);
	}
	lua_settop(L, old_top);
	// Component
	lua_getfield(L, idx, "Component");
	if(lua_isnil(L, -1)){
		_init_field_Component();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Component_cnt =lua_objlen(L, -1);
			if(m_Component == 0){
				m_Component =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Component);
			}
			for(int64_t i=0; i<Component_cnt; ++i){
				::core::String* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< ::core::String* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Component->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	// Observer
	lua_getfield(L, idx, "Observer");
	if(lua_isnil(L, -1)){
		_init_field_Observer();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Observer_cnt =lua_objlen(L, -1);
			if(m_Observer == 0){
				m_Observer =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Observer);
			}
			for(int64_t i=0; i<Observer_cnt; ++i){
				::core::String* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< ::core::String* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Observer->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool ActorConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// TemplateName
	str->appendString(::core::String::Format("%sTemplateName =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_TemplateName)->c_str()));
	// ClassName
	str->appendString(::core::String::Format("%sClassName =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_ClassName)->c_str()));
	// Component
	str->appendString(::core::String::Format("%sComponent ={\n", tab(tab_count+1)));
	const int64_t Component_cnt =(m_Component ? (m_Component->size()) : 0);
	for(int64_t i=0; i<Component_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Component->get(i));
		str->appendString(::core::String::Format("%s%s,\n", tab(tab_count+2), ::core::ToString< ::core::String* >(item)->c_str()));
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	// Observer
	str->appendString(::core::String::Format("%sObserver ={\n", tab(tab_count+1)));
	const int64_t Observer_cnt =(m_Observer ? (m_Observer->size()) : 0);
	for(int64_t i=0; i<Observer_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Observer->get(i));
		str->appendString(::core::String::Format("%s%s,\n", tab(tab_count+2), ::core::ToString< ::core::String* >(item)->c_str()));
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(ActorConfig, ::core::String*, TemplateName)
void ActorConfig::_init_field_TemplateName(){
	m_TemplateName =::core::String::NewString("");
	RETAIN_POINTER(m_TemplateName);
}
DEFINE_PROPERTY_P(ActorConfig, ::core::String*, ClassName)
void ActorConfig::_init_field_ClassName(){
	m_ClassName =::core::String::NewString("");
	RETAIN_POINTER(m_ClassName);
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(ActorConfig, ::core::Array*, ::core::String*, Component)
void ActorConfig::_init_field_Component(){
	m_Component =0;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(ActorConfig, ::core::Array*, ::core::String*, Observer)
void ActorConfig::_init_field_Observer(){
	m_Observer =0;
}
/** class ActorManagerConfig **/
/** impl ActorManagerConfig **/
ActorManagerConfig::ActorManagerConfig(){
}
ActorManagerConfig::~ActorManagerConfig(){
}
void ActorManagerConfig::init(){
	Super::init();
	_init_field_Actor();
}
void ActorManagerConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* ActorManagerConfig::Creator(){
	return ::core::SafeNew<ActorManagerConfig>();
}
void ActorManagerConfig::clean(){
	// clean Actor 
	CLEAN_POINTER(m_Actor);
}
int64_t ActorManagerConfig::group(){
	return 1;
}
int64_t ActorManagerConfig::id(){
	return 2;
}
const char* ActorManagerConfig::name(){
	return "ActorManagerConfig";
}
bool ActorManagerConfig::check(){
	//// check Actor 
	// check value
	const int64_t n_so_Actor =(m_Actor ? m_Actor->size() : 0);
	for(int64_t i=0; i<n_so_Actor; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Actor->get(i))){
			if(o->check() == false){
				ERROR("protocol ActorManagerConfig check field, check the %lldth Actor error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* ActorManagerConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Actor
	const int64_t Actor_cnt =(m_Actor ? (m_Actor->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Actor_cnt);
	for(int64_t i=0; i<Actor_cnt; ++i){
		ActorConfig* item =dynamic_cast< ActorConfig* >(m_Actor->get(i));
		::core::ToBytes< ActorConfig* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool ActorManagerConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Actor
	if(just_init){
		_init_field_Actor();
	}
	else{
		if(m_Actor == 0){
			m_Actor =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Actor);
		}
		int64_t Actor_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Actor_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Actor_cnt; ++i){
			ActorConfig* item =0;
			if(!::core::FromBytes< ActorConfig* >(bytes, item)){ clean(); return false; }
			m_Actor->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ActorManagerConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool ActorManagerConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Actor
	const int64_t Actor_cnt =(m_Actor ? (m_Actor->size()) : 0);
	lua_createtable(L, (int)Actor_cnt, 0);
	for(int64_t i=0; i<Actor_cnt; ++i){
		ActorConfig* item =dynamic_cast< ActorConfig* >(m_Actor->get(i));
		::core::ToLua< ActorConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Actor");
	return true;
}
bool ActorManagerConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Actor
	lua_getfield(L, idx, "Actor");
	if(lua_isnil(L, -1)){
		_init_field_Actor();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Actor_cnt =lua_objlen(L, -1);
			if(m_Actor == 0){
				m_Actor =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Actor);
			}
			for(int64_t i=0; i<Actor_cnt; ++i){
				ActorConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< ActorConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Actor->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool ActorManagerConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Actor
	str->appendString(::core::String::Format("%sActor ={\n", tab(tab_count+1)));
	const int64_t Actor_cnt =(m_Actor ? (m_Actor->size()) : 0);
	for(int64_t i=0; i<Actor_cnt; ++i){
		if(ActorConfig* item =dynamic_cast< ActorConfig* >(m_Actor->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(ActorManagerConfig, ::core::Array*, ActorConfig*, Actor)
void ActorManagerConfig::_init_field_Actor(){
	m_Actor =0;
}
/** class DataServiceConfig **/
/** impl DataServiceConfig **/
DataServiceConfig::DataServiceConfig(){
}
DataServiceConfig::~DataServiceConfig(){
}
void DataServiceConfig::init(){
	Super::init();
	_init_field_Mysql();
	_init_field_Redis();
}
void DataServiceConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* DataServiceConfig::Creator(){
	return ::core::SafeNew<DataServiceConfig>();
}
void DataServiceConfig::clean(){
	// clean Mysql 
	CLEAN_POINTER(m_Mysql);
	// clean Redis 
	CLEAN_POINTER(m_Redis);
}
int64_t DataServiceConfig::group(){
	return 1;
}
int64_t DataServiceConfig::id(){
	return 3;
}
const char* DataServiceConfig::name(){
	return "DataServiceConfig";
}
bool DataServiceConfig::check(){
	//// check Mysql 
	// check value
	//// check Redis 
	// check value
	return true;
}
::core::Bytes* DataServiceConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Mysql
	::core::ToBytes< MysqlConfig* >(bytes, m_Mysql);
	/// Redis
	::core::ToBytes< RedisConfig* >(bytes, m_Redis);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool DataServiceConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Mysql
	if(just_init){
		_init_field_Mysql();
	}
	else{
		if(!::core::FromBytes< MysqlConfig* >(bytes, m_Mysql)){ clean(); return false; }
		RETAIN_POINTER(m_Mysql);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal DataServiceConfig from bytes");
			return false;
		}
	}
	/// Redis
	if(just_init){
		_init_field_Redis();
	}
	else{
		if(!::core::FromBytes< RedisConfig* >(bytes, m_Redis)){ clean(); return false; }
		RETAIN_POINTER(m_Redis);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal DataServiceConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool DataServiceConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Mysql
	::core::ToLua< MysqlConfig* >(L, m_Mysql);
	lua_setfield(L, -2, "Mysql");
	// Redis
	::core::ToLua< RedisConfig* >(L, m_Redis);
	lua_setfield(L, -2, "Redis");
	return true;
}
bool DataServiceConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Mysql
	lua_getfield(L, idx, "Mysql");
	if(lua_isnil(L, -1)){
		_init_field_Mysql();
	}
	else{
		if(!::core::FromLua< MysqlConfig* >(L, -1, m_Mysql)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Mysql);
	}
	lua_settop(L, old_top);
	// Redis
	lua_getfield(L, idx, "Redis");
	if(lua_isnil(L, -1)){
		_init_field_Redis();
	}
	else{
		if(!::core::FromLua< RedisConfig* >(L, -1, m_Redis)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Redis);
	}
	lua_settop(L, old_top);
	return check();
}
bool DataServiceConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Mysql
	if(m_Mysql){
		str->appendString(::core::String::Format("%sMysql =\n", tab(tab_count+1)));
		m_Mysql->appendToString(str, tab_count + 1);
	}
	// Redis
	if(m_Redis){
		str->appendString(::core::String::Format("%sRedis =\n", tab(tab_count+1)));
		m_Redis->appendToString(str, tab_count + 1);
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(DataServiceConfig, MysqlConfig*, Mysql)
void DataServiceConfig::_init_field_Mysql(){
	m_Mysql =0;
}
DEFINE_PROPERTY_P(DataServiceConfig, RedisConfig*, Redis)
void DataServiceConfig::_init_field_Redis(){
	m_Redis =0;
}
/** class LogServiceConfig **/
/** impl LogServiceConfig **/
LogServiceConfig::LogServiceConfig(){
}
LogServiceConfig::~LogServiceConfig(){
}
void LogServiceConfig::init(){
	Super::init();
	_init_field_Target();
}
void LogServiceConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* LogServiceConfig::Creator(){
	return ::core::SafeNew<LogServiceConfig>();
}
void LogServiceConfig::clean(){
	// clean Target 
	CLEAN_POINTER(m_Target);
}
int64_t LogServiceConfig::group(){
	return 1;
}
int64_t LogServiceConfig::id(){
	return 4;
}
const char* LogServiceConfig::name(){
	return "LogServiceConfig";
}
bool LogServiceConfig::check(){
	//// check Target 
	// check value
	const int64_t n_so_Target =(m_Target ? m_Target->size() : 0);
	for(int64_t i=0; i<n_so_Target; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Target->get(i))){
			if(o->check() == false){
				ERROR("protocol LogServiceConfig check field, check the %lldth Target error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* LogServiceConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Target
	const int64_t Target_cnt =(m_Target ? (m_Target->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Target_cnt);
	for(int64_t i=0; i<Target_cnt; ++i){
		LogTargetConfig* item =dynamic_cast< LogTargetConfig* >(m_Target->get(i));
		::core::ToBytes< LogTargetConfig* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool LogServiceConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Target
	if(just_init){
		_init_field_Target();
	}
	else{
		if(m_Target == 0){
			m_Target =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Target);
		}
		int64_t Target_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Target_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Target_cnt; ++i){
			LogTargetConfig* item =0;
			if(!::core::FromBytes< LogTargetConfig* >(bytes, item)){ clean(); return false; }
			m_Target->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal LogServiceConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool LogServiceConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Target
	const int64_t Target_cnt =(m_Target ? (m_Target->size()) : 0);
	lua_createtable(L, (int)Target_cnt, 0);
	for(int64_t i=0; i<Target_cnt; ++i){
		LogTargetConfig* item =dynamic_cast< LogTargetConfig* >(m_Target->get(i));
		::core::ToLua< LogTargetConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Target");
	return true;
}
bool LogServiceConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Target
	lua_getfield(L, idx, "Target");
	if(lua_isnil(L, -1)){
		_init_field_Target();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Target_cnt =lua_objlen(L, -1);
			if(m_Target == 0){
				m_Target =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Target);
			}
			for(int64_t i=0; i<Target_cnt; ++i){
				LogTargetConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< LogTargetConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Target->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool LogServiceConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Target
	str->appendString(::core::String::Format("%sTarget ={\n", tab(tab_count+1)));
	const int64_t Target_cnt =(m_Target ? (m_Target->size()) : 0);
	for(int64_t i=0; i<Target_cnt; ++i){
		if(LogTargetConfig* item =dynamic_cast< LogTargetConfig* >(m_Target->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(LogServiceConfig, ::core::Array*, LogTargetConfig*, Target)
void LogServiceConfig::_init_field_Target(){
	m_Target =0;
}
/** class LogTargetConfig **/
/** impl LogTargetConfig **/
LogTargetConfig::LogTargetConfig(){
}
LogTargetConfig::~LogTargetConfig(){
}
void LogTargetConfig::init(){
	Super::init();
	_init_field_Type();
	_init_field_FileName();
	_init_field_FileLifeTime();
	_init_field_FlushMode();
}
void LogTargetConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* LogTargetConfig::Creator(){
	return ::core::SafeNew<LogTargetConfig>();
}
void LogTargetConfig::clean(){
	// clean Type 
	m_Type =0;
	// clean FileName 
	CLEAN_POINTER(m_FileName);
	// clean FileLifeTime 
	m_FileLifeTime =0;
	// clean FlushMode 
	m_FlushMode =0;
}
int64_t LogTargetConfig::group(){
	return 1;
}
int64_t LogTargetConfig::id(){
	return 5;
}
const char* LogTargetConfig::name(){
	return "LogTargetConfig";
}
bool LogTargetConfig::check(){
	//// check Type 
	// check value
	if(m_Type < 1){
		ERROR("protocol LogTargetConfig check field, Type < 1");
		return false;
	}
	//// check FileName 
	// check value
	if(::core::String::Length(m_FileName) < 1){
		ERROR("%s %lld", m_FileName?m_FileName->c_str():"null", ::core::String::Length(m_FileName));
		ERROR("protocol LogTargetConfig check field, length(FileName) < 1");
		return false;
	}
	//// check FileLifeTime 
	// check value
	if(m_FileLifeTime < 60){
		ERROR("protocol LogTargetConfig check field, FileLifeTime < 60");
		return false;
	}
	//// check FlushMode 
	// check value
	if(m_FlushMode < 0){
		ERROR("protocol LogTargetConfig check field, FlushMode < 0");
		return false;
	}
	return true;
}
::core::Bytes* LogTargetConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Type
	::core::ToBytes< int64_t >(bytes, m_Type);
	/// FileName
	::core::ToBytes< ::core::String* >(bytes, m_FileName);
	/// FileLifeTime
	::core::ToBytes< int64_t >(bytes, m_FileLifeTime);
	/// FlushMode
	::core::ToBytes< int64_t >(bytes, m_FlushMode);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool LogTargetConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Type
	if(just_init){
		_init_field_Type();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Type)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal LogTargetConfig from bytes");
			return false;
		}
	}
	/// FileName
	if(just_init){
		_init_field_FileName();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_FileName)){ clean(); return false; }
		RETAIN_POINTER(m_FileName);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal LogTargetConfig from bytes");
			return false;
		}
	}
	/// FileLifeTime
	if(just_init){
		_init_field_FileLifeTime();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_FileLifeTime)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal LogTargetConfig from bytes");
			return false;
		}
	}
	/// FlushMode
	if(just_init){
		_init_field_FlushMode();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_FlushMode)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal LogTargetConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool LogTargetConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Type
	::core::ToLua< int64_t >(L, m_Type);
	lua_setfield(L, -2, "Type");
	// FileName
	::core::ToLua< ::core::String* >(L, m_FileName);
	lua_setfield(L, -2, "FileName");
	// FileLifeTime
	::core::ToLua< int64_t >(L, m_FileLifeTime);
	lua_setfield(L, -2, "FileLifeTime");
	// FlushMode
	::core::ToLua< int64_t >(L, m_FlushMode);
	lua_setfield(L, -2, "FlushMode");
	return true;
}
bool LogTargetConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Type
	lua_getfield(L, idx, "Type");
	if(lua_isnil(L, -1)){
		_init_field_Type();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Type)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// FileName
	lua_getfield(L, idx, "FileName");
	if(lua_isnil(L, -1)){
		_init_field_FileName();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_FileName)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_FileName);
	}
	lua_settop(L, old_top);
	// FileLifeTime
	lua_getfield(L, idx, "FileLifeTime");
	if(lua_isnil(L, -1)){
		_init_field_FileLifeTime();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_FileLifeTime)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// FlushMode
	lua_getfield(L, idx, "FlushMode");
	if(lua_isnil(L, -1)){
		_init_field_FlushMode();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_FlushMode)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	return check();
}
bool LogTargetConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Type
	str->appendString(::core::String::Format("%sType =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Type)->c_str()));
	// FileName
	str->appendString(::core::String::Format("%sFileName =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_FileName)->c_str()));
	// FileLifeTime
	str->appendString(::core::String::Format("%sFileLifeTime =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_FileLifeTime)->c_str()));
	// FlushMode
	str->appendString(::core::String::Format("%sFlushMode =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_FlushMode)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(LogTargetConfig, int64_t, Type)
void LogTargetConfig::_init_field_Type(){
	m_Type =0;
}
DEFINE_PROPERTY_P(LogTargetConfig, ::core::String*, FileName)
void LogTargetConfig::_init_field_FileName(){
	m_FileName =::core::String::NewString("");
	RETAIN_POINTER(m_FileName);
}
DEFINE_PROPERTY(LogTargetConfig, int64_t, FileLifeTime)
void LogTargetConfig::_init_field_FileLifeTime(){
	m_FileLifeTime =1800;
}
DEFINE_PROPERTY(LogTargetConfig, int64_t, FlushMode)
void LogTargetConfig::_init_field_FlushMode(){
	m_FlushMode =0;
}
/** class MysqlConfig **/
/** impl MysqlConfig **/
MysqlConfig::MysqlConfig(){
}
MysqlConfig::~MysqlConfig(){
}
void MysqlConfig::init(){
	Super::init();
	_init_field_Host();
	_init_field_Port();
	_init_field_User();
	_init_field_Passwd();
}
void MysqlConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* MysqlConfig::Creator(){
	return ::core::SafeNew<MysqlConfig>();
}
void MysqlConfig::clean(){
	// clean Host 
	CLEAN_POINTER(m_Host);
	// clean Port 
	m_Port =0;
	// clean User 
	CLEAN_POINTER(m_User);
	// clean Passwd 
	CLEAN_POINTER(m_Passwd);
}
int64_t MysqlConfig::group(){
	return 1;
}
int64_t MysqlConfig::id(){
	return 6;
}
const char* MysqlConfig::name(){
	return "MysqlConfig";
}
bool MysqlConfig::check(){
	//// check Host 
	// check value
	//// check Port 
	// check value
	//// check User 
	// check value
	//// check Passwd 
	// check value
	return true;
}
::core::Bytes* MysqlConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Host
	::core::ToBytes< ::core::String* >(bytes, m_Host);
	/// Port
	::core::ToBytes< int64_t >(bytes, m_Port);
	/// User
	::core::ToBytes< ::core::String* >(bytes, m_User);
	/// Passwd
	::core::ToBytes< ::core::String* >(bytes, m_Passwd);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool MysqlConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Host
	if(just_init){
		_init_field_Host();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Host)){ clean(); return false; }
		RETAIN_POINTER(m_Host);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal MysqlConfig from bytes");
			return false;
		}
	}
	/// Port
	if(just_init){
		_init_field_Port();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Port)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal MysqlConfig from bytes");
			return false;
		}
	}
	/// User
	if(just_init){
		_init_field_User();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_User)){ clean(); return false; }
		RETAIN_POINTER(m_User);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal MysqlConfig from bytes");
			return false;
		}
	}
	/// Passwd
	if(just_init){
		_init_field_Passwd();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Passwd)){ clean(); return false; }
		RETAIN_POINTER(m_Passwd);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal MysqlConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool MysqlConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Host
	::core::ToLua< ::core::String* >(L, m_Host);
	lua_setfield(L, -2, "Host");
	// Port
	::core::ToLua< int64_t >(L, m_Port);
	lua_setfield(L, -2, "Port");
	// User
	::core::ToLua< ::core::String* >(L, m_User);
	lua_setfield(L, -2, "User");
	// Passwd
	::core::ToLua< ::core::String* >(L, m_Passwd);
	lua_setfield(L, -2, "Passwd");
	return true;
}
bool MysqlConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Host
	lua_getfield(L, idx, "Host");
	if(lua_isnil(L, -1)){
		_init_field_Host();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Host)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Host);
	}
	lua_settop(L, old_top);
	// Port
	lua_getfield(L, idx, "Port");
	if(lua_isnil(L, -1)){
		_init_field_Port();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Port)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// User
	lua_getfield(L, idx, "User");
	if(lua_isnil(L, -1)){
		_init_field_User();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_User)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_User);
	}
	lua_settop(L, old_top);
	// Passwd
	lua_getfield(L, idx, "Passwd");
	if(lua_isnil(L, -1)){
		_init_field_Passwd();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Passwd)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Passwd);
	}
	lua_settop(L, old_top);
	return check();
}
bool MysqlConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Host
	str->appendString(::core::String::Format("%sHost =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Host)->c_str()));
	// Port
	str->appendString(::core::String::Format("%sPort =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Port)->c_str()));
	// User
	str->appendString(::core::String::Format("%sUser =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_User)->c_str()));
	// Passwd
	str->appendString(::core::String::Format("%sPasswd =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Passwd)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(MysqlConfig, ::core::String*, Host)
void MysqlConfig::_init_field_Host(){
	m_Host =::core::String::NewString("");
	RETAIN_POINTER(m_Host);
}
DEFINE_PROPERTY(MysqlConfig, int64_t, Port)
void MysqlConfig::_init_field_Port(){
	m_Port =0;
}
DEFINE_PROPERTY_P(MysqlConfig, ::core::String*, User)
void MysqlConfig::_init_field_User(){
	m_User =::core::String::NewString("");
	RETAIN_POINTER(m_User);
}
DEFINE_PROPERTY_P(MysqlConfig, ::core::String*, Passwd)
void MysqlConfig::_init_field_Passwd(){
	m_Passwd =::core::String::NewString("");
	RETAIN_POINTER(m_Passwd);
}
/** class NetworkConfig **/
/** impl NetworkConfig **/
NetworkConfig::NetworkConfig(){
}
NetworkConfig::~NetworkConfig(){
}
void NetworkConfig::init(){
	Super::init();
	_init_field_SlaveCount();
	_init_field_Listener();
	_init_field_Remote();
	_init_field_Route();
}
void NetworkConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* NetworkConfig::Creator(){
	return ::core::SafeNew<NetworkConfig>();
}
void NetworkConfig::clean(){
	// clean SlaveCount 
	m_SlaveCount =0;
	// clean Listener 
	CLEAN_POINTER(m_Listener);
	// clean Remote 
	CLEAN_POINTER(m_Remote);
	// clean Route 
	CLEAN_POINTER(m_Route);
}
int64_t NetworkConfig::group(){
	return 1;
}
int64_t NetworkConfig::id(){
	return 7;
}
const char* NetworkConfig::name(){
	return "NetworkConfig";
}
bool NetworkConfig::check(){
	//// check SlaveCount 
	// check value
	//// check Listener 
	// check value
	const int64_t n_so_Listener =(m_Listener ? m_Listener->size() : 0);
	for(int64_t i=0; i<n_so_Listener; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Listener->get(i))){
			if(o->check() == false){
				ERROR("protocol NetworkConfig check field, check the %lldth Listener error", (long long)i);
				return false;
			}
		}
	}
	//// check Remote 
	// check value
	const int64_t n_so_Remote =(m_Remote ? m_Remote->size() : 0);
	for(int64_t i=0; i<n_so_Remote; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Remote->get(i))){
			if(o->check() == false){
				ERROR("protocol NetworkConfig check field, check the %lldth Remote error", (long long)i);
				return false;
			}
		}
	}
	//// check Route 
	// check value
	const int64_t n_so_Route =(m_Route ? m_Route->size() : 0);
	for(int64_t i=0; i<n_so_Route; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Route->get(i))){
			if(o->check() == false){
				ERROR("protocol NetworkConfig check field, check the %lldth Route error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* NetworkConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// SlaveCount
	::core::ToBytes< int64_t >(bytes, m_SlaveCount);
	/// Listener
	const int64_t Listener_cnt =(m_Listener ? (m_Listener->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Listener_cnt);
	for(int64_t i=0; i<Listener_cnt; ++i){
		NetworkListenerConfig* item =dynamic_cast< NetworkListenerConfig* >(m_Listener->get(i));
		::core::ToBytes< NetworkListenerConfig* >(bytes, item);
	}
	/// Remote
	const int64_t Remote_cnt =(m_Remote ? (m_Remote->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Remote_cnt);
	for(int64_t i=0; i<Remote_cnt; ++i){
		NetworkConnectionConfig* item =dynamic_cast< NetworkConnectionConfig* >(m_Remote->get(i));
		::core::ToBytes< NetworkConnectionConfig* >(bytes, item);
	}
	/// Route
	const int64_t Route_cnt =(m_Route ? (m_Route->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Route_cnt);
	for(int64_t i=0; i<Route_cnt; ++i){
		NetworkRouteConfig* item =dynamic_cast< NetworkRouteConfig* >(m_Route->get(i));
		::core::ToBytes< NetworkRouteConfig* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool NetworkConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// SlaveCount
	if(just_init){
		_init_field_SlaveCount();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_SlaveCount)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConfig from bytes");
			return false;
		}
	}
	/// Listener
	if(just_init){
		_init_field_Listener();
	}
	else{
		if(m_Listener == 0){
			m_Listener =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Listener);
		}
		int64_t Listener_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Listener_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Listener_cnt; ++i){
			NetworkListenerConfig* item =0;
			if(!::core::FromBytes< NetworkListenerConfig* >(bytes, item)){ clean(); return false; }
			m_Listener->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConfig from bytes");
			return false;
		}
	}
	/// Remote
	if(just_init){
		_init_field_Remote();
	}
	else{
		if(m_Remote == 0){
			m_Remote =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Remote);
		}
		int64_t Remote_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Remote_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Remote_cnt; ++i){
			NetworkConnectionConfig* item =0;
			if(!::core::FromBytes< NetworkConnectionConfig* >(bytes, item)){ clean(); return false; }
			m_Remote->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConfig from bytes");
			return false;
		}
	}
	/// Route
	if(just_init){
		_init_field_Route();
	}
	else{
		if(m_Route == 0){
			m_Route =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Route);
		}
		int64_t Route_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Route_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Route_cnt; ++i){
			NetworkRouteConfig* item =0;
			if(!::core::FromBytes< NetworkRouteConfig* >(bytes, item)){ clean(); return false; }
			m_Route->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool NetworkConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// SlaveCount
	::core::ToLua< int64_t >(L, m_SlaveCount);
	lua_setfield(L, -2, "SlaveCount");
	// Listener
	const int64_t Listener_cnt =(m_Listener ? (m_Listener->size()) : 0);
	lua_createtable(L, (int)Listener_cnt, 0);
	for(int64_t i=0; i<Listener_cnt; ++i){
		NetworkListenerConfig* item =dynamic_cast< NetworkListenerConfig* >(m_Listener->get(i));
		::core::ToLua< NetworkListenerConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Listener");
	// Remote
	const int64_t Remote_cnt =(m_Remote ? (m_Remote->size()) : 0);
	lua_createtable(L, (int)Remote_cnt, 0);
	for(int64_t i=0; i<Remote_cnt; ++i){
		NetworkConnectionConfig* item =dynamic_cast< NetworkConnectionConfig* >(m_Remote->get(i));
		::core::ToLua< NetworkConnectionConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Remote");
	// Route
	const int64_t Route_cnt =(m_Route ? (m_Route->size()) : 0);
	lua_createtable(L, (int)Route_cnt, 0);
	for(int64_t i=0; i<Route_cnt; ++i){
		NetworkRouteConfig* item =dynamic_cast< NetworkRouteConfig* >(m_Route->get(i));
		::core::ToLua< NetworkRouteConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Route");
	return true;
}
bool NetworkConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// SlaveCount
	lua_getfield(L, idx, "SlaveCount");
	if(lua_isnil(L, -1)){
		_init_field_SlaveCount();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_SlaveCount)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Listener
	lua_getfield(L, idx, "Listener");
	if(lua_isnil(L, -1)){
		_init_field_Listener();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Listener_cnt =lua_objlen(L, -1);
			if(m_Listener == 0){
				m_Listener =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Listener);
			}
			for(int64_t i=0; i<Listener_cnt; ++i){
				NetworkListenerConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< NetworkListenerConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Listener->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	// Remote
	lua_getfield(L, idx, "Remote");
	if(lua_isnil(L, -1)){
		_init_field_Remote();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Remote_cnt =lua_objlen(L, -1);
			if(m_Remote == 0){
				m_Remote =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Remote);
			}
			for(int64_t i=0; i<Remote_cnt; ++i){
				NetworkConnectionConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< NetworkConnectionConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Remote->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	// Route
	lua_getfield(L, idx, "Route");
	if(lua_isnil(L, -1)){
		_init_field_Route();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Route_cnt =lua_objlen(L, -1);
			if(m_Route == 0){
				m_Route =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Route);
			}
			for(int64_t i=0; i<Route_cnt; ++i){
				NetworkRouteConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< NetworkRouteConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Route->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool NetworkConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// SlaveCount
	str->appendString(::core::String::Format("%sSlaveCount =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_SlaveCount)->c_str()));
	// Listener
	str->appendString(::core::String::Format("%sListener ={\n", tab(tab_count+1)));
	const int64_t Listener_cnt =(m_Listener ? (m_Listener->size()) : 0);
	for(int64_t i=0; i<Listener_cnt; ++i){
		if(NetworkListenerConfig* item =dynamic_cast< NetworkListenerConfig* >(m_Listener->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	// Remote
	str->appendString(::core::String::Format("%sRemote ={\n", tab(tab_count+1)));
	const int64_t Remote_cnt =(m_Remote ? (m_Remote->size()) : 0);
	for(int64_t i=0; i<Remote_cnt; ++i){
		if(NetworkConnectionConfig* item =dynamic_cast< NetworkConnectionConfig* >(m_Remote->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	// Route
	str->appendString(::core::String::Format("%sRoute ={\n", tab(tab_count+1)));
	const int64_t Route_cnt =(m_Route ? (m_Route->size()) : 0);
	for(int64_t i=0; i<Route_cnt; ++i){
		if(NetworkRouteConfig* item =dynamic_cast< NetworkRouteConfig* >(m_Route->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(NetworkConfig, int64_t, SlaveCount)
void NetworkConfig::_init_field_SlaveCount(){
	m_SlaveCount =0;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(NetworkConfig, ::core::Array*, NetworkListenerConfig*, Listener)
void NetworkConfig::_init_field_Listener(){
	m_Listener =0;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(NetworkConfig, ::core::Array*, NetworkConnectionConfig*, Remote)
void NetworkConfig::_init_field_Remote(){
	m_Remote =0;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(NetworkConfig, ::core::Array*, NetworkRouteConfig*, Route)
void NetworkConfig::_init_field_Route(){
	m_Route =0;
}
/** class NetworkConnectionConfig **/
/** impl NetworkConnectionConfig **/
NetworkConnectionConfig::NetworkConnectionConfig(){
}
NetworkConnectionConfig::~NetworkConnectionConfig(){
}
void NetworkConnectionConfig::init(){
	Super::init();
	_init_field_Type();
	_init_field_Id();
	_init_field_Host();
	_init_field_Port();
	_init_field_RunnerBegin();
	_init_field_RunnerRange();
}
void NetworkConnectionConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* NetworkConnectionConfig::Creator(){
	return ::core::SafeNew<NetworkConnectionConfig>();
}
void NetworkConnectionConfig::clean(){
	// clean Type 
	CLEAN_POINTER(m_Type);
	// clean Id 
	m_Id =0;
	// clean Host 
	CLEAN_POINTER(m_Host);
	// clean Port 
	m_Port =0;
	// clean RunnerBegin 
	m_RunnerBegin =0;
	// clean RunnerRange 
	m_RunnerRange =0;
}
int64_t NetworkConnectionConfig::group(){
	return 1;
}
int64_t NetworkConnectionConfig::id(){
	return 8;
}
const char* NetworkConnectionConfig::name(){
	return "NetworkConnectionConfig";
}
bool NetworkConnectionConfig::check(){
	//// check Type 
	// check value
	//// check Id 
	// check value
	//// check Host 
	// check value
	//// check Port 
	// check value
	//// check RunnerBegin 
	// check value
	//// check RunnerRange 
	// check value
	return true;
}
::core::Bytes* NetworkConnectionConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Type
	::core::ToBytes< ::core::String* >(bytes, m_Type);
	/// Id
	::core::ToBytes< int64_t >(bytes, m_Id);
	/// Host
	::core::ToBytes< ::core::String* >(bytes, m_Host);
	/// Port
	::core::ToBytes< int32_t >(bytes, m_Port);
	/// RunnerBegin
	::core::ToBytes< int64_t >(bytes, m_RunnerBegin);
	/// RunnerRange
	::core::ToBytes< int64_t >(bytes, m_RunnerRange);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool NetworkConnectionConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Type
	if(just_init){
		_init_field_Type();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Type)){ clean(); return false; }
		RETAIN_POINTER(m_Type);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// Id
	if(just_init){
		_init_field_Id();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Id)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// Host
	if(just_init){
		_init_field_Host();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Host)){ clean(); return false; }
		RETAIN_POINTER(m_Host);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// Port
	if(just_init){
		_init_field_Port();
	}
	else{
		if(!::core::FromBytes< int32_t >(bytes, m_Port)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// RunnerBegin
	if(just_init){
		_init_field_RunnerBegin();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_RunnerBegin)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// RunnerRange
	if(just_init){
		_init_field_RunnerRange();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_RunnerRange)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkConnectionConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool NetworkConnectionConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Type
	::core::ToLua< ::core::String* >(L, m_Type);
	lua_setfield(L, -2, "Type");
	// Id
	::core::ToLua< int64_t >(L, m_Id);
	lua_setfield(L, -2, "Id");
	// Host
	::core::ToLua< ::core::String* >(L, m_Host);
	lua_setfield(L, -2, "Host");
	// Port
	::core::ToLua< int32_t >(L, m_Port);
	lua_setfield(L, -2, "Port");
	// RunnerBegin
	::core::ToLua< int64_t >(L, m_RunnerBegin);
	lua_setfield(L, -2, "RunnerBegin");
	// RunnerRange
	::core::ToLua< int64_t >(L, m_RunnerRange);
	lua_setfield(L, -2, "RunnerRange");
	return true;
}
bool NetworkConnectionConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Type
	lua_getfield(L, idx, "Type");
	if(lua_isnil(L, -1)){
		_init_field_Type();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Type)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Type);
	}
	lua_settop(L, old_top);
	// Id
	lua_getfield(L, idx, "Id");
	if(lua_isnil(L, -1)){
		_init_field_Id();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Id)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Host
	lua_getfield(L, idx, "Host");
	if(lua_isnil(L, -1)){
		_init_field_Host();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Host)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Host);
	}
	lua_settop(L, old_top);
	// Port
	lua_getfield(L, idx, "Port");
	if(lua_isnil(L, -1)){
		_init_field_Port();
	}
	else{
		if(!::core::FromLua< int32_t >(L, -1, m_Port)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// RunnerBegin
	lua_getfield(L, idx, "RunnerBegin");
	if(lua_isnil(L, -1)){
		_init_field_RunnerBegin();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_RunnerBegin)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// RunnerRange
	lua_getfield(L, idx, "RunnerRange");
	if(lua_isnil(L, -1)){
		_init_field_RunnerRange();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_RunnerRange)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	return check();
}
bool NetworkConnectionConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Type
	str->appendString(::core::String::Format("%sType =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Type)->c_str()));
	// Id
	str->appendString(::core::String::Format("%sId =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Id)->c_str()));
	// Host
	str->appendString(::core::String::Format("%sHost =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Host)->c_str()));
	// Port
	str->appendString(::core::String::Format("%sPort =%s,\n", tab(tab_count+1), ::core::ToString< int32_t >(m_Port)->c_str()));
	// RunnerBegin
	str->appendString(::core::String::Format("%sRunnerBegin =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_RunnerBegin)->c_str()));
	// RunnerRange
	str->appendString(::core::String::Format("%sRunnerRange =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_RunnerRange)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(NetworkConnectionConfig, ::core::String*, Type)
void NetworkConnectionConfig::_init_field_Type(){
	m_Type =::core::String::NewString("");
	RETAIN_POINTER(m_Type);
}
DEFINE_PROPERTY(NetworkConnectionConfig, int64_t, Id)
void NetworkConnectionConfig::_init_field_Id(){
	m_Id =0;
}
DEFINE_PROPERTY_P(NetworkConnectionConfig, ::core::String*, Host)
void NetworkConnectionConfig::_init_field_Host(){
	m_Host =::core::String::NewString("localhost");
	RETAIN_POINTER(m_Host);
}
DEFINE_PROPERTY(NetworkConnectionConfig, int32_t, Port)
void NetworkConnectionConfig::_init_field_Port(){
	m_Port =0;
}
DEFINE_PROPERTY(NetworkConnectionConfig, int64_t, RunnerBegin)
void NetworkConnectionConfig::_init_field_RunnerBegin(){
	m_RunnerBegin =0;
}
DEFINE_PROPERTY(NetworkConnectionConfig, int64_t, RunnerRange)
void NetworkConnectionConfig::_init_field_RunnerRange(){
	m_RunnerRange =10000000;
}
/** class NetworkListenerConfig **/
/** impl NetworkListenerConfig **/
NetworkListenerConfig::NetworkListenerConfig(){
}
NetworkListenerConfig::~NetworkListenerConfig(){
}
void NetworkListenerConfig::init(){
	Super::init();
	_init_field_Type();
	_init_field_Id();
	_init_field_Path();
	_init_field_Port();
	_init_field_DeliverBegin();
	_init_field_DeliverRange();
	_init_field_RunnerBegin();
	_init_field_RunnerRange();
}
void NetworkListenerConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* NetworkListenerConfig::Creator(){
	return ::core::SafeNew<NetworkListenerConfig>();
}
void NetworkListenerConfig::clean(){
	// clean Type 
	CLEAN_POINTER(m_Type);
	// clean Id 
	m_Id =0;
	// clean Path 
	CLEAN_POINTER(m_Path);
	// clean Port 
	m_Port =0;
	// clean DeliverBegin 
	m_DeliverBegin =0;
	// clean DeliverRange 
	m_DeliverRange =0;
	// clean RunnerBegin 
	m_RunnerBegin =0;
	// clean RunnerRange 
	m_RunnerRange =0;
}
int64_t NetworkListenerConfig::group(){
	return 1;
}
int64_t NetworkListenerConfig::id(){
	return 9;
}
const char* NetworkListenerConfig::name(){
	return "NetworkListenerConfig";
}
bool NetworkListenerConfig::check(){
	//// check Type 
	// check value
	//// check Id 
	// check value
	//// check Path 
	// check value
	//// check Port 
	// check value
	//// check DeliverBegin 
	// check value
	//// check DeliverRange 
	// check value
	//// check RunnerBegin 
	// check value
	//// check RunnerRange 
	// check value
	return true;
}
::core::Bytes* NetworkListenerConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Type
	::core::ToBytes< ::core::String* >(bytes, m_Type);
	/// Id
	::core::ToBytes< int64_t >(bytes, m_Id);
	/// Path
	::core::ToBytes< ::core::String* >(bytes, m_Path);
	/// Port
	::core::ToBytes< int32_t >(bytes, m_Port);
	/// DeliverBegin
	::core::ToBytes< int64_t >(bytes, m_DeliverBegin);
	/// DeliverRange
	::core::ToBytes< int64_t >(bytes, m_DeliverRange);
	/// RunnerBegin
	::core::ToBytes< int64_t >(bytes, m_RunnerBegin);
	/// RunnerRange
	::core::ToBytes< int64_t >(bytes, m_RunnerRange);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool NetworkListenerConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Type
	if(just_init){
		_init_field_Type();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Type)){ clean(); return false; }
		RETAIN_POINTER(m_Type);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// Id
	if(just_init){
		_init_field_Id();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Id)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// Path
	if(just_init){
		_init_field_Path();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Path)){ clean(); return false; }
		RETAIN_POINTER(m_Path);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// Port
	if(just_init){
		_init_field_Port();
	}
	else{
		if(!::core::FromBytes< int32_t >(bytes, m_Port)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// DeliverBegin
	if(just_init){
		_init_field_DeliverBegin();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_DeliverBegin)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// DeliverRange
	if(just_init){
		_init_field_DeliverRange();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_DeliverRange)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// RunnerBegin
	if(just_init){
		_init_field_RunnerBegin();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_RunnerBegin)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// RunnerRange
	if(just_init){
		_init_field_RunnerRange();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_RunnerRange)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkListenerConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool NetworkListenerConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Type
	::core::ToLua< ::core::String* >(L, m_Type);
	lua_setfield(L, -2, "Type");
	// Id
	::core::ToLua< int64_t >(L, m_Id);
	lua_setfield(L, -2, "Id");
	// Path
	::core::ToLua< ::core::String* >(L, m_Path);
	lua_setfield(L, -2, "Path");
	// Port
	::core::ToLua< int32_t >(L, m_Port);
	lua_setfield(L, -2, "Port");
	// DeliverBegin
	::core::ToLua< int64_t >(L, m_DeliverBegin);
	lua_setfield(L, -2, "DeliverBegin");
	// DeliverRange
	::core::ToLua< int64_t >(L, m_DeliverRange);
	lua_setfield(L, -2, "DeliverRange");
	// RunnerBegin
	::core::ToLua< int64_t >(L, m_RunnerBegin);
	lua_setfield(L, -2, "RunnerBegin");
	// RunnerRange
	::core::ToLua< int64_t >(L, m_RunnerRange);
	lua_setfield(L, -2, "RunnerRange");
	return true;
}
bool NetworkListenerConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Type
	lua_getfield(L, idx, "Type");
	if(lua_isnil(L, -1)){
		_init_field_Type();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Type)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Type);
	}
	lua_settop(L, old_top);
	// Id
	lua_getfield(L, idx, "Id");
	if(lua_isnil(L, -1)){
		_init_field_Id();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Id)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Path
	lua_getfield(L, idx, "Path");
	if(lua_isnil(L, -1)){
		_init_field_Path();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Path)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Path);
	}
	lua_settop(L, old_top);
	// Port
	lua_getfield(L, idx, "Port");
	if(lua_isnil(L, -1)){
		_init_field_Port();
	}
	else{
		if(!::core::FromLua< int32_t >(L, -1, m_Port)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// DeliverBegin
	lua_getfield(L, idx, "DeliverBegin");
	if(lua_isnil(L, -1)){
		_init_field_DeliverBegin();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_DeliverBegin)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// DeliverRange
	lua_getfield(L, idx, "DeliverRange");
	if(lua_isnil(L, -1)){
		_init_field_DeliverRange();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_DeliverRange)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// RunnerBegin
	lua_getfield(L, idx, "RunnerBegin");
	if(lua_isnil(L, -1)){
		_init_field_RunnerBegin();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_RunnerBegin)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// RunnerRange
	lua_getfield(L, idx, "RunnerRange");
	if(lua_isnil(L, -1)){
		_init_field_RunnerRange();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_RunnerRange)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	return check();
}
bool NetworkListenerConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Type
	str->appendString(::core::String::Format("%sType =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Type)->c_str()));
	// Id
	str->appendString(::core::String::Format("%sId =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Id)->c_str()));
	// Path
	str->appendString(::core::String::Format("%sPath =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Path)->c_str()));
	// Port
	str->appendString(::core::String::Format("%sPort =%s,\n", tab(tab_count+1), ::core::ToString< int32_t >(m_Port)->c_str()));
	// DeliverBegin
	str->appendString(::core::String::Format("%sDeliverBegin =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_DeliverBegin)->c_str()));
	// DeliverRange
	str->appendString(::core::String::Format("%sDeliverRange =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_DeliverRange)->c_str()));
	// RunnerBegin
	str->appendString(::core::String::Format("%sRunnerBegin =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_RunnerBegin)->c_str()));
	// RunnerRange
	str->appendString(::core::String::Format("%sRunnerRange =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_RunnerRange)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(NetworkListenerConfig, ::core::String*, Type)
void NetworkListenerConfig::_init_field_Type(){
	m_Type =::core::String::NewString("");
	RETAIN_POINTER(m_Type);
}
DEFINE_PROPERTY(NetworkListenerConfig, int64_t, Id)
void NetworkListenerConfig::_init_field_Id(){
	m_Id =0;
}
DEFINE_PROPERTY_P(NetworkListenerConfig, ::core::String*, Path)
void NetworkListenerConfig::_init_field_Path(){
	m_Path =::core::String::NewString("localhost");
	RETAIN_POINTER(m_Path);
}
DEFINE_PROPERTY(NetworkListenerConfig, int32_t, Port)
void NetworkListenerConfig::_init_field_Port(){
	m_Port =0;
}
DEFINE_PROPERTY(NetworkListenerConfig, int64_t, DeliverBegin)
void NetworkListenerConfig::_init_field_DeliverBegin(){
	m_DeliverBegin =0;
}
DEFINE_PROPERTY(NetworkListenerConfig, int64_t, DeliverRange)
void NetworkListenerConfig::_init_field_DeliverRange(){
	m_DeliverRange =0;
}
DEFINE_PROPERTY(NetworkListenerConfig, int64_t, RunnerBegin)
void NetworkListenerConfig::_init_field_RunnerBegin(){
	m_RunnerBegin =0;
}
DEFINE_PROPERTY(NetworkListenerConfig, int64_t, RunnerRange)
void NetworkListenerConfig::_init_field_RunnerRange(){
	m_RunnerRange =10000000;
}
/** class NetworkRouteConfig **/
/** impl NetworkRouteConfig **/
NetworkRouteConfig::NetworkRouteConfig(){
}
NetworkRouteConfig::~NetworkRouteConfig(){
}
void NetworkRouteConfig::init(){
	Super::init();
	_init_field_IdService();
	_init_field_IdBegin();
	_init_field_IdEnd();
	_init_field_Algrithm();
}
void NetworkRouteConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* NetworkRouteConfig::Creator(){
	return ::core::SafeNew<NetworkRouteConfig>();
}
void NetworkRouteConfig::clean(){
	// clean IdService 
	m_IdService =0;
	// clean IdBegin 
	m_IdBegin =0;
	// clean IdEnd 
	m_IdEnd =0;
	// clean Algrithm 
	m_Algrithm =0;
}
int64_t NetworkRouteConfig::group(){
	return 1;
}
int64_t NetworkRouteConfig::id(){
	return 10;
}
const char* NetworkRouteConfig::name(){
	return "NetworkRouteConfig";
}
bool NetworkRouteConfig::check(){
	//// check IdService 
	// check value
	//// check IdBegin 
	// check value
	//// check IdEnd 
	// check value
	//// check Algrithm 
	// check value
	return true;
}
::core::Bytes* NetworkRouteConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// IdService
	::core::ToBytes< int64_t >(bytes, m_IdService);
	/// IdBegin
	::core::ToBytes< int64_t >(bytes, m_IdBegin);
	/// IdEnd
	::core::ToBytes< int64_t >(bytes, m_IdEnd);
	/// Algrithm
	::core::ToBytes< int64_t >(bytes, m_Algrithm);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool NetworkRouteConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// IdService
	if(just_init){
		_init_field_IdService();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdService)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkRouteConfig from bytes");
			return false;
		}
	}
	/// IdBegin
	if(just_init){
		_init_field_IdBegin();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdBegin)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkRouteConfig from bytes");
			return false;
		}
	}
	/// IdEnd
	if(just_init){
		_init_field_IdEnd();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdEnd)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkRouteConfig from bytes");
			return false;
		}
	}
	/// Algrithm
	if(just_init){
		_init_field_Algrithm();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Algrithm)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal NetworkRouteConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool NetworkRouteConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// IdService
	::core::ToLua< int64_t >(L, m_IdService);
	lua_setfield(L, -2, "IdService");
	// IdBegin
	::core::ToLua< int64_t >(L, m_IdBegin);
	lua_setfield(L, -2, "IdBegin");
	// IdEnd
	::core::ToLua< int64_t >(L, m_IdEnd);
	lua_setfield(L, -2, "IdEnd");
	// Algrithm
	::core::ToLua< int64_t >(L, m_Algrithm);
	lua_setfield(L, -2, "Algrithm");
	return true;
}
bool NetworkRouteConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// IdService
	lua_getfield(L, idx, "IdService");
	if(lua_isnil(L, -1)){
		_init_field_IdService();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdService)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// IdBegin
	lua_getfield(L, idx, "IdBegin");
	if(lua_isnil(L, -1)){
		_init_field_IdBegin();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdBegin)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// IdEnd
	lua_getfield(L, idx, "IdEnd");
	if(lua_isnil(L, -1)){
		_init_field_IdEnd();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdEnd)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Algrithm
	lua_getfield(L, idx, "Algrithm");
	if(lua_isnil(L, -1)){
		_init_field_Algrithm();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Algrithm)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	return check();
}
bool NetworkRouteConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// IdService
	str->appendString(::core::String::Format("%sIdService =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdService)->c_str()));
	// IdBegin
	str->appendString(::core::String::Format("%sIdBegin =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdBegin)->c_str()));
	// IdEnd
	str->appendString(::core::String::Format("%sIdEnd =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdEnd)->c_str()));
	// Algrithm
	str->appendString(::core::String::Format("%sAlgrithm =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Algrithm)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(NetworkRouteConfig, int64_t, IdService)
void NetworkRouteConfig::_init_field_IdService(){
	m_IdService =0;
}
DEFINE_PROPERTY(NetworkRouteConfig, int64_t, IdBegin)
void NetworkRouteConfig::_init_field_IdBegin(){
	m_IdBegin =0;
}
DEFINE_PROPERTY(NetworkRouteConfig, int64_t, IdEnd)
void NetworkRouteConfig::_init_field_IdEnd(){
	m_IdEnd =0;
}
DEFINE_PROPERTY(NetworkRouteConfig, int64_t, Algrithm)
void NetworkRouteConfig::_init_field_Algrithm(){
	m_Algrithm =0;
}
/** class RedisConfig **/
/** impl RedisConfig **/
RedisConfig::RedisConfig(){
}
RedisConfig::~RedisConfig(){
}
void RedisConfig::init(){
	Super::init();
	_init_field_Host();
	_init_field_Port();
}
void RedisConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* RedisConfig::Creator(){
	return ::core::SafeNew<RedisConfig>();
}
void RedisConfig::clean(){
	// clean Host 
	CLEAN_POINTER(m_Host);
	// clean Port 
	m_Port =0;
}
int64_t RedisConfig::group(){
	return 1;
}
int64_t RedisConfig::id(){
	return 11;
}
const char* RedisConfig::name(){
	return "RedisConfig";
}
bool RedisConfig::check(){
	//// check Host 
	// check value
	//// check Port 
	// check value
	return true;
}
::core::Bytes* RedisConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Host
	::core::ToBytes< ::core::String* >(bytes, m_Host);
	/// Port
	::core::ToBytes< int64_t >(bytes, m_Port);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool RedisConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Host
	if(just_init){
		_init_field_Host();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Host)){ clean(); return false; }
		RETAIN_POINTER(m_Host);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal RedisConfig from bytes");
			return false;
		}
	}
	/// Port
	if(just_init){
		_init_field_Port();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Port)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal RedisConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool RedisConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Host
	::core::ToLua< ::core::String* >(L, m_Host);
	lua_setfield(L, -2, "Host");
	// Port
	::core::ToLua< int64_t >(L, m_Port);
	lua_setfield(L, -2, "Port");
	return true;
}
bool RedisConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Host
	lua_getfield(L, idx, "Host");
	if(lua_isnil(L, -1)){
		_init_field_Host();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Host)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Host);
	}
	lua_settop(L, old_top);
	// Port
	lua_getfield(L, idx, "Port");
	if(lua_isnil(L, -1)){
		_init_field_Port();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Port)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	return check();
}
bool RedisConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Host
	str->appendString(::core::String::Format("%sHost =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Host)->c_str()));
	// Port
	str->appendString(::core::String::Format("%sPort =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Port)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(RedisConfig, ::core::String*, Host)
void RedisConfig::_init_field_Host(){
	m_Host =::core::String::NewString("");
	RETAIN_POINTER(m_Host);
}
DEFINE_PROPERTY(RedisConfig, int64_t, Port)
void RedisConfig::_init_field_Port(){
	m_Port =0;
}
/** class ServiceConfig **/
/** impl ServiceConfig **/
ServiceConfig::ServiceConfig(){
}
ServiceConfig::~ServiceConfig(){
}
void ServiceConfig::init(){
	Super::init();
	_init_field_IdService();
	_init_field_IdBegin();
	_init_field_IdEnd();
	_init_field_Algrithm();
	_init_field_Path();
}
void ServiceConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* ServiceConfig::Creator(){
	return ::core::SafeNew<ServiceConfig>();
}
void ServiceConfig::clean(){
	// clean IdService 
	m_IdService =0;
	// clean IdBegin 
	m_IdBegin =0;
	// clean IdEnd 
	m_IdEnd =0;
	// clean Algrithm 
	m_Algrithm =0;
	// clean Path 
	CLEAN_POINTER(m_Path);
}
int64_t ServiceConfig::group(){
	return 1;
}
int64_t ServiceConfig::id(){
	return 12;
}
const char* ServiceConfig::name(){
	return "ServiceConfig";
}
bool ServiceConfig::check(){
	//// check IdService 
	// check value
	//// check IdBegin 
	// check value
	//// check IdEnd 
	// check value
	//// check Algrithm 
	// check value
	//// check Path 
	// check value
	return true;
}
::core::Bytes* ServiceConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// IdService
	::core::ToBytes< int64_t >(bytes, m_IdService);
	/// IdBegin
	::core::ToBytes< int64_t >(bytes, m_IdBegin);
	/// IdEnd
	::core::ToBytes< int64_t >(bytes, m_IdEnd);
	/// Algrithm
	::core::ToBytes< int64_t >(bytes, m_Algrithm);
	/// Path
	::core::ToBytes< ::core::String* >(bytes, m_Path);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool ServiceConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// IdService
	if(just_init){
		_init_field_IdService();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdService)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceConfig from bytes");
			return false;
		}
	}
	/// IdBegin
	if(just_init){
		_init_field_IdBegin();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdBegin)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceConfig from bytes");
			return false;
		}
	}
	/// IdEnd
	if(just_init){
		_init_field_IdEnd();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_IdEnd)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceConfig from bytes");
			return false;
		}
	}
	/// Algrithm
	if(just_init){
		_init_field_Algrithm();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Algrithm)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceConfig from bytes");
			return false;
		}
	}
	/// Path
	if(just_init){
		_init_field_Path();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Path)){ clean(); return false; }
		RETAIN_POINTER(m_Path);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool ServiceConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// IdService
	::core::ToLua< int64_t >(L, m_IdService);
	lua_setfield(L, -2, "IdService");
	// IdBegin
	::core::ToLua< int64_t >(L, m_IdBegin);
	lua_setfield(L, -2, "IdBegin");
	// IdEnd
	::core::ToLua< int64_t >(L, m_IdEnd);
	lua_setfield(L, -2, "IdEnd");
	// Algrithm
	::core::ToLua< int64_t >(L, m_Algrithm);
	lua_setfield(L, -2, "Algrithm");
	// Path
	::core::ToLua< ::core::String* >(L, m_Path);
	lua_setfield(L, -2, "Path");
	return true;
}
bool ServiceConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// IdService
	lua_getfield(L, idx, "IdService");
	if(lua_isnil(L, -1)){
		_init_field_IdService();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdService)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// IdBegin
	lua_getfield(L, idx, "IdBegin");
	if(lua_isnil(L, -1)){
		_init_field_IdBegin();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdBegin)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// IdEnd
	lua_getfield(L, idx, "IdEnd");
	if(lua_isnil(L, -1)){
		_init_field_IdEnd();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_IdEnd)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Algrithm
	lua_getfield(L, idx, "Algrithm");
	if(lua_isnil(L, -1)){
		_init_field_Algrithm();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Algrithm)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Path
	lua_getfield(L, idx, "Path");
	if(lua_isnil(L, -1)){
		_init_field_Path();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Path)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Path);
	}
	lua_settop(L, old_top);
	return check();
}
bool ServiceConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// IdService
	str->appendString(::core::String::Format("%sIdService =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdService)->c_str()));
	// IdBegin
	str->appendString(::core::String::Format("%sIdBegin =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdBegin)->c_str()));
	// IdEnd
	str->appendString(::core::String::Format("%sIdEnd =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_IdEnd)->c_str()));
	// Algrithm
	str->appendString(::core::String::Format("%sAlgrithm =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Algrithm)->c_str()));
	// Path
	str->appendString(::core::String::Format("%sPath =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Path)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(ServiceConfig, int64_t, IdService)
void ServiceConfig::_init_field_IdService(){
	m_IdService =0;
}
DEFINE_PROPERTY(ServiceConfig, int64_t, IdBegin)
void ServiceConfig::_init_field_IdBegin(){
	m_IdBegin =0;
}
DEFINE_PROPERTY(ServiceConfig, int64_t, IdEnd)
void ServiceConfig::_init_field_IdEnd(){
	m_IdEnd =0;
}
DEFINE_PROPERTY(ServiceConfig, int64_t, Algrithm)
void ServiceConfig::_init_field_Algrithm(){
	m_Algrithm =1;
}
DEFINE_PROPERTY_P(ServiceConfig, ::core::String*, Path)
void ServiceConfig::_init_field_Path(){
	m_Path =::core::String::NewString("");
	RETAIN_POINTER(m_Path);
}
/** class ServiceManagerConfig **/
/** impl ServiceManagerConfig **/
ServiceManagerConfig::ServiceManagerConfig(){
}
ServiceManagerConfig::~ServiceManagerConfig(){
}
void ServiceManagerConfig::init(){
	Super::init();
	_init_field_Service();
}
void ServiceManagerConfig::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* ServiceManagerConfig::Creator(){
	return ::core::SafeNew<ServiceManagerConfig>();
}
void ServiceManagerConfig::clean(){
	// clean Service 
	CLEAN_POINTER(m_Service);
}
int64_t ServiceManagerConfig::group(){
	return 1;
}
int64_t ServiceManagerConfig::id(){
	return 13;
}
const char* ServiceManagerConfig::name(){
	return "ServiceManagerConfig";
}
bool ServiceManagerConfig::check(){
	//// check Service 
	// check value
	const int64_t n_so_Service =(m_Service ? m_Service->size() : 0);
	for(int64_t i=0; i<n_so_Service; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Service->get(i))){
			if(o->check() == false){
				ERROR("protocol ServiceManagerConfig check field, check the %lldth Service error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* ServiceManagerConfig::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Service
	const int64_t Service_cnt =(m_Service ? (m_Service->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Service_cnt);
	for(int64_t i=0; i<Service_cnt; ++i){
		ServiceConfig* item =dynamic_cast< ServiceConfig* >(m_Service->get(i));
		::core::ToBytes< ServiceConfig* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool ServiceManagerConfig::fromBytes(::core::Bytes* bytes){
	clean();
	if(!bytes){ return false; }
	const int64_t read_cursor_beg =bytes->getReadCursor();
	int64_t read_cursor_cur =0;
	int64_t read_cnt =0;
	packet_size_t length =0;
	if(!bytes->read(&length, sizeof(length))){ return false; }
	length =net_to_host_packet_size(length);
	if(0 == length){ return true; }
	bool just_init =false;
	/// Service
	if(just_init){
		_init_field_Service();
	}
	else{
		if(m_Service == 0){
			m_Service =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Service);
		}
		int64_t Service_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Service_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Service_cnt; ++i){
			ServiceConfig* item =0;
			if(!::core::FromBytes< ServiceConfig* >(bytes, item)){ clean(); return false; }
			m_Service->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal ServiceManagerConfig from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool ServiceManagerConfig::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Service
	const int64_t Service_cnt =(m_Service ? (m_Service->size()) : 0);
	lua_createtable(L, (int)Service_cnt, 0);
	for(int64_t i=0; i<Service_cnt; ++i){
		ServiceConfig* item =dynamic_cast< ServiceConfig* >(m_Service->get(i));
		::core::ToLua< ServiceConfig* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Service");
	return true;
}
bool ServiceManagerConfig::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Service
	lua_getfield(L, idx, "Service");
	if(lua_isnil(L, -1)){
		_init_field_Service();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Service_cnt =lua_objlen(L, -1);
			if(m_Service == 0){
				m_Service =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Service);
			}
			for(int64_t i=0; i<Service_cnt; ++i){
				ServiceConfig* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< ServiceConfig* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Service->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool ServiceManagerConfig::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Service
	str->appendString(::core::String::Format("%sService ={\n", tab(tab_count+1)));
	const int64_t Service_cnt =(m_Service ? (m_Service->size()) : 0);
	for(int64_t i=0; i<Service_cnt; ++i){
		if(ServiceConfig* item =dynamic_cast< ServiceConfig* >(m_Service->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(ServiceManagerConfig, ::core::Array*, ServiceConfig*, Service)
void ServiceManagerConfig::_init_field_Service(){
	m_Service =0;
}
}
}
