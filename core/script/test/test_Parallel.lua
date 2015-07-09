--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- Parallel checking --")
	local n, err =Parallel(
		function()
			-- print "a"
		end,
		function()
			-- print "b"
		end,
		function()
			-- print "c"
			return nil
		end,
		function()
			-- print "d"
		end,
		function()
			-- print "e"
		end
	);
	assert(n==5, err)
end);

