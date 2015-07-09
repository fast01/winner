--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local coroutine =coroutine
local pcall =pcall
local assert =assert
module "Core"

-- Parallel --
function Parallel(...)
	-- check arg
	local fn_list ={ ... }
	for i=1, #fn_list do
		if not is_function(fn_list[i]) then
			return 0, "arg must be function"
		end
	end
	-- prepare context
	local context ={
		remain_count =#fn_list,
		error_count =0
	};
	-- run all functions
	for i=1, #fn_list do
		local cr =coroutine.create(function(fn)
			-- call
			local ok, err =pcall(fn)
			if not ok or (is_boolean(err) and not err) then -- pcall fail or fn return false
				context.error_count =context.error_count + 1
				WARN("Parallel error %s", err)
			end
			-- decr remain_count
			context.remain_count =context.remain_count - 1
			-- try resume caller
			if context.remain_count == 0 and context.caller_coroutine then
				if coroutine.status(context.caller_coroutine) ~= 'dead' then
					local ok, err =coroutine.resume(context.caller_coroutine)
					assert(ok, err)
				else
					WARN("caller coroutine dead")
				end
			end
		end);
		coroutine.resume(cr, fn_list[i])
	end
	-- try add caller coroutine to context
	if context.remain_count > 0 then
		context.caller_coroutine =coroutine.running()
		coroutine.yield()
	end
	if context.error_count == 0 then
		return #fn_list
	else
		return (#fn_list - context.error_count), sprintf("error count %d", context.error_count)
	end
end	
