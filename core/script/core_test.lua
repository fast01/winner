--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
-- UnitTest --
Core.UnitTest ={
	Add =function(fn)
		if type(fn) ~= 'function' then
			return
		end
		Core.UnitTest.test_list =Core.UnitTest.test_list or {}
		table.insert(Core.UnitTest.test_list, fn)
	end,
	Test =function()
		local ls =Core.UnitTest.test_list
		if not ls then
			return
		end
		for i=1, #ls do
			local fn =ls[i]
			fn()
		end
	end
};

-- unit test --
require "test_Parallel"
require "test_Class"
require "test_Actor"
require "test_Md5"
require "test_Base64"
require "test_Sha"
require "test_Json"
require "test_Xml"
require "test_UrlEncode"
require "test_CharsetConvert"
require "test_Mysql"

