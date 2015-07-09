/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** json to lua **/
	inline void json_to_lua(lua_State* L, cJSON* ele){
		// try set child
		lua_checkstack(L, 2);
		if(ele->type == cJSON_Array){
			lua_newtable(L);
			cJSON* child =ele->child;
			int64_t idx =1;
			while(child){
				switch(child->type){
				case cJSON_NULL:
					lua_pushnil(L);
					lua_rawseti(L, -2, idx);
					break;
				case cJSON_False:
					lua_pushboolean(L, 0);
					lua_rawseti(L, -2, idx);
					break;
				case cJSON_True:
					lua_pushboolean(L, 1);
					lua_rawseti(L, -2, idx);
					break;
				case cJSON_Number:
					lua_pushnumber(L, child->valuedouble);
					lua_rawseti(L, -2, idx);
					break;
				case cJSON_String:
					lua_pushstring(L, child->valuestring);
					lua_rawseti(L, -2, idx);
					break;
				case cJSON_Array:
				case cJSON_Object:
					json_to_lua(L, child);
					lua_rawseti(L, -2, idx);
					break;
				default:
					break;
				}
				idx +=1;
				child =child->next;
			}
		}
		else if(ele->type == cJSON_Object){
			lua_newtable(L);
			cJSON* child =ele->child;
			while(child){
				switch(child->type){
				case cJSON_NULL:
					break;
				case cJSON_False:
					lua_pushboolean(L, 0);
					lua_setfield(L, -2, child->string);
					break;
				case cJSON_True:
					lua_pushboolean(L, 1);
					lua_setfield(L, -2, child->string);
					break;
				case cJSON_Number:
					lua_pushnumber(L, child->valuedouble);
					lua_setfield(L, -2, child->string);
					break;
				case cJSON_String:
					lua_pushstring(L, child->valuestring);
					lua_setfield(L, -2, child->string);
					break;
				case cJSON_Array:
				case cJSON_Object:
					json_to_lua(L, child);
					lua_setfield(L, -2, child->string);
					break;
				default:
					break;
				}
				child =child->next;
			}
		}
		else{
			FATAL("never run to here");
		}
	}

	// lua_to_json
	inline cJSON* lua_to_json(lua_State* L){
		if(lua_objlen(L, -1) > 0){
			cJSON* ele =cJSON_CreateArray();
			const int len =lua_objlen(L, -1);
			for(int i=1; i<=len; ++i){
				lua_rawgeti(L, -1, i);
				if(lua_isnil(L, -1)){
					cJSON_AddItemToArray(ele, cJSON_CreateNull());
				}
				else if(lua_isboolean(L, -1)){
					if(lua_toboolean(L, -1)){
						cJSON_AddItemToArray(ele, cJSON_CreateTrue());
					}
					else{
						cJSON_AddItemToArray(ele, cJSON_CreateFalse());
					}
				}
				else if(lua_isnumber(L, -1)){
					cJSON_AddItemToArray(ele, cJSON_CreateNumber(lua_tonumber(L, -1)));
				}
				else if(lua_isstring(L, -1)){
					cJSON_AddItemToArray(ele, cJSON_CreateString(lua_tostring(L, -1)));
				}
				else if(lua_istable(L, -1)){
					cJSON_AddItemToArray(ele, lua_to_json(L));
				}
				lua_pop(L, 1);
			}
			return ele;
		}
		else{
			cJSON* ele =cJSON_CreateObject();
			lua_pushnil(L);
			while(0 != lua_next(L, -2)){
				if(lua_isstring(L, -2)){
					const char* name =lua_tostring(L, -2);
					if(lua_isboolean(L, -1)){
						if(lua_toboolean(L, -1)){
							cJSON_AddTrueToObject(ele, name);
						}
						else{
							cJSON_AddFalseToObject(ele, name);
						}
					}
					else if(lua_isnumber(L, -1)){
						cJSON_AddNumberToObject(ele, name, lua_tonumber(L, -1));
					}
					else if(lua_isstring(L, -1)){
						cJSON_AddStringToObject(ele, name, lua_tostring(L, -1));
					}
					else if(lua_istable(L, -1)){
						cJSON_AddItemToObject(ele, name, lua_to_json(L));
					}
				}
				lua_pop(L, 1);
			}
			return ele;
		}
	}
	/*** JsonForLua ***/
	int JsonForLua::_Encode(lua_State* L){
		if((lua_gettop(L)>=1) && (lua_istable(L, 1))){
			cJSON* root =lua_to_json(L);
			char* str =cJSON_PrintUnformatted(root);
			lua_pushstring(L, str);
			free(str);
			cJSON_Delete(root);
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
	}
	int JsonForLua::_Decode(lua_State* L){
		// check arg
		if((lua_gettop(L)<1) || (!lua_isstring(L, 1))){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		const char* str =lua_tostring(L, 1);
		// parse
		cJSON* root =cJSON_Parse(str);
		if(!root){
			lua_pushnil(L);
			lua_pushstring(L, "json format error");
			return 2;
		}
		if(root->type == cJSON_Object || root->type == cJSON_Array){
			json_to_lua(L, root);
			cJSON_Delete(root);
			return 1;
		}
		else{
			cJSON_Delete(root);
			lua_pushnil(L);
			lua_pushstring(L, "json decode error, invalid root node, only allow object and array");
			return 2;
		}
	}
	bool JsonForLua::RegisterToLua(lua_State* L){
		CLASS_FUNC func[2] ={
			{ "Encode", &_Encode },
			{ "Decode", &_Decode },
		};
		return register_class_to_lua(L, "Core", "Json", 0, 0,
			2, func,
			0, 0,
			0, 0
		);
	}
}
