--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
module "Core"

UnitTest.Add(function()
	DEBUG("-- CharsetConvert checking --")
	local src =".-_~ fool我 love winne";
	local dst =CharsetConvert.Convert(src, "utf8", "gb2312");
	assert(CharsetConvert.Convert(dst, "gb2312", "utf8") == src)

	local test =function(src)
		local dst =CharsetConvert.Convert(src, "utf8", "gb2312");
		assert(CharsetConvert.Convert(dst, "gb2312", "utf8") == src)
	end

	test("1")
	test("12")
	test("123")
	test("12卧34")
	test("1234详细信息5")
	test("e12阿萨斯萨345aaaasssss6")
end);

