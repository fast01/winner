/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl DispatcherManager ***/
	/** singleton **/
	DEFINE_PROCESS_LOCAL_SINGLETON(DispatcherManager)

	/** ctor & dtor **/
	DispatcherManager::DispatcherManager()
		: m_dispatcher_list(0){
	}
	DispatcherManager::~DispatcherManager(){
	}

	/** Object **/
	void DispatcherManager::init(){
		Super::init();
		m_dispatcher_list =New<Array>();
		m_dispatcher_list->retain();
	}
	void DispatcherManager::finalize(){
		CLEAN_POINTER(m_dispatcher_list);
		Super::finalize();
	}
	/** SELF **/

	void DispatcherManager::addDispatcher(Dispatcher* dispatcher){
		if(!dispatcher){
			return;
		}
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_dispatcher_list->removeEquals(dispatcher);
		m_dispatcher_list->push_back(dispatcher);
	}
	void DispatcherManager::removeDispatcher(Dispatcher* dispatcher){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_dispatcher_list->removeEquals(dispatcher);
	}
	void DispatcherManager::clearDispatcher(){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		m_dispatcher_list->clear();
	}

	/** request **/
	bool DispatcherManager::request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->request(requestor, packet, body, body_len);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager request to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}
	bool DispatcherManager::requestByObject(Requestor* requestor, PACKET& packet, Object* obj){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->requestByObject(requestor, packet, obj);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager request to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}

	/** notify **/
	bool DispatcherManager::notify(PACKET& packet, void* body, const int64_t body_len){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->notify(packet, body, body_len);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager notify to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}
	bool DispatcherManager::notifyByObject(PACKET& packet, Object* obj){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->notifyByObject(packet, obj);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager notify to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}

	/** reply **/
	bool DispatcherManager::reply(PACKET& packet, void* body, const int64_t body_len){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->reply(packet, body, body_len);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager reply to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}
	bool DispatcherManager::replyByObject(PACKET& packet, Object* obj){
		if(Dispatcher* dispatcher =_match(static_cast<int64_t>(packet.to))){
			const bool ret =dispatcher->replyByObject(packet, obj);
			dispatcher->release();
			return ret;
		}
		else{
			ERROR("dispatcher manager reply to %lld failed, missmatch", (long long)packet.to);
			return false;
		}
	}

	/** match **/
	bool DispatcherManager::isMatch(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		const int64_t n =m_dispatcher_list->size();
		for(int64_t i=0; i<n; ++i){
			Dispatcher* dispatcher =static_cast< Dispatcher* >(m_dispatcher_list->get(i));
			if(dispatcher->isMatch(id)){
				return true;
			}
		}
		return false;
	}

	/** util **/
	// request
	bool DispatcherManager::Request(Requestor* requestor, PACKET& packet, void* body, const int64_t body_len){
		if(auto self =Instance()){
			return self->request(requestor, packet, body, body_len);
		}
		return false;
	}
	bool DispatcherManager::RequestByObject(Requestor* requestor, PACKET& packet, Object* obj){
		if(auto self =Instance()){
			return self->requestByObject(requestor, packet, obj);
		}
		return false;
	}
	// notify
	bool DispatcherManager::Notify(PACKET& packet, void* body, const int64_t body_len){
		if(auto self =Instance()){
			return self->notify(packet, body, body_len);
		}
		return false;
	}
	bool DispatcherManager::NotifyByObject(PACKET& packet, Object* obj){
		if(auto self =Instance()){
			return self->notifyByObject(packet, obj);
		}
		return false;
	}
	// reply
	bool DispatcherManager::Reply(PACKET& packet, void* body, const int64_t body_len){
		if(auto self =Instance()){
			return self->reply(packet, body, body_len);
		}
		return false;
	}
	bool DispatcherManager::ReplyByObject(PACKET& packet, Object* obj){
		if(auto self =Instance()){
			return self->replyByObject(packet, obj);
		}
		return false;
	}

	/** private **/
	Dispatcher* DispatcherManager::_match(const int64_t id){
		std::lock_guard<LOCK_TYPE> guard(m_lock);
		const int64_t n =m_dispatcher_list->size();
		for(int64_t i=0; i<n; ++i){
			Dispatcher* dispatcher =static_cast< Dispatcher* >(m_dispatcher_list->get(i));
			if(dispatcher->isMatch(id)){
				dispatcher->retain();
				return dispatcher;
			}
		}
		return 0;
	}
	/** register lua **/
	int DispatcherManager::_Request(lua_State* L){
		// check
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// prepare args
		Requestor* requestor =0;
		if(!get_object_from_lua<Requestor>(L, 1, requestor)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid requestor");
			return 2;
		}
		PACKET packet;
		if(!get_packet_from_lua(L, 2, packet)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid packet");
			return 2;
		}
		Bytes* body =0;
		if(!get_object_from_lua<Bytes>(L, 3, body) || !body){
			lua_pushnil(L);
			lua_pushstring(L, "invalid body");
			return 2;
		}
		// call
		if(Request(requestor, packet, body->data(), body->size())){
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "dispatcher error");
			return 2;
		}
	}
	int DispatcherManager::_RequestByObject(lua_State* L){
		// check
		if(lua_gettop(L) < 3){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// prepare args
		Requestor* requestor =0;
		if(!get_object_from_lua<Requestor>(L, 1, requestor)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid requestor");
			return 2;
		}
		PACKET packet;
		if(!get_packet_from_lua(L, 2, packet)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid packet");
			return 2;
		}
		Object* obj =0;
		get_object_from_lua<Object>(L, 3, obj);
		// call
		if(RequestByObject(requestor, packet, obj)){
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "dispatcher error");
			return 2;
		}
	}
	int DispatcherManager::_Notify(lua_State* L){
		// check
		if(lua_gettop(L) < 2){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// prepare args
		PACKET packet;
		if(!get_packet_from_lua(L, 1, packet)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid packet");
			return 2;
		}
		Bytes* body =0;
		if(!get_object_from_lua<Bytes>(L, 2, body) || !body){
			lua_pushnil(L);
			lua_pushstring(L, "invalid body");
			return 2;
		}
		// call
		if(Notify(packet, body->data(), body->size())){
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "dispatcher error");
			return 2;
		}
	}
	int DispatcherManager::_NotifyByObject(lua_State* L){
		// check
		if(lua_gettop(L) < 2){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// prepare args
		PACKET packet;
		if(!get_packet_from_lua(L, 1, packet)){
			lua_pushnil(L);
			lua_pushstring(L, "invalid packet");
			return 2;
		}
		Object* obj =0;
		get_object_from_lua<Object>(L, 2, obj);
		// call
		if(NotifyByObject(packet, obj)){
			lua_pushboolean(L, 1);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "dispatcher error");
			return 2;
		}
	}
	bool DispatcherManager::RegisterToLua(lua_State* L){
		CLASS_FUNC func[4] ={
			{ "Request", &_Request},
			{ "RequestByObject", &_RequestByObject},
			{ "Notify", &_Notify},
			{ "NotifyByObject", &_NotifyByObject},
		};
		return register_class_to_lua(L, "Core", "DispatcherManager", 0, 0,
			4, func,
			0, 0,
			0, 0
		);
	}
}
