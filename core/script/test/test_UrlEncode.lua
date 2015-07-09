--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
module "Core"

UnitTest.Add(function()
	DEBUG("-- UrlEncode checking --")
	local src =".-_~ fool我 love winne";
	local dst =UrlEncode.Encode(src);
	assert(dst == ".-_~%20fool%E6%88%91%20love%20winne")
	assert(UrlEncode.Decode(dst) == src)

	local test =function(src)
		local dst =UrlEncode.Encode(src);
		-- DEBUG("%s => %s", src, dst)
		assert(UrlEncode.Decode(dst) == src)
	end

	test("1")
	test("12")
	test("123")
	test("1234")
	test("12345")
	test("123456")
end);
