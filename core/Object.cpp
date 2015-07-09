/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** class Object impl ***/
	DEFINE_SUPPORT_SCRIPT(Object, "core::Object")

	/*** global object ***/
	Object* null =0;

	/** ctor & dtor **/
	Object::Object()
		: m_iRefCount(0){
	}
	Object::~Object(){
	}

	/** init & finalize **/
	void Object::init(){
	}
	Object* Object::clone(){
		return this;
	}

	void Object::finalize(){
	}
	const char* Object::typeName(){
		static thread_local char name[32] ={0};
		strncpy(name, typeid(*this).name(), 31);
		return name;
	}

	/** ref count **/
	void Object::release(){
		if(--m_iRefCount == 0){
			// if(m_iRefCount < 0){
				// ERROR("!!!!!!!!!!!! m_iRefCount %lld < 0", (long long)m_iRefCount);
			// }
			this->finalize();
			this->~Object();
			DEALLOCATE(reinterpret_cast< void* >(this));
		}
	}
	void Object::retain(){
		++m_iRefCount;
	}
	int64_t Object::getRefCount(){
		return m_iRefCount;
	}
	void Object::memoryOrderSync(){
		const int64_t n =m_iRefCount;
		if(n <= 0){
			FATAL("Object ref count <= 0");
		}
	}

	//** hash code **//
	int64_t Object::getHashCode(){
		return static_cast<int64_t>(reinterpret_cast< uint64_t >(this) / 2);
	}

	//** compare **//
	bool Object::equals(Object* obj){
		return this==obj;
	}
	bool Object::Equals(Object* obj1, Object* obj2){
		if(obj1 == obj2){ return true; }
		else if(obj1 && !obj2){ return false; }
		else if(!obj1 && obj2){ return false; }
		else{
			return obj1->equals(obj2);
		}
	}

	//** toString **//
	String* Object::toString(){
		return String::Format("Object[0x%p]", this);
	}

	//** bytes **//
	bool Object::fromBytes(Bytes* bytes){
		return false;
	}
	Bytes* Object::toBytes(Bytes* bytes){
		return 0;
	}
	/** class info **/
	ClassInfo* Object::getClassInfo(){
		FATAL("if you need class info , you must override this in derive class");
		return 0;
	}
	String* Object::getClassName(){
		ClassInfo* ci =getClassInfo();
		ASSERT(ci);
		return ci->getName();
	}

	/** for lua **/
	int Object::GC_FOR_LUA(lua_State* L){
		/// check arg
		if(lua_gettop(L) < 1){
			lua_pushstring(L, "invalid arg");
			lua_error(L);
			return 0;
		}
		Object* object=0;
		if(!get_object_from_lua< Object >(L, -1, object)){
			lua_pushstring(L, "not a valid Object");
			lua_error(L);
			return 0;
		}

		// gc
		if(object){
			object->release();
		}
		return 0;
	}
	bool Object::RegisterToLua(lua_State* L){
		return register_class_to_lua(L, "Core", "Object", MAKE_LUA_METATABLE_NAME(core::Object), &GC_FOR_LUA,
			0, 0,
			0, 0,
			0, 0
		);
	}
}
