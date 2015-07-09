--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local _G =_G
local debug =debug
local error =error
module "Core"

function LOG(lv, ...)
	Logger.PrintLog(lv, sprintf(...))
end
function INFO(...)
	Logger.Info(sprintf(...))
end
function WARN(...)
	Logger.Warn(sprintf(...))
end
function ERROR(...)
	-- Logger.Error(debug.traceback())
	local errmsg =sprintf(...)
	-- Logger.Error(errmsg)
	error(errmsg)
end
function FATAL(...)
	Logger.Error(debug.traceback())
	Logger.Fatal(sprintf(...))
end
function DEBUG(...)
	Logger.Debug(sprintf(...))
end

-- export --
export{
	LOG = LOG,
	INFO = INFO,
	WARN = WARN,
	ERROR = ERROR,
	FATAL = FATAL,
	DEBUG = DEBUG,
};
