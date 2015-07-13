/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl HtmlTemplate ***/
	/** ctor & dtor **/
	HtmlTemplate::HtmlTemplate()
		: m_L(0){
	}
	HtmlTemplate::~HtmlTemplate(){
	}
	/** Object **/
	void HtmlTemplate::init(){
		Super::init();
		// object pool manager
		OPH();
		
		// new state
		m_L =luaL_newstate();
		if(!m_L){
			ERROR("html template init failed, luaL_newstate error");
			return;	
		}
		luaL_openlibs(m_L);
		lua_settop(m_L, 0);

		// set version
		lua_pushnumber(m_L, LUA_SCRIPT_VERSION);
		lua_setglobal(m_L, "LUA_SCRIPT_VERSION");

		// open core
		if(luaL_dofile(m_L, "../core/script/core.lua")){
			lua_close(m_L);
			m_L =0;
			ERROR("html template init failed, open core error");
			return;
		}
		lua_settop(m_L, 0);

		// call register
		if(!ProcessLocal::Instance()->initLua(m_L)){
			lua_close(m_L);
			m_L =0;
			ERROR("html template init failed, process local register error");
			return;
		}
		lua_settop(m_L, 0);
	}
	void HtmlTemplate::finalize(){
		if(m_L){
			lua_close(m_L);
			m_L =0;
		}
		Super::finalize();	
	}
	/** slef **/
	String* HtmlTemplate::render(String* source, Hash* param){
		/// check
		if(!m_L){
			ERROR("html template render failed, not init");
			return 0;
		}
		if(!source){
			ERROR("html template render failed, source is null");
			return 0;
		}
		if(source->empty()){
			return source;
		}

		/// prepare
		LuaTopHelper lth(m_L);
		// push function
		if(!get_lua_global_var(m_L, "Core.HtmlTemplate.run")){
			ERROR("html template render failed, Core.HtmlTemplate.run is not exist");
			return 0;
		}
		if(lua_isfunction(m_L, -1) == 0){
			ERROR("html template render failed, Core.HtmlTemplate.run is not a function");
			return 0;
		}
		// push source
		lua_pushstring(m_L, source->c_str());

		// push param
		const int64_t narg =_push_hash(param);
		if(narg < 0){
			ERROR("html template render failed, push param error");
			return 0;
		}

		/// call
		if(lua_pcall(m_L, 1+narg, 1, 0) != 0){
			const char* err =lua_tostring(m_L, -1);	
			ERROR("html template render failed, %s", err);
			return 0;
		}
		const char* target =lua_tostring(m_L, -1);
		if(!target){
			ERROR("html template render failed, return is not string");
			return 0;
		}

		/// return
		return String::New(target);
	}
	bool HtmlTemplate::good(){
		return m_L != 0;
	}

	/** private **/
	int64_t HtmlTemplate::_push_hash(Hash* param){
#define _MATCH(t, n) \
		else if(t* n =dynamic_cast< t* >(value)){ \
			lua_pushnumber(m_L, static_cast<double>(n->getValue())); \
		}

#define _MATCH_A(t, n) \
		else if(t##Array* n =dynamic_cast< t##Array* >(value)){ \
			const int count =static_cast<int>(n->size()); \
			lua_createtable(m_L, count, 0); \
			for(int i=0; i<count; ++i){ \
				lua_pushnumber(m_L, static_cast<double>(n->get(i))); \
				lua_rawseti(m_L, -2, i+1); \
			} \
		}

		if(!param){
			return 0;
		}
		lua_createtable(m_L, 0, static_cast<int>(param->size()));
		HashIterator* it =static_cast< HashIterator* >(param->iterator());
		while(it->next()){
			if(String* name =dynamic_cast< String* >(it->getKey())){
				Object* value =it->getValue();
				if(String* str =dynamic_cast< String* >(value)){
					lua_pushstring(m_L, str->c_str());
				}
				else if(Boolean* b =dynamic_cast< Boolean* >(value)){
					lua_pushboolean(m_L, b->getValue() ? 1 : 0);
				}
				_MATCH(Int64, i64)
				_MATCH(Int32, i32)
				_MATCH(Int16, i16)
				_MATCH(Int8, i8)

				_MATCH(Uint64, ui64)
				_MATCH(Uint32, ui32)
				_MATCH(Uint16, ui16)
				_MATCH(Uint8, ui8)

				_MATCH(Float64, f64)
				_MATCH(Float32, f32)

				_MATCH_A(Int64, ai64)
				_MATCH_A(Int32, ai32)
				_MATCH_A(Int16, ai16)
				_MATCH_A(Int8, ai8)

				_MATCH_A(Uint64, aui64)
				_MATCH_A(Uint32, aui32)
				_MATCH_A(Uint16, aui16)
				_MATCH_A(Uint8, aui8)

				_MATCH_A(Float64, af64)
				_MATCH_A(Float32, af32)
				else if(Array* ls =dynamic_cast< Array* >(value)){
					const int count =static_cast< int >(ls->size());
					lua_createtable(m_L, count, 0);
					for(int64_t i=0; i<count; ++i){
						if(String* s =dynamic_cast< String* >(ls->get(i))){
							lua_pushstring(m_L, s->c_str());
							lua_rawseti(m_L, -2, i+1);
						}
						else{
							ERROR("push hash to lua failed, Array item only support String");
							return -1;
						}
					}
				}
				else if(Hash* tb =dynamic_cast< Hash* >(value)){
					if(_push_hash(tb) != 1){
						return -1;
					}
				}
				else{
					ERROR("push hash to lua failed, only support Int* Int*Array, Uint* Uint*Array, Boolean BooleanArray, Float* Float*Array, String, Array with String, Hash");
					return -1;
				}
				lua_setfield(m_L, -2, name->c_str());
			}
		}
		return 1;
#undef _MATCH
#undef _MATCH_A
	}
}
