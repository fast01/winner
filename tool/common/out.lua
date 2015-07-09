require "util"

----
---- lua file
----
local g_lua_file =nil
function set_lua_output(path)
	check_nstring(path)
	if g_lua_file then
		g_lua_file:close()
		g_lua_file =nil
	end
	g_lua_file =io.open(path, "w")
	if not g_lua_file then
		perror("fail to open " .. path)
	end
end
function outlua(fmt, ...)
	check_not_nil(g_lua_file)
	g_lua_file:write(string.format(fmt, ...) .. "\n")
end

----
---- inc file
----
local g_inc_file =nil
function set_inc_output(path)
	check_nstring(path)
	if g_inc_file then
		g_inc_file:close()
		g_inc_file =nil
	end
	g_inc_file =io.open(path, "w")
	if not g_inc_file then
		perror("fail to open " .. path)
	end
end
function outinc(fmt, ...)
	check_not_nil(g_inc_file)
	g_inc_file:write(string.format(fmt, ...) .. "\n")
end


----
---- cpp file
----
local g_cpp_file =nil
function set_cpp_output(path)
	check_nstring(path)
	if g_cpp_file then
		g_cpp_file:close()
		g_cpp_file =nil
	end
	g_cpp_file =io.open(path, "w")
	if not g_cpp_file then
		perror("fail to open " .. path)
	end
end
function outcpp(fmt, ...)
	check_not_nil(g_cpp_file)
	g_cpp_file:write(string.format(fmt, ...) .. "\n")
end

