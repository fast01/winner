#include "protocol.h"

namespace protocol{
/*** register ***/
static PFN_PROTOCOL_CREATOR g_protocol_creator[PROTOCOL_MAX_ID];
bool register_protocol(){
	// Need't set as zero, because g_protocol_creator will be set zero when program started, but i have compulsion!!!
	memset(g_protocol_creator, 0, sizeof(g_protocol_creator));
	g_protocol_creator[1001 - 1] =S2FirstRequest::Creator; 
	g_protocol_creator[1002 - 1] =S2FirstRespond::Creator; 
	g_protocol_creator[1003 - 1] =S2SecondRequest::Creator; 
	g_protocol_creator[1004 - 1] =S2SecondRespond::Creator; 
	g_protocol_creator[1005 - 1] =S3FirstRequest::Creator; 
	g_protocol_creator[1006 - 1] =S3FirstRespond::Creator; 
	g_protocol_creator[1007 - 1] =S3SecondRequest::Creator; 
	g_protocol_creator[1008 - 1] =S3SecondRespond::Creator; 
	g_protocol_creator[1009 - 1] =S5FirstRequest::Creator; 
	g_protocol_creator[1010 - 1] =S5FirstRespond::Creator; 
	g_protocol_creator[1011 - 1] =S5SecondRequest::Creator; 
	g_protocol_creator[1012 - 1] =S5SecondRespond::Creator; 
	g_protocol_creator[1013 - 1] =S4FirstRequest::Creator; 
	g_protocol_creator[1014 - 1] =S4FirstRespond::Creator; 
	g_protocol_creator[1015 - 1] =S4SecondRequest::Creator; 
	g_protocol_creator[1016 - 1] =S4SecondRespond::Creator; 
	g_protocol_creator[1017 - 1] =Family::Creator; 
	g_protocol_creator[1018 - 1] =Game::Creator; 
	g_protocol_creator[1019 - 1] =Human::Creator; 
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
/** class S2FirstRequest **/
/** impl S2FirstRequest **/
S2FirstRequest::S2FirstRequest(){
}
S2FirstRequest::~S2FirstRequest(){
}
void S2FirstRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S2FirstRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S2FirstRequest::Creator(){
	return ::core::SafeNew<S2FirstRequest>();
}
void S2FirstRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S2FirstRequest::group(){
	return 17;
}
int64_t S2FirstRequest::id(){
	return 1001;
}
const char* S2FirstRequest::name(){
	return "S2FirstRequest";
}
bool S2FirstRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S2FirstRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S2FirstRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S2FirstRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S2FirstRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S2FirstRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S2FirstRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S2FirstRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S2FirstRequest, int64_t, Param1)
void S2FirstRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S2FirstRequest, boolean_t, Param2)
void S2FirstRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S2FirstRequest, ::core::String*, Param3)
void S2FirstRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S2FirstRespond **/
/** impl S2FirstRespond **/
S2FirstRespond::S2FirstRespond(){
}
S2FirstRespond::~S2FirstRespond(){
}
void S2FirstRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S2FirstRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S2FirstRespond::Creator(){
	return ::core::SafeNew<S2FirstRespond>();
}
void S2FirstRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S2FirstRespond::group(){
	return 17;
}
int64_t S2FirstRespond::id(){
	return 1002;
}
const char* S2FirstRespond::name(){
	return "S2FirstRespond";
}
bool S2FirstRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S2FirstRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S2FirstRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2FirstRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S2FirstRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S2FirstRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S2FirstRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S2FirstRespond, int64_t, Error)
void S2FirstRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S2FirstRespond, int64_t, Result1)
void S2FirstRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S2FirstRespond, boolean_t, Result2)
void S2FirstRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S2FirstRespond, ::core::String*, Result3)
void S2FirstRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S2SecondRequest **/
/** impl S2SecondRequest **/
S2SecondRequest::S2SecondRequest(){
}
S2SecondRequest::~S2SecondRequest(){
}
void S2SecondRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S2SecondRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S2SecondRequest::Creator(){
	return ::core::SafeNew<S2SecondRequest>();
}
void S2SecondRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S2SecondRequest::group(){
	return 17;
}
int64_t S2SecondRequest::id(){
	return 1003;
}
const char* S2SecondRequest::name(){
	return "S2SecondRequest";
}
bool S2SecondRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S2SecondRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S2SecondRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S2SecondRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S2SecondRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S2SecondRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S2SecondRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S2SecondRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S2SecondRequest, int64_t, Param1)
void S2SecondRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S2SecondRequest, boolean_t, Param2)
void S2SecondRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S2SecondRequest, ::core::String*, Param3)
void S2SecondRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S2SecondRespond **/
/** impl S2SecondRespond **/
S2SecondRespond::S2SecondRespond(){
}
S2SecondRespond::~S2SecondRespond(){
}
void S2SecondRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S2SecondRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S2SecondRespond::Creator(){
	return ::core::SafeNew<S2SecondRespond>();
}
void S2SecondRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S2SecondRespond::group(){
	return 17;
}
int64_t S2SecondRespond::id(){
	return 1004;
}
const char* S2SecondRespond::name(){
	return "S2SecondRespond";
}
bool S2SecondRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S2SecondRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S2SecondRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S2SecondRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S2SecondRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S2SecondRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S2SecondRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S2SecondRespond, int64_t, Error)
void S2SecondRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S2SecondRespond, int64_t, Result1)
void S2SecondRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S2SecondRespond, boolean_t, Result2)
void S2SecondRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S2SecondRespond, ::core::String*, Result3)
void S2SecondRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S3FirstRequest **/
/** impl S3FirstRequest **/
S3FirstRequest::S3FirstRequest(){
}
S3FirstRequest::~S3FirstRequest(){
}
void S3FirstRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S3FirstRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S3FirstRequest::Creator(){
	return ::core::SafeNew<S3FirstRequest>();
}
void S3FirstRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S3FirstRequest::group(){
	return 17;
}
int64_t S3FirstRequest::id(){
	return 1005;
}
const char* S3FirstRequest::name(){
	return "S3FirstRequest";
}
bool S3FirstRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S3FirstRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S3FirstRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S3FirstRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S3FirstRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S3FirstRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S3FirstRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S3FirstRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S3FirstRequest, int64_t, Param1)
void S3FirstRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S3FirstRequest, boolean_t, Param2)
void S3FirstRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S3FirstRequest, ::core::String*, Param3)
void S3FirstRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S3FirstRespond **/
/** impl S3FirstRespond **/
S3FirstRespond::S3FirstRespond(){
}
S3FirstRespond::~S3FirstRespond(){
}
void S3FirstRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S3FirstRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S3FirstRespond::Creator(){
	return ::core::SafeNew<S3FirstRespond>();
}
void S3FirstRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S3FirstRespond::group(){
	return 17;
}
int64_t S3FirstRespond::id(){
	return 1006;
}
const char* S3FirstRespond::name(){
	return "S3FirstRespond";
}
bool S3FirstRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S3FirstRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S3FirstRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3FirstRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S3FirstRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S3FirstRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S3FirstRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S3FirstRespond, int64_t, Error)
void S3FirstRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S3FirstRespond, int64_t, Result1)
void S3FirstRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S3FirstRespond, boolean_t, Result2)
void S3FirstRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S3FirstRespond, ::core::String*, Result3)
void S3FirstRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S3SecondRequest **/
/** impl S3SecondRequest **/
S3SecondRequest::S3SecondRequest(){
}
S3SecondRequest::~S3SecondRequest(){
}
void S3SecondRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S3SecondRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S3SecondRequest::Creator(){
	return ::core::SafeNew<S3SecondRequest>();
}
void S3SecondRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S3SecondRequest::group(){
	return 17;
}
int64_t S3SecondRequest::id(){
	return 1007;
}
const char* S3SecondRequest::name(){
	return "S3SecondRequest";
}
bool S3SecondRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S3SecondRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S3SecondRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S3SecondRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S3SecondRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S3SecondRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S3SecondRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S3SecondRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S3SecondRequest, int64_t, Param1)
void S3SecondRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S3SecondRequest, boolean_t, Param2)
void S3SecondRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S3SecondRequest, ::core::String*, Param3)
void S3SecondRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S3SecondRespond **/
/** impl S3SecondRespond **/
S3SecondRespond::S3SecondRespond(){
}
S3SecondRespond::~S3SecondRespond(){
}
void S3SecondRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S3SecondRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S3SecondRespond::Creator(){
	return ::core::SafeNew<S3SecondRespond>();
}
void S3SecondRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S3SecondRespond::group(){
	return 17;
}
int64_t S3SecondRespond::id(){
	return 1008;
}
const char* S3SecondRespond::name(){
	return "S3SecondRespond";
}
bool S3SecondRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S3SecondRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S3SecondRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S3SecondRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S3SecondRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S3SecondRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S3SecondRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S3SecondRespond, int64_t, Error)
void S3SecondRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S3SecondRespond, int64_t, Result1)
void S3SecondRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S3SecondRespond, boolean_t, Result2)
void S3SecondRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S3SecondRespond, ::core::String*, Result3)
void S3SecondRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S5FirstRequest **/
/** impl S5FirstRequest **/
S5FirstRequest::S5FirstRequest(){
}
S5FirstRequest::~S5FirstRequest(){
}
void S5FirstRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S5FirstRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S5FirstRequest::Creator(){
	return ::core::SafeNew<S5FirstRequest>();
}
void S5FirstRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S5FirstRequest::group(){
	return 17;
}
int64_t S5FirstRequest::id(){
	return 1009;
}
const char* S5FirstRequest::name(){
	return "S5FirstRequest";
}
bool S5FirstRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S5FirstRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S5FirstRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S5FirstRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S5FirstRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S5FirstRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S5FirstRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S5FirstRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S5FirstRequest, int64_t, Param1)
void S5FirstRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S5FirstRequest, boolean_t, Param2)
void S5FirstRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S5FirstRequest, ::core::String*, Param3)
void S5FirstRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S5FirstRespond **/
/** impl S5FirstRespond **/
S5FirstRespond::S5FirstRespond(){
}
S5FirstRespond::~S5FirstRespond(){
}
void S5FirstRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S5FirstRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S5FirstRespond::Creator(){
	return ::core::SafeNew<S5FirstRespond>();
}
void S5FirstRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S5FirstRespond::group(){
	return 17;
}
int64_t S5FirstRespond::id(){
	return 1010;
}
const char* S5FirstRespond::name(){
	return "S5FirstRespond";
}
bool S5FirstRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S5FirstRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S5FirstRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5FirstRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S5FirstRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S5FirstRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S5FirstRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S5FirstRespond, int64_t, Error)
void S5FirstRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S5FirstRespond, int64_t, Result1)
void S5FirstRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S5FirstRespond, boolean_t, Result2)
void S5FirstRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S5FirstRespond, ::core::String*, Result3)
void S5FirstRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S5SecondRequest **/
/** impl S5SecondRequest **/
S5SecondRequest::S5SecondRequest(){
}
S5SecondRequest::~S5SecondRequest(){
}
void S5SecondRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S5SecondRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S5SecondRequest::Creator(){
	return ::core::SafeNew<S5SecondRequest>();
}
void S5SecondRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S5SecondRequest::group(){
	return 17;
}
int64_t S5SecondRequest::id(){
	return 1011;
}
const char* S5SecondRequest::name(){
	return "S5SecondRequest";
}
bool S5SecondRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S5SecondRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S5SecondRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S5SecondRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S5SecondRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S5SecondRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S5SecondRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S5SecondRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S5SecondRequest, int64_t, Param1)
void S5SecondRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S5SecondRequest, boolean_t, Param2)
void S5SecondRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S5SecondRequest, ::core::String*, Param3)
void S5SecondRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S5SecondRespond **/
/** impl S5SecondRespond **/
S5SecondRespond::S5SecondRespond(){
}
S5SecondRespond::~S5SecondRespond(){
}
void S5SecondRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S5SecondRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S5SecondRespond::Creator(){
	return ::core::SafeNew<S5SecondRespond>();
}
void S5SecondRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S5SecondRespond::group(){
	return 17;
}
int64_t S5SecondRespond::id(){
	return 1012;
}
const char* S5SecondRespond::name(){
	return "S5SecondRespond";
}
bool S5SecondRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S5SecondRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S5SecondRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S5SecondRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S5SecondRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S5SecondRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S5SecondRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S5SecondRespond, int64_t, Error)
void S5SecondRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S5SecondRespond, int64_t, Result1)
void S5SecondRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S5SecondRespond, boolean_t, Result2)
void S5SecondRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S5SecondRespond, ::core::String*, Result3)
void S5SecondRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S4FirstRequest **/
/** impl S4FirstRequest **/
S4FirstRequest::S4FirstRequest(){
}
S4FirstRequest::~S4FirstRequest(){
}
void S4FirstRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S4FirstRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S4FirstRequest::Creator(){
	return ::core::SafeNew<S4FirstRequest>();
}
void S4FirstRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S4FirstRequest::group(){
	return 17;
}
int64_t S4FirstRequest::id(){
	return 1013;
}
const char* S4FirstRequest::name(){
	return "S4FirstRequest";
}
bool S4FirstRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S4FirstRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S4FirstRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S4FirstRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S4FirstRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S4FirstRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S4FirstRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S4FirstRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S4FirstRequest, int64_t, Param1)
void S4FirstRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S4FirstRequest, boolean_t, Param2)
void S4FirstRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S4FirstRequest, ::core::String*, Param3)
void S4FirstRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S4FirstRespond **/
/** impl S4FirstRespond **/
S4FirstRespond::S4FirstRespond(){
}
S4FirstRespond::~S4FirstRespond(){
}
void S4FirstRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S4FirstRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S4FirstRespond::Creator(){
	return ::core::SafeNew<S4FirstRespond>();
}
void S4FirstRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S4FirstRespond::group(){
	return 17;
}
int64_t S4FirstRespond::id(){
	return 1014;
}
const char* S4FirstRespond::name(){
	return "S4FirstRespond";
}
bool S4FirstRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S4FirstRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S4FirstRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4FirstRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S4FirstRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S4FirstRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S4FirstRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S4FirstRespond, int64_t, Error)
void S4FirstRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S4FirstRespond, int64_t, Result1)
void S4FirstRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S4FirstRespond, boolean_t, Result2)
void S4FirstRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S4FirstRespond, ::core::String*, Result3)
void S4FirstRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class S4SecondRequest **/
/** impl S4SecondRequest **/
S4SecondRequest::S4SecondRequest(){
}
S4SecondRequest::~S4SecondRequest(){
}
void S4SecondRequest::init(){
	Super::init();
	_init_field_Param1();
	_init_field_Param2();
	_init_field_Param3();
}
void S4SecondRequest::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S4SecondRequest::Creator(){
	return ::core::SafeNew<S4SecondRequest>();
}
void S4SecondRequest::clean(){
	// clean Param1 
	m_Param1 =0;
	// clean Param2 
	m_Param2 =false;
	// clean Param3 
	CLEAN_POINTER(m_Param3);
}
int64_t S4SecondRequest::group(){
	return 17;
}
int64_t S4SecondRequest::id(){
	return 1015;
}
const char* S4SecondRequest::name(){
	return "S4SecondRequest";
}
bool S4SecondRequest::check(){
	//// check Param1 
	// check value
	//// check Param2 
	// check value
	//// check Param3 
	// check value
	if(::core::String::Length(m_Param3) < 1){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S4SecondRequest check field, length(Param3) < 1");
		return false;
	}
	if(::core::String::Length(m_Param3) > 128){
		ERROR("%s %lld", m_Param3?m_Param3->c_str():"null", ::core::String::Length(m_Param3));
		ERROR("protocol S4SecondRequest check field, length(Param3) > 128");
		return false;
	}
	return true;
}
::core::Bytes* S4SecondRequest::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Param1
	::core::ToBytes< int64_t >(bytes, m_Param1);
	/// Param2
	::core::ToBytes< boolean_t >(bytes, m_Param2);
	/// Param3
	::core::ToBytes< ::core::String* >(bytes, m_Param3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S4SecondRequest::fromBytes(::core::Bytes* bytes){
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
	/// Param1
	if(just_init){
		_init_field_Param1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Param1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRequest from bytes");
			return false;
		}
	}
	/// Param2
	if(just_init){
		_init_field_Param2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Param2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRequest from bytes");
			return false;
		}
	}
	/// Param3
	if(just_init){
		_init_field_Param3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Param3)){ clean(); return false; }
		RETAIN_POINTER(m_Param3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRequest from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S4SecondRequest::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Param1
	::core::ToLua< int64_t >(L, m_Param1);
	lua_setfield(L, -2, "Param1");
	// Param2
	::core::ToLua< boolean_t >(L, m_Param2);
	lua_setfield(L, -2, "Param2");
	// Param3
	::core::ToLua< ::core::String* >(L, m_Param3);
	lua_setfield(L, -2, "Param3");
	return true;
}
bool S4SecondRequest::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Param1
	lua_getfield(L, idx, "Param1");
	if(lua_isnil(L, -1)){
		_init_field_Param1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Param1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param2
	lua_getfield(L, idx, "Param2");
	if(lua_isnil(L, -1)){
		_init_field_Param2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Param2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Param3
	lua_getfield(L, idx, "Param3");
	if(lua_isnil(L, -1)){
		_init_field_Param3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Param3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Param3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S4SecondRequest::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Param1
	str->appendString(::core::String::Format("%sParam1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Param1)->c_str()));
	// Param2
	str->appendString(::core::String::Format("%sParam2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Param2)->c_str()));
	// Param3
	str->appendString(::core::String::Format("%sParam3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Param3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S4SecondRequest, int64_t, Param1)
void S4SecondRequest::_init_field_Param1(){
	m_Param1 =0;
}
DEFINE_PROPERTY(S4SecondRequest, boolean_t, Param2)
void S4SecondRequest::_init_field_Param2(){
	m_Param2 =false;
}
DEFINE_PROPERTY_P(S4SecondRequest, ::core::String*, Param3)
void S4SecondRequest::_init_field_Param3(){
	m_Param3 =::core::String::NewString("");
	RETAIN_POINTER(m_Param3);
}
/** class S4SecondRespond **/
/** impl S4SecondRespond **/
S4SecondRespond::S4SecondRespond(){
}
S4SecondRespond::~S4SecondRespond(){
}
void S4SecondRespond::init(){
	Super::init();
	_init_field_Error();
	_init_field_Result1();
	_init_field_Result2();
	_init_field_Result3();
}
void S4SecondRespond::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* S4SecondRespond::Creator(){
	return ::core::SafeNew<S4SecondRespond>();
}
void S4SecondRespond::clean(){
	// clean Error 
	m_Error =0;
	// clean Result1 
	m_Result1 =0;
	// clean Result2 
	m_Result2 =false;
	// clean Result3 
	CLEAN_POINTER(m_Result3);
}
int64_t S4SecondRespond::group(){
	return 17;
}
int64_t S4SecondRespond::id(){
	return 1016;
}
const char* S4SecondRespond::name(){
	return "S4SecondRespond";
}
bool S4SecondRespond::check(){
	//// check Error 
	// check value
	//// check Result1 
	// check value
	//// check Result2 
	// check value
	//// check Result3 
	// check value
	return true;
}
::core::Bytes* S4SecondRespond::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Error
	::core::ToBytes< int64_t >(bytes, m_Error);
	/// Result1
	::core::ToBytes< int64_t >(bytes, m_Result1);
	/// Result2
	::core::ToBytes< boolean_t >(bytes, m_Result2);
	/// Result3
	::core::ToBytes< ::core::String* >(bytes, m_Result3);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool S4SecondRespond::fromBytes(::core::Bytes* bytes){
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
	/// Error
	if(just_init){
		_init_field_Error();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Error)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRespond from bytes");
			return false;
		}
	}
	/// Result1
	if(just_init){
		_init_field_Result1();
	}
	else{
		if(!::core::FromBytes< int64_t >(bytes, m_Result1)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRespond from bytes");
			return false;
		}
	}
	/// Result2
	if(just_init){
		_init_field_Result2();
	}
	else{
		if(!::core::FromBytes< boolean_t >(bytes, m_Result2)){ clean(); return false; }
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRespond from bytes");
			return false;
		}
	}
	/// Result3
	if(just_init){
		_init_field_Result3();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Result3)){ clean(); return false; }
		RETAIN_POINTER(m_Result3);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal S4SecondRespond from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool S4SecondRespond::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Error
	::core::ToLua< int64_t >(L, m_Error);
	lua_setfield(L, -2, "Error");
	// Result1
	::core::ToLua< int64_t >(L, m_Result1);
	lua_setfield(L, -2, "Result1");
	// Result2
	::core::ToLua< boolean_t >(L, m_Result2);
	lua_setfield(L, -2, "Result2");
	// Result3
	::core::ToLua< ::core::String* >(L, m_Result3);
	lua_setfield(L, -2, "Result3");
	return true;
}
bool S4SecondRespond::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Error
	lua_getfield(L, idx, "Error");
	if(lua_isnil(L, -1)){
		_init_field_Error();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Error)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result1
	lua_getfield(L, idx, "Result1");
	if(lua_isnil(L, -1)){
		_init_field_Result1();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Result1)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result2
	lua_getfield(L, idx, "Result2");
	if(lua_isnil(L, -1)){
		_init_field_Result2();
	}
	else{
		if(!::core::FromLua< boolean_t >(L, -1, m_Result2)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Result3
	lua_getfield(L, idx, "Result3");
	if(lua_isnil(L, -1)){
		_init_field_Result3();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Result3)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Result3);
	}
	lua_settop(L, old_top);
	return check();
}
bool S4SecondRespond::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Error
	str->appendString(::core::String::Format("%sError =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Error)->c_str()));
	// Result1
	str->appendString(::core::String::Format("%sResult1 =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Result1)->c_str()));
	// Result2
	str->appendString(::core::String::Format("%sResult2 =%s,\n", tab(tab_count+1), ::core::ToString< boolean_t >(m_Result2)->c_str()));
	// Result3
	str->appendString(::core::String::Format("%sResult3 =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Result3)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(S4SecondRespond, int64_t, Error)
void S4SecondRespond::_init_field_Error(){
	m_Error =0;
}
DEFINE_PROPERTY(S4SecondRespond, int64_t, Result1)
void S4SecondRespond::_init_field_Result1(){
	m_Result1 =0;
}
DEFINE_PROPERTY(S4SecondRespond, boolean_t, Result2)
void S4SecondRespond::_init_field_Result2(){
	m_Result2 =false;
}
DEFINE_PROPERTY_P(S4SecondRespond, ::core::String*, Result3)
void S4SecondRespond::_init_field_Result3(){
	m_Result3 =::core::String::NewString("");
	RETAIN_POINTER(m_Result3);
}
/** class Family **/
/** impl Family **/
Family::Family(){
}
Family::~Family(){
}
void Family::init(){
	Super::init();
	_init_field_Father();
	_init_field_Mother();
	_init_field_Children();
}
void Family::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* Family::Creator(){
	return ::core::SafeNew<Family>();
}
void Family::clean(){
	// clean Father 
	CLEAN_POINTER(m_Father);
	// clean Mother 
	CLEAN_POINTER(m_Mother);
	// clean Children 
	CLEAN_POINTER(m_Children);
}
int64_t Family::group(){
	return 17;
}
int64_t Family::id(){
	return 1017;
}
const char* Family::name(){
	return "Family";
}
bool Family::check(){
	//// check Father 
	// check value
	//// check Mother 
	// check value
	//// check Children 
	// check value
	const int64_t n_so_Children =(m_Children ? m_Children->size() : 0);
	for(int64_t i=0; i<n_so_Children; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Children->get(i))){
			if(o->check() == false){
				ERROR("protocol Family check field, check the %lldth Children error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* Family::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Father
	::core::ToBytes< Human* >(bytes, m_Father);
	/// Mother
	::core::ToBytes< Human* >(bytes, m_Mother);
	/// Children
	const int64_t Children_cnt =(m_Children ? (m_Children->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Children_cnt);
	for(int64_t i=0; i<Children_cnt; ++i){
		Human* item =dynamic_cast< Human* >(m_Children->get(i));
		::core::ToBytes< Human* >(bytes, item);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool Family::fromBytes(::core::Bytes* bytes){
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
	/// Father
	if(just_init){
		_init_field_Father();
	}
	else{
		if(!::core::FromBytes< Human* >(bytes, m_Father)){ clean(); return false; }
		RETAIN_POINTER(m_Father);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Family from bytes");
			return false;
		}
	}
	/// Mother
	if(just_init){
		_init_field_Mother();
	}
	else{
		if(!::core::FromBytes< Human* >(bytes, m_Mother)){ clean(); return false; }
		RETAIN_POINTER(m_Mother);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Family from bytes");
			return false;
		}
	}
	/// Children
	if(just_init){
		_init_field_Children();
	}
	else{
		if(m_Children == 0){
			m_Children =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Children);
		}
		int64_t Children_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Children_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Children_cnt; ++i){
			Human* item =0;
			if(!::core::FromBytes< Human* >(bytes, item)){ clean(); return false; }
			m_Children->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Family from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool Family::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Father
	::core::ToLua< Human* >(L, m_Father);
	lua_setfield(L, -2, "Father");
	// Mother
	::core::ToLua< Human* >(L, m_Mother);
	lua_setfield(L, -2, "Mother");
	// Children
	const int64_t Children_cnt =(m_Children ? (m_Children->size()) : 0);
	lua_createtable(L, (int)Children_cnt, 0);
	for(int64_t i=0; i<Children_cnt; ++i){
		Human* item =dynamic_cast< Human* >(m_Children->get(i));
		::core::ToLua< Human* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Children");
	return true;
}
bool Family::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Father
	lua_getfield(L, idx, "Father");
	if(lua_isnil(L, -1)){
		_init_field_Father();
	}
	else{
		if(!::core::FromLua< Human* >(L, -1, m_Father)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Father);
	}
	lua_settop(L, old_top);
	// Mother
	lua_getfield(L, idx, "Mother");
	if(lua_isnil(L, -1)){
		_init_field_Mother();
	}
	else{
		if(!::core::FromLua< Human* >(L, -1, m_Mother)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Mother);
	}
	lua_settop(L, old_top);
	// Children
	lua_getfield(L, idx, "Children");
	if(lua_isnil(L, -1)){
		_init_field_Children();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Children_cnt =lua_objlen(L, -1);
			if(m_Children == 0){
				m_Children =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Children);
			}
			for(int64_t i=0; i<Children_cnt; ++i){
				Human* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< Human* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Children->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool Family::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Father
	if(m_Father){
		str->appendString(::core::String::Format("%sFather =\n", tab(tab_count+1)));
		m_Father->appendToString(str, tab_count + 1);
	}
	// Mother
	if(m_Mother){
		str->appendString(::core::String::Format("%sMother =\n", tab(tab_count+1)));
		m_Mother->appendToString(str, tab_count + 1);
	}
	// Children
	str->appendString(::core::String::Format("%sChildren ={\n", tab(tab_count+1)));
	const int64_t Children_cnt =(m_Children ? (m_Children->size()) : 0);
	for(int64_t i=0; i<Children_cnt; ++i){
		if(Human* item =dynamic_cast< Human* >(m_Children->get(i))){
			item->appendToString(str, tab_count + 2);
		}
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY_P(Family, Human*, Father)
void Family::_init_field_Father(){
	m_Father =0;
}
DEFINE_PROPERTY_P(Family, Human*, Mother)
void Family::_init_field_Mother(){
	m_Mother =0;
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(Family, ::core::Array*, Human*, Children)
void Family::_init_field_Children(){
	m_Children =0;
}
/** class Game **/
/** impl Game **/
Game::Game(){
}
Game::~Game(){
}
void Game::init(){
	Super::init();
	_init_field_Id();
	_init_field_Name();
	_init_field_Desc();
}
void Game::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* Game::Creator(){
	return ::core::SafeNew<Game>();
}
void Game::clean(){
	// clean Id 
	m_Id =0;
	// clean Name 
	CLEAN_POINTER(m_Name);
	// clean Desc 
	CLEAN_POINTER(m_Desc);
}
int64_t Game::group(){
	return 17;
}
int64_t Game::id(){
	return 1018;
}
const char* Game::name(){
	return "Game";
}
bool Game::check(){
	//// check Id 
	// check value
	if(m_Id < 0){
		ERROR("protocol Game check field, Id < 0");
		return false;
	}
	if(m_Id > 1000000){
		ERROR("protocol Game check field, Id > 1000000");
		return false;
	}
	//// check Name 
	// check value
	if(::core::String::Length(m_Name) < 0){
		ERROR("%s %lld", m_Name?m_Name->c_str():"null", ::core::String::Length(m_Name));
		ERROR("protocol Game check field, length(Name) < 0");
		return false;
	}
	if(::core::String::Length(m_Name) > 100){
		ERROR("%s %lld", m_Name?m_Name->c_str():"null", ::core::String::Length(m_Name));
		ERROR("protocol Game check field, length(Name) > 100");
		return false;
	}
	//// check Desc 
	// check value
	return true;
}
::core::Bytes* Game::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Id
	::core::ToBytes< int64_t >(bytes, m_Id);
	/// Name
	::core::ToBytes< ::core::String* >(bytes, m_Name);
	/// Desc
	::core::ToBytes< ::core::String* >(bytes, m_Desc);
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool Game::fromBytes(::core::Bytes* bytes){
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
			ERROR("read too much bytes when unmashal Game from bytes");
			return false;
		}
	}
	/// Name
	if(just_init){
		_init_field_Name();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Name)){ clean(); return false; }
		RETAIN_POINTER(m_Name);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Game from bytes");
			return false;
		}
	}
	/// Desc
	if(just_init){
		_init_field_Desc();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Desc)){ clean(); return false; }
		RETAIN_POINTER(m_Desc);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Game from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool Game::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Id
	::core::ToLua< int64_t >(L, m_Id);
	lua_setfield(L, -2, "Id");
	// Name
	::core::ToLua< ::core::String* >(L, m_Name);
	lua_setfield(L, -2, "Name");
	// Desc
	::core::ToLua< ::core::String* >(L, m_Desc);
	lua_setfield(L, -2, "Desc");
	return true;
}
bool Game::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Id
	lua_getfield(L, idx, "Id");
	if(lua_isnil(L, -1)){
		_init_field_Id();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Id)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Name
	lua_getfield(L, idx, "Name");
	if(lua_isnil(L, -1)){
		_init_field_Name();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Name)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Name);
	}
	lua_settop(L, old_top);
	// Desc
	lua_getfield(L, idx, "Desc");
	if(lua_isnil(L, -1)){
		_init_field_Desc();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Desc)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Desc);
	}
	lua_settop(L, old_top);
	return check();
}
bool Game::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Id
	str->appendString(::core::String::Format("%sId =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Id)->c_str()));
	// Name
	str->appendString(::core::String::Format("%sName =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Name)->c_str()));
	// Desc
	str->appendString(::core::String::Format("%sDesc =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Desc)->c_str()));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(Game, int64_t, Id)
void Game::_init_field_Id(){
	m_Id =99;
}
DEFINE_PROPERTY_P(Game, ::core::String*, Name)
void Game::_init_field_Name(){
	m_Name =::core::String::NewString("fool");
	RETAIN_POINTER(m_Name);
}
DEFINE_PROPERTY_P(Game, ::core::String*, Desc)
void Game::_init_field_Desc(){
	m_Desc =::core::String::NewString("");
	RETAIN_POINTER(m_Desc);
}
/** class Human **/
/** impl Human **/
Human::Human(){
}
Human::~Human(){
}
void Human::init(){
	Super::init();
	_init_field_Id();
	_init_field_Name();
	_init_field_Desc();
	_init_field_Book();
	_init_field_Favorite();
}
void Human::finalize(){
	clean();
	Super::finalize();
}
::core::ProtocolBase* Human::Creator(){
	return ::core::SafeNew<Human>();
}
void Human::clean(){
	// clean Id 
	m_Id =0;
	// clean Name 
	CLEAN_POINTER(m_Name);
	// clean Desc 
	CLEAN_POINTER(m_Desc);
	// clean Book 
	CLEAN_POINTER(m_Book);
	// clean Favorite 
	for(int64_t i=0; i<3; ++i){
		CLEAN_POINTER(m_Favorite[i]);
	}
}
int64_t Human::group(){
	return 17;
}
int64_t Human::id(){
	return 1019;
}
const char* Human::name(){
	return "Human";
}
bool Human::check(){
	//// check Id 
	// check value
	//// check Name 
	// check value
	//// check Desc 
	// check value
	//// check Book 
	// check value
	const int64_t n_ll_Book =(m_Book ? m_Book->size() : 0);
	for(int64_t i=0; i<n_ll_Book; ++i){
		if(::core::String::Length(dynamic_cast< ::core::String* >(m_Book->get(i))) < 0){
			ERROR("protocol Human check field, length(Book[%lld]) < 0", (long long)i);
			return false;
		}
	}
	const int64_t n_hl_Book =(m_Book ? m_Book->size() : 0);
	for(int64_t i=0; i<n_hl_Book; ++i){
		if(::core::String::Length(dynamic_cast< ::core::String* >(m_Book->get(i))) > 100){
			ERROR("protocol Human check field, length(Book[%lld]) > 100", (long long)i);
			return false;
		}
	}
	//// check Favorite 
	// check value
	for(int64_t i=0; i<3; ++i){
		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_Favorite[i])){
			if(o->check() == false){
				ERROR("protocol Human check field, check the %lldth Favorite error", (long long)i);
				return false;
			}
		}
	}
	return true;
}
::core::Bytes* Human::toBytes(::core::Bytes* bytes){
	if(!check()){
		return 0;
	}
	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }
	const int64_t write_cursor_beg =bytes->getWriteCursor();
	packet_size_t length =0;;
	bytes->write(&length, sizeof(length));
	/// Id
	::core::ToBytes< int64_t >(bytes, m_Id);
	/// Name
	::core::ToBytes< ::core::String* >(bytes, m_Name);
	/// Desc
	::core::ToBytes< ::core::String* >(bytes, m_Desc);
	/// Book
	const int64_t Book_cnt =(m_Book ? (m_Book->size()) : 0);
	::core::ToBytes<int64_t>(bytes, Book_cnt);
	for(int64_t i=0; i<Book_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Book->get(i));
		::core::ToBytes< ::core::String* >(bytes, item);
	}
	/// Favorite
	for(int64_t i=0; i<3; ++i){
		::core::ToBytes< Game* >(bytes, m_Favorite[i]);
	}
	/// reset length
	const int64_t write_cursor_end =bytes->getWriteCursor();
	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);
	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;
	return bytes;
}
bool Human::fromBytes(::core::Bytes* bytes){
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
			ERROR("read too much bytes when unmashal Human from bytes");
			return false;
		}
	}
	/// Name
	if(just_init){
		_init_field_Name();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Name)){ clean(); return false; }
		RETAIN_POINTER(m_Name);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Human from bytes");
			return false;
		}
	}
	/// Desc
	if(just_init){
		_init_field_Desc();
	}
	else{
		if(!::core::FromBytes< ::core::String* >(bytes, m_Desc)){ clean(); return false; }
		RETAIN_POINTER(m_Desc);
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Human from bytes");
			return false;
		}
	}
	/// Book
	if(just_init){
		_init_field_Book();
	}
	else{
		if(m_Book == 0){
			m_Book =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
			RETAIN_POINTER(m_Book);
		}
		int64_t Book_cnt =0;
		if(!::core::FromBytes<int64_t>(bytes, Book_cnt)){ clean(); return false; }
		for(int64_t i=0; i<Book_cnt; ++i){
			::core::String* item =0;
			if(!::core::FromBytes< ::core::String* >(bytes, item)){ clean(); return false; }
			m_Book->push_back(item);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Human from bytes");
			return false;
		}
	}
	/// Favorite
	if(just_init){
		_init_field_Favorite();
	}
	else{
		for(int64_t i=0; i<3; ++i){
			if(!::core::FromBytes< Game* >(bytes, m_Favorite[i])){ clean(); return false; }
			RETAIN_POINTER(m_Favorite[i]);
		}
		// check read cursor
		read_cursor_cur =bytes->getReadCursor();
		read_cnt =read_cursor_cur - read_cursor_beg;
		if(read_cnt == (int64_t)length){
			just_init =true;
		}
		else if(read_cnt > (int64_t)length){
			ERROR("read too much bytes when unmashal Human from bytes");
			return false;
		}
	}
	/// skip remaind bytes
	if(read_cnt < (int64_t)length){
		bytes->skip(length - read_cnt);
	}
	return check();
}
bool Human::toLua(lua_State* L){
	if(!check()){
		return false;
	}
	lua_createtable(L, 0, 0);
	// Id
	::core::ToLua< int64_t >(L, m_Id);
	lua_setfield(L, -2, "Id");
	// Name
	::core::ToLua< ::core::String* >(L, m_Name);
	lua_setfield(L, -2, "Name");
	// Desc
	::core::ToLua< ::core::String* >(L, m_Desc);
	lua_setfield(L, -2, "Desc");
	// Book
	const int64_t Book_cnt =(m_Book ? (m_Book->size()) : 0);
	lua_createtable(L, (int)Book_cnt, 0);
	for(int64_t i=0; i<Book_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Book->get(i));
		::core::ToLua< ::core::String* >(L, item);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Book");
	// Favorite
	lua_createtable(L, 3, 0);
	for(int64_t i=0; i<3; ++i){
		::core::ToLua< Game* >(L, m_Favorite[i]);
		lua_rawseti(L, -2, i+1);
	}
	lua_setfield(L, -2, "Favorite");
	return true;
}
bool Human::fromLua(lua_State* L, const int64_t idx){
	clean();
	::core::LuaTopHelper lth(L);
	const int old_top =lua_gettop(L);
	if(old_top <= 0){ return false; }
	if(!lua_istable(L, idx)){ return false; }
	// Id
	lua_getfield(L, idx, "Id");
	if(lua_isnil(L, -1)){
		_init_field_Id();
	}
	else{
		if(!::core::FromLua< int64_t >(L, -1, m_Id)){ clean(); lua_settop(L, old_top); return false; }
	}
	lua_settop(L, old_top);
	// Name
	lua_getfield(L, idx, "Name");
	if(lua_isnil(L, -1)){
		_init_field_Name();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Name)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Name);
	}
	lua_settop(L, old_top);
	// Desc
	lua_getfield(L, idx, "Desc");
	if(lua_isnil(L, -1)){
		_init_field_Desc();
	}
	else{
		if(!::core::FromLua< ::core::String* >(L, -1, m_Desc)){ clean(); lua_settop(L, old_top); return false; }
		RETAIN_POINTER(m_Desc);
	}
	lua_settop(L, old_top);
	// Book
	lua_getfield(L, idx, "Book");
	if(lua_isnil(L, -1)){
		_init_field_Book();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			const int64_t Book_cnt =lua_objlen(L, -1);
			if(m_Book == 0){
				m_Book =::core::SafeNew< std::remove_pointer< ::core::Array* >::type >();
				RETAIN_POINTER(m_Book);
			}
			for(int64_t i=0; i<Book_cnt; ++i){
				::core::String* item =0;
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< ::core::String* >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				m_Book->push_back(item);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	// Favorite
	lua_getfield(L, idx, "Favorite");
	if(lua_isnil(L, -1)){
		_init_field_Favorite();
	}
	else if(lua_istable(L, -1)){
		if(lua_istable(L, -1)){
			for(int64_t i=0; i<3; ++i){
				lua_rawgeti(L, -1, i+1);
				if(!::core::FromLua< Game* >(L, -1, m_Favorite[i])){ clean(); lua_settop(L, old_top); return false; }
				lua_pop(L, 1);
				RETAIN_POINTER(m_Favorite[i]);
			}
		}
	}
	else{
		return false;
	}
	lua_settop(L, old_top);
	return check();
}
bool Human::appendToString(::core::Bytes* str, int64_t tab_count){
	if(tab_count < 0){ tab_count =0; }
	str->appendString(::core::String::Format("%s{\n", tab(tab_count)));
	// Id
	str->appendString(::core::String::Format("%sId =%s,\n", tab(tab_count+1), ::core::ToString< int64_t >(m_Id)->c_str()));
	// Name
	str->appendString(::core::String::Format("%sName =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Name)->c_str()));
	// Desc
	str->appendString(::core::String::Format("%sDesc =%s,\n", tab(tab_count+1), ::core::ToString< ::core::String* >(m_Desc)->c_str()));
	// Book
	str->appendString(::core::String::Format("%sBook ={\n", tab(tab_count+1)));
	const int64_t Book_cnt =(m_Book ? (m_Book->size()) : 0);
	for(int64_t i=0; i<Book_cnt; ++i){
		::core::String* item =dynamic_cast< ::core::String* >(m_Book->get(i));
		str->appendString(::core::String::Format("%s%s,\n", tab(tab_count+2), ::core::ToString< ::core::String* >(item)->c_str()));
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	// Favorite
	str->appendString(::core::String::Format("%sFavorite ={\n", tab(tab_count+1)));
	for(int64_t i=0; i<3; ++i){
		if(m_Favorite[i]){ m_Favorite[i]->appendToString(str, tab_count + 2); }
	}
	str->appendString(::core::String::Format("%s},\n", tab(tab_count+1)));
	str->appendString(::core::String::Format("%s},\n", tab(tab_count)));
	str->appendNull();
	return true;
}
DEFINE_PROPERTY(Human, int64_t, Id)
void Human::_init_field_Id(){
	m_Id =0;
}
DEFINE_PROPERTY_P(Human, ::core::String*, Name)
void Human::_init_field_Name(){
	m_Name =::core::String::NewString("");
	RETAIN_POINTER(m_Name);
}
DEFINE_PROPERTY_P(Human, ::core::String*, Desc)
void Human::_init_field_Desc(){
	m_Desc =::core::String::NewString("");
	RETAIN_POINTER(m_Desc);
}
DEFINE_PROPERTY_DYNAMIC_ARRAY(Human, ::core::Array*, ::core::String*, Book)
void Human::_init_field_Book(){
	m_Book =0;
}
DEFINE_PROPERTY_FIXED_ARRAY_P(Human, Game*, Favorite, 3)
void Human::_init_field_Favorite(){
	memset(m_Favorite, 0, sizeof(m_Favorite));
}
}
