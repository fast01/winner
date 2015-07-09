require "build_in_type"

----
---- is ----
----
function is_build_in_type(t)
	return BuildInTypeTable[t]
end
function is_number(v)
	return type(v) == 'number'
end
function is_string(v)
	return type(v) == 'string'
end
function is_nstring(v)
	return type(v) == 'string' and #v>0
end
function is_boolean(v)
	return type(v) == 'boolean'
end
function is_table(v)
	return type(v) == 'table'
end
function is_ntable(v)
	return type(v) == 'ntable'
end
function is_object(v)
	return type(v) == 'userdata'
end
function is_userdata(v)
	return type(v) == 'userdata'
end
function is_function(v)
	return type(v) == 'function'
end
function is_coroutine(v)
	return type(v) == 'thread'
end
function is_id(id)
	if type(id)~='string' or #id==0 then
		return false
	end
	local ch =string.sub(id, 1, 1)
	if ch>='0' and ch<='9' then
		return false
	end
	for i=2, #id do
		local ch =string.sub(id, i, i)
		if not( (ch>='0' and ch<='9') or (ch>='a' and ch<='z') or (ch>='A' and ch<='Z') or (ch=='_') ) then
			return false
		end
	end
	return true
end

----
---- check ----
----
function check_id(id, msg)
	if type(id)~='string' or #id==0 then
		perror(msg)
	end
	local ch =string.sub(id, 1, 1)
	if ch>='0' and ch<='9' then
		perror(msg)
	end
	for i=1, #id do
		ch =string.sub(id, i, i)
		if not( (ch>='0' and ch<='9') or (ch>='a' and ch<='z') or (ch>='A' and ch<='Z') or (ch=='_') ) then
			perror(msg)
		end
	end
end
function check_type(t, msg)
	if type(t)~='string' or #t==0 then
		perror(msg)
	end
	if is_build_in_type(t) then
		return
	end
	check_id(t, msg)
end
function check_int(n, msg)
	if type(n)~='number' then
		perror(msg)
	end
end
function check_string(s, msg)
	if type(s)~='string' then
		perror(msg)
	end
end
function check_nstring(s, msg)
	if type(s)~='string' or #s==0 then
		perror(msg)
	end
end
function check_table(tb, msg)
	if type(tb)~='table' then
		perror(msg)
	end
end
function check_ntable(tb, msg)
	if type(tb)~='table' or #tb==0 then
		perror(msg)
	end
end
function check_nil(v, msg)
	if type(v)~='nil' then
		perror(msg)
	end
end
function check_not_nil(v, msg)
	if type(v)=='nil' then
		perror(msg)
	end
end
function check(v, msg)
	if not v then
		perror(msg)
	end
end

----
---- path util ----
----
function path_is_end_with_slash(path)
	return #path>0 and string.sub(path, -1, -1) == '/'
end
function path_ensure_end_with_slash(path)
	if #path>0 and string.sub(path, -1, -1) ~= '/' then
		path =path .. '/'
	end
	return path
end
function path_ensure_end_without_slash(path)
	if #path>0 and string.sub(path, -1, -1) == '/' then
		path =string.sub(path, 1, -2)
	end
	return path
end

----
---- error ----
----
function perror(msg)
	print(msg)
	print(debug.traceback())
	os.exit(1)
end
function syntax_error(msg)
	print("syntax error:" .. msg)
	print(debug.traceback())
	os.exit(1)
end

function encode_quat_string(str)
	str =string.gsub(str, '\\', '\\\\')
	str =string.gsub(str, '"', '\\"')
	str =string.gsub(str, '\'', "\\'")
	return str
end

----
---- sprintf ----
----
function sprintf(...)
	return string.format(...)
end

----
---- sprint_table ----
----
function sprint_table(tb, tab, record_tb, loop_opt)
	record_tb =record_tb or {}
	loop_opt =loop_opt or 'mark'
	local show_table_addr =loop_opt == 'mark' or loop_opt == 'error'
	if record_tb[tb] then -- occurs loop
		if loop_opt == 'mark' then
			return tostring(tb)
		elseif loop_opt == 'error' then
			assert(not record_tb[tb], sprintf("print table %s loop", tostring(tb)))
		else
			return ''
		end
	end
	tab =tab or 0
	local str =""
	str =str .. sprintf("\n%s%s{\n", string.rep("\t", tab), (show_table_addr and sprintf("<%s>", tostring(tb)) or ''))
	if #tb > 0 then
		for i=1, #tb do
			local item =tb[i]
			if type(item) == 'nil' then
				str =str .. string.rep("\t", tab+1) .. 'nil' .. ',\n'
			elseif type(item) == 'boolean' then
				str =str .. string.rep("\t", tab+1) .. (item and 'true' or 'false') .. ',\n'
			elseif type(item) == 'number' then
				str =str .. string.rep("\t", tab+1) .. item  .. ',\n'
			elseif type(item) == 'string' then
				str =str .. string.rep("\t", tab+1) .. '"' .. encode_quat_string(item) .. '"' .. ',\n'
			elseif type(item) == 'table' then
				str =str .. string.rep("\t", tab+1) .. sprint_table(item, tab+1, record_tb, loop_opt) .. ',\n'
			end
		end
	else
		for k, item in pairs(tb) do
			if type(item) == 'boolean' then
				str =str .. string.rep("\t", tab+1) .. k .. ' =' .. (item and 'true' or 'false') .. ',\n'
			elseif type(item) == 'number' then
				str =str .. string.rep("\t", tab+1) .. k .. ' =' ..  item  .. ',\n'
			elseif type(item) == 'string' then
				str =str .. string.rep("\t", tab+1) .. k .. ' =' ..  '"' .. encode_quat_string(item) .. '"' .. ',\n'
			elseif type(item) == 'table' then
				str =str .. string.rep("\t", tab+1) .. k .. ' =' ..  sprint_table(item, tab+1, record_tb, loop_opt) .. ',\n'
			end
		end
	end
	str =str .. sprintf("%s}", string.rep("\t", tab))
	record_tb[tb] =tb
	str =string.gsub(str, "\n[\t]*\n", "\n")
	return str
end

---
--- provide module and export
---
if not module then
	function module(name)
		_G.cur_mod =name
	end
	function export(tb)
		_G[_G.cur_mod] =tb
	end
else
	function export(tb)
		for k, v in pairs(tb) do
			M[k] =v
		end
	end
end
