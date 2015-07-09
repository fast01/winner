--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- Json checking --")
	local text =[[{"books":["C",null,"C++",1987],"study":{"name":"c++"},"learn":[22,{"name":"ai"},{"name":"aia"}],"man":true,"name":"fool","age":27}]];

	local tb, err =Json.Decode(text);
	assert(tb, err)

	local text1, err =Json.Encode(tb);
	assert(text1, err)

	assert(#text1 == #text)
end);
