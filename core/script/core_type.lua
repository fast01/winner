--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local type =type
local assert =assert
local string =string

module "Core"

-- build in type --
BuildInTypeTable ={
	boolean =true,
	int8    =true,
	int16   =true,
	int32   =true,
	int64   =true,
	uint8   =true,
	uint16  =true,
	uint32  =true,
	uint64  =true,
	float32 =true,
	float64 =true,
	string  =true,
};

-- type trait --
function is_build_in_type(t)
	return BuildInTypeTable[t]
end
function is_nil(v)
	return type(v) == 'nil'
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
	if not ( (ch>='a' and ch<='z') or (ch>='A' and ch<='Z') or (ch=='_') ) then
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
assert(is_id("a1111"))
assert(is_id("_1111"))
assert(not is_id("1111"))
assert(not is_id("a1-11"))
assert(not is_id("-a1-11"))

