--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- Md5 checking --")
	local text ="fool love winne";
	local code =Md5.Sum(text);
	assert(code == '83bf4378d8517d024980293480fd9533')
end);
