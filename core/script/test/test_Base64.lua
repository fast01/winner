--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
module "Core"

UnitTest.Add(function()
	DEBUG("-- Base64 checking --")
	local src ="fool love winne";
	local dst =Base64.Encode(src);

	assert(dst == 'Zm9vbCBsb3ZlIHdpbm5l')
	assert(Base64.Decode(dst) == src)
	local test =function(src)
		local dst =Base64.Encode(src);
		-- DEBUG("%s => %s", src, dst)
		assert(Base64.Decode(dst) == src)
	end

	test("1")
	test("12")
	test("123")
	test("1234")
	test("12345")
	test("123456")
end);

