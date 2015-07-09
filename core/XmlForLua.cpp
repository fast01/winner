/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	inline void xml_to_lua(lua_State* L, TiXmlElement* ele){
		// element table
		lua_newtable(L);

		// try set tag
		if(const char* value =ele->Value()){
			lua_pushstring(L, value);
			lua_setfield(L, -2, "tag");
		}

		// try set text
		if(const char* text =ele->GetText()){
			lua_pushstring(L, text);
			lua_setfield(L, -2, "text");
		}

		// try set attribute
		lua_newtable(L);
		TiXmlAttribute *attr =ele->FirstAttribute();
		while(attr){
			const char* name =attr->Name();
			const char* value =attr->Value();
			if(name && value){
				lua_pushstring(L, value);
				lua_setfield(L, -2, name);
			}
			attr =attr->Next();
		}
		lua_setfield(L, -2, "attr");

		// try set child
		lua_newtable(L);
		int idx =1;
		TiXmlElement * child_ele =ele->FirstChildElement();
		while(child_ele){
			xml_to_lua(L, child_ele);
			lua_rawseti(L, -2, idx);
			idx +=1;
			child_ele =child_ele->NextSiblingElement();
		}
		lua_setfield(L, -2, "child");
	}

	// lua_to_xml
	inline TiXmlElement* lua_to_xml(lua_State* L, const char* default_tag){
		// tag
		const char* tag =default_tag;
		lua_getfield(L, -1, "tag");
		if(lua_isstring(L, -1)){
			tag =lua_tostring(L, -1);
		}
		if(!tag){
			return 0;
		}
		TiXmlElement* ele =new TiXmlElement(tag);
		lua_pop(L, 1);


		// try set text
		lua_getfield(L, -1, "text");
		if(lua_isstring(L, -1)){
			ele->LinkEndChild(new TiXmlText(lua_tostring(L, -1)));
		}
		lua_pop(L, 1);

		// try set attribute
		lua_getfield(L, -1, "attr");
		if(lua_istable(L, -1)){
			lua_pushnil(L);
			while(0 != lua_next(L, -2)){
				if(lua_isstring(L, -2)){
					const char* name =lua_tostring(L, -2);
					const char* value =0;
					if(lua_isstring(L, -1)){
						value =lua_tostring(L, -1);
					}
					else if(lua_isboolean(L, -1)){
						value =(lua_toboolean(L, -1)) ? "true" : "false";
					}
					else if(lua_isnumber(L, -1)){
						char digest[64] ={0};
						ENSURE(sprintf(digest, "%G", lua_tonumber(L, -1)) == 1);
						value =digest;
					}
					if(name && value){
						ele->SetAttribute(name, value);
					}
				}
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

		// try set child
		lua_getfield(L, -1, "child");
		if(lua_istable(L, -1)){
			const int len =lua_objlen(L, -1);
			for(int i=1; i<=len; ++i){
				lua_rawgeti(L, -1, i);
				if(lua_istable(L, -1)){
					ele->LinkEndChild(lua_to_xml(L, "item"));
				}
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);
		return ele;
	}
		
	/*** XmlForLua ***/
	int XmlForLua::_Encode(lua_State* L){
		// prepare name
		if((lua_gettop(L)<1) || (!lua_istable(L, 1))){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		// parse
		TiXmlPrinter printer;
		printer.SetIndent("\t");
		TiXmlElement* ele =lua_to_xml(L, "root");
		if(ele->Accept(&printer) && printer.CStr()){
			lua_pushstring(L, printer.CStr());
			delete ele;
			return 1;
		}
		else{
			lua_pushnil(L);
			lua_pushstring(L, "tinyxml unknown error");
			delete ele;
			return 2;
		}
	}
	int XmlForLua::_Decode(lua_State* L){
		// check arg
		if((lua_gettop(L)<1) || (!lua_isstring(L, 1))){
			lua_pushnil(L);
			lua_pushstring(L, "invalid arg");
			return 2;
		}
		const char* str =lua_tostring(L, 1);
		// parse
		TiXmlDocument doc;
		doc.Parse(str, 0, TIXML_ENCODING_UTF8);
		if(doc.Error()){
			lua_pushnil(L);
			lua_pushfstring(L, "invalid xml string, %s", doc.ErrorDesc());
			return 2;
		}
		TiXmlElement* e =doc.RootElement();
		if(!e){
			lua_pushnil(L);
			lua_pushstring(L, "invalid xml string, root is null");
			return 2;
		}
		xml_to_lua(L, e);
		return 1;
	}
	bool XmlForLua::RegisterToLua(lua_State* L){
		CLASS_FUNC func[2] ={
			{ "Encode", &_Encode },
			{ "Decode", &_Decode },
		};
		return register_class_to_lua(L, "Core", "Xml", 0, 0,
			2, func,
			0, 0,
			0, 0
		);
	}
}
