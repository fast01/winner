--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local _G =_G
local debug =debug
local error =error
local table =table
module "Core"

-- CharsetConvert --
CharsetConvert ={
	GbkToUtf8 =function(src)
		return CharsetConvert.MbsConvert(src, "gbk", "utf8")
	end,
	Utf8ToGbk =function(src)
		return CharsetConvert.MbsConvert(src, "utf8", "gbk")
	end,
}

