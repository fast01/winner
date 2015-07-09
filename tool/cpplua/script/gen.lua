#!/usr/local/bin/lua
package.path =package.path .. ";./?.lua;./prototypes/?.pt;../../common/?.lua"

require "build_in_type"
require "util"
require "out"

-- func --
function parse_fromlua(t)
	if is_build_in_type(t) then
		return "::core::FromLua"
	else
		return "::core::get_object_from_lua"
	end
end
function parse_tolua(t)
	if is_build_in_type(t) then
		return "::core::ToLua"
	else
		return "::core::push_object_to_lua"
	end
end
function check_prototype(prototype)
	check_table(prototype, "check_prototype fail, prototype is not a nonempty table")
	check_string(prototype.qualified_name, "check_prototype fail, prototype.qualified_name is not a valid string")
	check_id(prototype.name, "check_prototype fail, prototype.name is not a valid id")

	-- func
	prototype.nonstatic_count =0
	prototype.static_count =0
	prototype.func =prototype.func or {}
	check_table(prototype.func, string.format("check_prototype %s fail, prototype.func is not a valid table", prototype.name))
	for i=1, #prototype.func do
		local func =prototype.func[i]
		check_table(func, string.format("check_prototype %s fail, prototype.func[%d] is not a valid table", prototype.name, i))
		check_id(func.name, string.format("check_prototype %s fail, prototype.func[%d].name is not a valid id", prototype.name, i))
		
		if func.static then
			prototype.static_count =prototype.static_count + 1
		else
			prototype.nonstatic_count =prototype.nonstatic_count + 1
		end

		-- return
		func.ret =func.ret or "void"
		if is_string(func.ret) then
			func.ret ={ type =func.ret }
		end
		check_table(func.ret, string.format("check_prototype %s fail, prototype.func[%d].ret is not a valid table", prototype.name, i))
		check_string(func.ret.type, string.format("check_prototype %s fail, prototype.func[%d].ret.type is not a valid id", prototype.name, i))
		func.ret.tolua =parse_tolua(func.ret.type)
		if func.ret.type == 'void' then
			func.ret.raw_type =func.ret.type
		elseif func.ret.type == "string" then
			func.ret.raw_type =func.ret.type
			func.ret.type ="::core::String*"
		elseif is_build_in_type(func.ret.type) then
			func.ret.type =func.ret.type .. "_t"
			func.ret.raw_type =func.ret.type
		else
			func.ret.raw_type =func.ret.type
			func.ret.type =func.ret.type .. "*"
		end

		-- arg
		func.arg =func.arg or {}
		check_table(func.arg, string.format("check_prototype %s fail, prototype.func[%d].arg is not a valid table", prototype.name, i))
		local old_arg =func.arg
		func.arg ={}
		for j=1, #old_arg do
			local arg ={ type =old_arg[j] }
			table.insert(func.arg, arg)
			check_table(arg, string.format("check_prototype %s fail, prototype.func[%d].arg[%d] is not a valid table", prototype.name, i, j))
			check_string(arg.type, string.format("check_prototype %s fail, prototype.func[%d].arg[%d].type is not a valid id", prototype.name, i, j))
			arg.name =string.format("a%d", j)
			arg.fromlua =parse_fromlua(arg.type)
			if arg.type == "string" then
				arg.raw_type =arg.type
				arg.type ="::core::String*"
			elseif is_build_in_type(arg.type) then
				arg.type =arg.type .. "_t"
				arg.raw_type =arg.type
			else
				arg.raw_type =arg.type
				arg.type =arg.type .. "*"
			end
		end
	end

	-- enum
	prototype.enum =prototype.enum or {}
	for i=1, #prototype.enum do
		local enum =prototype.enum[i]
		check_id(enum, string.format("check_prototype %s fail, prototype.enum[%d] is not a valid string", prototype.name, i))
	end

	-- lua func
	prototype.lua_func =prototype.lua_func or {}
	check_table(prototype.lua_func, string.format("check_prototype %s fail, prototype.lua_func is not a valid table", prototype.name))
	for i=1, #prototype.lua_func do
		local func =prototype.lua_func[i]
		check_table(func, string.format("check_prototype %s fail, prototype.lua_func[%d] is not a valid table", prototype.name, i))
		check_string(func.name, string.format("check_prototype %s fail, prototype.lua_func[%d].name is not a valid string", prototype.name, i))
		func.unique_name =string.gsub(func.name, "%.", "_")
		
		-- return
		func.ret =func.ret or "void"
		if is_string(func.ret) then
			func.ret ={ type =func.ret }
		end
		check_table(func.ret, string.format("check_prototype %s fail, prototype.lua_func[%d].ret is not a valid table", prototype.name, i))
		check_string(func.ret.type, string.format("check_prototype %s fail, prototype.lua_func[%d].ret.type is not a valid id", prototype.name, i))
		func.ret.fromlua =parse_fromlua(func.ret.type)
		if func.ret.type == 'void' then
			func.ret.raw_type =func.ret.type
		elseif func.ret.type == "string" then
			func.ret.raw_type =func.ret.type
			func.ret.type ="::core::String*"
		elseif is_build_in_type(func.ret.type) then
			func.ret.type =func.ret.type .. "_t"
			func.ret.raw_type =func.ret.type
		else
			func.ret.raw_type =func.ret.type
			func.ret.type =func.ret.type .. "*"
		end

		-- arg
		func.arg =func.arg or {}
		check_table(func.arg, string.format("check_prototype %s fail, prototype.lua_func[%d].arg is not a valid table", prototype.name, i))
		local old_arg =func.arg
		func.arg ={}
		for j=1, #old_arg do
			local arg ={ type =old_arg[j] }
			table.insert(func.arg, arg)
			check_table(arg, string.format("check_prototype %s fail, prototype.lua_func[%d].arg[%d] is not a valid table", prototype.name, i, j))
			check_string(arg.type, string.format("check_prototype %s fail, prototype.lua_func[%d].arg[%d].type is not a valid id", prototype.name, i, j))
			arg.name =string.format("a%d", j)
			arg.tolua =parse_tolua(arg.type)
			if arg.type == "string" then
				arg.raw_type =arg.type
				arg.type ="::core::String*"
			elseif is_build_in_type(arg.type) then
				arg.type =arg.type .. "_t"
				arg.raw_type =arg.type
			else
				arg.raw_type =arg.type
				arg.type =arg.type .. "*"
			end
		end
	end
end

-- gen register --
function gen_func_nonstatic(func, prototype)
	outcpp("	static int %s(lua_State* L){", func.name)
	-- get self
	outcpp("		/* get self */")
	outcpp("		%s* self =0;", prototype.qualified_name)
	outcpp("		if(!get_object_from_lua< %s >(L, %d, self) || !self){", prototype.qualified_name, 1)
	outcpp('			ERROR("fail to call %%s, self invalid", __FUNCTION__);')
	outcpp('			return 0;')
	outcpp("		}")
	-- get arg
	outcpp("		/* get arg */")
	local args =""
	for i=1, #func.arg do
		local arg =func.arg[i]
		outcpp("		%s %s;", arg.type, arg.name)
		outcpp("		if(!%s< %s >(L, %d, %s)){", arg.fromlua, arg.type, i+1, arg.name)
		outcpp('			ERROR("fail to call %s, arg %%s invalid", __FUNCTION__);', arg.name)
		outcpp('			return 0;')
		outcpp("		}")
		if #args > 0 then
			args =args .. ', '
		end
		args =args .. arg.name
	end
	-- call
	outcpp("		/* call */")
	if func.ret.type == "void" then
		outcpp("		self->%s(%s);", func.name, args)
		outcpp("		return 0;")
	else
		outcpp("		%s ret =self->%s(%s);", func.ret.type, func.name, args)
		local optional =''
		outcpp("		%s< %s >(L, ret);", func.ret.tolua, func.ret.type)
		outcpp("		return 1;")
	end
	outcpp("	}")
end
function gen_func_static(func, prototype)
	outcpp("	static int %s(lua_State* L){", func.name)
	-- get arg
	outcpp("		/* get arg */")
	local args =""
	for i=1, #func.arg do
		local arg =func.arg[i]
		outcpp("		%s %s;", arg.type, arg.name)
		outcpp("		if(!%s< %s >(L, %d, %s)){", arg.fromlua, arg.type, i, arg.name)
		outcpp('			ERROR("fail to call %%s, arg %s invalid", __FUNCTION__);', arg.name)
		outcpp('			return 0;')
		outcpp("		}")
		if #args > 0 then
			args =args .. ', '
		end
		args =args .. arg.name
	end
	-- call
	outcpp("		/* call */")
	if func.ret.type == "void" then
		outcpp("		%s::%s(%s);", prototype.qualified_name, func.name, args)
		outcpp("		return 0;")
	else
		outcpp("		%s ret =%s::%s(%s);", func.ret.type, prototype.qualified_name, func.name, args)
		outcpp("		%s< %s >(L, ret);", func.ret.tolua, func.ret.type)
		outcpp("		return 1;")
	end
	outcpp("	}")
end
function gen_register(prototype)
	outcpp("/** register %s **/", prototype.qualified_name)
	outcpp("class CL_%s{", prototype.name)
	-- nonstatic func
	for i=1, #prototype.func do
		local func =prototype.func[i]
		if func.static then
			gen_func_static(func, prototype)
		else
			gen_func_nonstatic(func, prototype)
		end
	end
	-- gc
	outcpp("public:")
	if prototype.nonstatic_count > 0 then
		outcpp("	static int GC(lua_State*L){")
		outcpp("		%s* self =0;", prototype.qualified_name)
		outcpp("		if(!get_object_from_lua< %s >(L, 1, self)){", prototype.qualified_name)
		outcpp('			ERROR("fail to call %%s, self invalid", __FUNCTION__);')
		outcpp('			return 0;')
		outcpp("		}")
		outcpp("		RELEASE_POINTER(self);")
		outcpp("		return 0;")
		outcpp("	}")
	end
	-- register
	outcpp("public:")
	outcpp("	static bool Register(lua_State* L){")
	outcpp("		LuaTopHelper tlh(L);")
	outcpp("		// prepare global Class table")
	outcpp('		if(!ensure_lua_global_table(L, "%s.%s")){ return false; }', LUA_MODULE_NAME, prototype.name)
	outcpp("		// prepare prototype table")
	outcpp('		lua_pushstring(L, "%s");', prototype.name)
	outcpp('		lua_setfield(L, -2, "name");')
	outcpp('		lua_pushstring(L, "%s");', prototype.qualified_name)
	outcpp('		lua_setfield(L, -2, "qualified_name");')

	-- enum
	if #prototype.enum > 0 then
		outcpp("		// enum")
		for i=1, #prototype.enum do
			local enum =prototype.enum[i]
			outcpp("		lua_pushinteger(L, %s::%s);", prototype.qualified_name, enum)
			outcpp('		lua_setfield(L, -2, "%s");', enum)
		end
	end

	-- metatable
	if prototype.nonstatic_count > 0 then
		outcpp("		// metatable")
		outcpp('		lua_getfield(L, -1, "metatable");')
		outcpp('		if(lua_istable(L, -1) == 0){')
		outcpp('			lua_pop(L, 1);')
		outcpp('			lua_newtable(L);')
		outcpp('			lua_pushvalue(L, -1);')
		outcpp('			lua_setfield(L, -3, "metatable");')
		outcpp('		}')
		for i=1, #prototype.func do
			local func =prototype.func[i]
			if not func.static then
				outcpp("		lua_pushcfunction(L, CL_%s::%s);", prototype.name, func.name)
				outcpp('		lua_setfield(L, -2, "%s");', func.name)
				outcpp("")
			end
		end
		outcpp("		lua_pushcfunction(L, CL_%s::GC);", prototype.name)
		outcpp('		lua_setfield(L, -2, "__gc");')
		outcpp("")
		outcpp('		lua_pushvalue(L, -1);')
		outcpp('		lua_setfield(L, -2, "__index");')
		outcpp('		lua_setglobal(L, "mt_*%s*_tm");', prototype.qualified_name)
	end

	-- static func
	if prototype.static_count > 0 then
		outcpp("		// static")
		for i=1, #prototype.func do
			local func =prototype.func[i]
			if func.static then
				outcpp("		lua_pushcfunction(L, CL_%s::%s);", prototype.name, func.name)
				outcpp('		lua_setfield(L, -2, "%s");', func.name)
				outcpp("")
			end
		end
	end

	outcpp("		return true;")
	outcpp("	}")
	outcpp("};")
end

function gen_inc(prototype_list)
	set_inc_output(string.format("%s/%s.h", OUTPUT_PATH, OUTPUT_NAME));
	outinc("#ifndef H_AUTO_GEN_CODE_%s_H__", string.upper(OUTPUT_NAME))
	outinc("#define H_AUTO_GEN_CODE_%s_H__", string.upper(OUTPUT_NAME))
	if INCLUDE_FILE then
		outinc('#include "%s"', INCLUDE_FILE)
	end
	outinc("")
	-- namespace begin
	for i=1, #NAMESPACE do
		outinc("namespace %s{", NAMESPACE[i])
	end
	outinc("	bool register_cpplua(lua_State* L);")
	for i=1, #prototype_list do
		local prototype =prototype_list[i]
		for j=1, #prototype.lua_func do
			local lua_func =prototype.lua_func[j]
			local param_str =""
			for k=1, #lua_func.arg do
				local arg =lua_func.arg[k]
				if #param_str > 0 then
					param_str =param_str .. ", "
				end
				param_str =param_str .. string.format("%s %s", arg.type, arg.name)
			end
			outinc("	/** Call lua func %s **/", lua_func.name)
			outinc("	class %s{", lua_func.unique_name)
			outinc("	public:")
			outinc("		%s(lua_State*L, const char* name) : m_L(L){ m_name =String::NewString(name); m_name->retain(); }", lua_func.unique_name)
			outinc("		~%s(){ m_name->release(); }", lua_func.unique_name)
			outinc("	public:")
			outinc("		%s operator()(%s){", lua_func.ret.type, param_str)
			outinc("			typedef %s ret_type;", lua_func.ret.type)
			outinc("			LuaTopHelper lth(m_L);")
			outinc("			// push func")
			outinc("			get_lua_global_var(m_L, m_name->c_str());")
			outinc("			if(!lua_isfunction(m_L, -1)){")
			outinc('				ERROR("fail to call %s, not a valid lua function");', lua_func.name)
			outinc("				return %s;", lua_func.ret.type=='void' and "" or "ret_type()")
			outinc("			}")
			outinc("			// push arg")
			for k=1, #lua_func.arg do
				local arg =lua_func.arg[k]
				outinc("			%s< %s >(m_L, %s);", arg.tolua, arg.type, arg.name)
			end
			outinc("			// call")
			if lua_func.ret.type == 'void' then
				outinc("			const int errcode =lua_pcall(m_L, %d, 0, 0);", #lua_func.arg)
				outinc("			if(errcode != 0){")
				outinc('				ERROR("fail to call %s, %%s", lua_tostring(m_L, -1));', lua_func.name)
				outinc("			}")
			else
				outinc("			const int errcode =lua_pcall(m_L, %d, 1, 0);", #lua_func.arg)
				outinc("			if(errcode != 0){")
				outinc('				ERROR("fail to call %s, %%s", lua_tostring(m_L, -1));', lua_func.name)
				outinc("				return ret_type();")
				outinc("			}")
				outinc("			else{")
				outinc("				%s ret;", lua_func.ret.type)
				outinc("				if(!%s< %s >(m_L, -1, ret)){", lua_func.ret.fromlua, lua_func.ret.type)
				outinc('					ERROR("fail to call %s, return value invalid");', lua_func.name)
				outinc("					return ret_type();")
				outinc("				}")
				outinc("				return ret;")
				outinc("			}")
			end
			outinc("		}")
			outinc("	private:")
			outinc("		lua_State* m_L;")
			outinc("		::core::String* m_name;")
			outinc("	};")
		end
	end
	-- namespace close
	for i=1, #NAMESPACE do
		outinc("}");
	end
	outinc("#endif")
end
function gen_cpp(prototype_list)
	set_cpp_output(string.format("%s/%s.cpp", OUTPUT_PATH, OUTPUT_NAME));
	outcpp('#include "%s.h"', OUTPUT_NAME)
	outcpp("")
	-- namespace begin
	for i=1, #NAMESPACE do
		outcpp("namespace %s{", NAMESPACE[i])
	end
	outcpp("")
	for i=1, #prototype_list do
		local prototype =prototype_list[i]
		gen_register(prototype)
	end
	outcpp("bool register_cpplua(lua_State* L){")
	for i=1, #prototype_list do
		local prototype =prototype_list[i]
		outcpp("	CL_%s::Register(L);", prototype.name)
	end
	outcpp("	return true;")
	outcpp("}")
	outcpp("")
	-- namespace close
	for i=1, #NAMESPACE do
		outcpp("}");
	end
end

----
---- compile
----
-- load data and check --
if #arg < 2 then
	print("missing input dir or dir empty")
	os.exit(1)
end
local g_dir =path_ensure_end_without_slash(arg[1])
print("directory is " .. g_dir)

local g_prototype_list ={}
for i=2, #arg do
	local file =arg[i]
	if #file>3 and string.sub(file, -3, -1)==".pt" then
		local module_name =string.sub(file, 1, -4)
		print("compiling " .. module_name)
		dofile(g_dir .. "/" .. file)
		local prototype =_G[module_name];
		check_table(prototype, "moudle is not " .. module_name)
		for k, v in pairs(prototype) do
			if type(k) == 'string' and string.sub(k, 1, 1)>='A' and string.sub(k, 1, 1)<='Z' and type(v) == 'table' then
				print ("\tparsing " .. k)
				v.name =k
				check_prototype(v)
				table.insert(g_prototype_list, v)
			end
		end
	end
end

-- load config --
local cfg =require "config"
NAMESPACE    =cfg.Namespace
OUTPUT_NAME  =cfg.OutputName
OUTPUT_PATH  =cfg.OutputPath
LUA_MODULE_NAME =cfg.LuaModuleName
INCLUDE_FILE =cfg.IncludeFile

check_table(NAMESPACE, "NAMESPACE is not valid")
check_nstring(OUTPUT_PATH, "OUTPUT_PATH is not valid")
check_nstring(OUTPUT_NAME, "OUTPUT_NAME is not valid")
check_nstring(LUA_MODULE_NAME, "LUA_MODULE_NAME is not valid")
OUTPUT_PATH =path_ensure_end_without_slash(OUTPUT_PATH)

-- gen code --
gen_inc(g_prototype_list)
gen_cpp(g_prototype_list)
print("success")
