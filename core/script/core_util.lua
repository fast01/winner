--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local type =type
local string =string
local pairs =pairs
local assert =assert
local tostring =tostring
local pcall =pcall
local print =print
local debug =debug
local os =os
local table =table

module "Core"


----
---- encode_quat_string ----
----
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
	local st, err =pcall(string.format, ...)
	if not st then
		print(err)
		print(debug.traceback())
		return ''
	end
	return err
end

----
---- sprint_table ----
----
sprint_table =nil
function sprint_table(tb, option, record)
	-- Global.print(Global.debug.traceback())
	-- prepare args
	option =option or {}
	record =record or {}
	local loop =option.loop or 'mark'
	local tab =option.tab or 0
	local show_table_addr =option.show_table_addr
	local show_func =option.show_func
	local show_coroutine =option.show_coroutine
	local show_userdata =option.show_userdata

	if is_nil(option.show_table_addr) then
		show_table_addr =true
	end

	-- occurs loop 
	if record[tb] then
		if loop == 'mark' then
			return tostring(tb)
		elseif loop == 'error' then
			assert(not record[tb], sprintf("print table %s loop", tostring(tb)))
		else
			return ''
		end
	end

	-- record self
	record[tb] =tb

	-- building
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
			elseif type(item) == 'function' then
				if show_func then
					str =str .. string.rep("\t", tab+1) .. tostring(item)  .. ',\n'
				end
			elseif type(item) == 'thread' then
				if show_coroutine then
					str =str .. string.rep("\t", tab+1) .. tostring(item)  .. ',\n'
				end
			elseif type(item) == 'userdata' then
				if show_userdata then
					str =str .. string.rep("\t", tab+1) .. tostring(item)  .. ',\n'
				end
			elseif type(item) == 'table' then
				local sub_option ={
					loop=loop,
					tab=tab+1,
					show_table_addr=show_table_addr,
					show_func=show_func,
					show_coroutine =show_coroutine,
					show_userdata =show_userdata
				}
				str =str .. string.rep("\t", tab+1) .. sprint_table(item, sub_option, record) .. ',\n'
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
			elseif type(item) == 'function' then
				if show_func then
					str =str .. string.rep("\t", tab+1) .. k .. ' =' .. tostring(item) .. ',\n'
				end
			elseif type(item) == 'thread' then
				if show_coroutine then
					str =str .. string.rep("\t", tab+1) .. k .. ' =' .. tostring(item) .. ',\n'
				end
			elseif type(item) == 'userdata' then
				if show_userdata then
					str =str .. string.rep("\t", tab+1) .. k .. ' =' .. tostring(item) .. ',\n'
				end
			elseif type(item) == 'table' then
				local sub_option ={
					loop=loop,
					tab=tab+1,
					show_table_addr=show_table_addr,
					show_func=show_func,
					show_coroutine =show_coroutine,
					show_userdata =show_userdata
				}
				str =str .. string.rep("\t", tab+1) .. k .. ' =' ..  sprint_table(item, sub_option, record) .. ',\n'
			end
		end
	end
	str =str .. sprintf("%s}", string.rep("\t", tab))
	str =string.gsub(str, "\n[\t]*\n", "\n")
	return str
end

---
--- time
---
function time_to_string(t)
	return os.date("%Y-%m-%d %H:%M:%S", t)
end

--
-- deep copy
--
function deep_copy(obj, depth)
	if is_number(obj) or is_boolean(obj) or is_string(obj) or is_function(obj) then
		return obj
	elseif is_table(obj) then
		depth =depth or 0
		assert(depth < 100000, "deep copy failed, loop")
		local tb ={}
		for k, v in Global.pairs(obj) do
			tb[k] =deep_copy(v, depth + 1)
		end
		return tb
	elseif is_nil(obj) then
		return nil
	else
		WARN("deep copy occurs %s", Global.type(obj))
		return nil
	end
end

---
--- array
---
function array_concate(...)
	local arr_list ={ ... }
	local ret ={}
	for i=1, #arr_list do
		local arr =arr_list[i]
		if not is_table(arr) and not is_nil(arr) then
			return nil, sprintf("arg #%d must be table", i)
		end
		if arr then
			for j=1, #arr do
				table.insert(ret, arr[j])
			end
		end
	end
	return ret
end
