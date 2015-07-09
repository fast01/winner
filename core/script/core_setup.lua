--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local global =_G
local pairs =pairs
local print =print
module "Core"

-- var global --
Global =global
-- print("in core_setup.lua Core is " .. tostring(_ENV))

-- func export --
function export(desc)
	for k, v in pairs(desc) do
		Global[k] =v
	end
end
