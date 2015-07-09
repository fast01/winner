--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local protocol =require "protocol"

SERVICE_ID =SERVICE_ID_S5_LUA
SERVICE_NAME ="s5 lua service"
SERVICE_DESC ="I'm s5 lua service"

Service.On(
	"load",
	function(path)
		Service.SetProtocolGroupId(protocol.GroupId)
	end	
)
Service.On(
	"update",
	function(now)
	end	
)
Service.On(
	"unload",
	function(now)
		DEBUG("unload s5 lua service")
	end	
)
Service.On(
	protocol.S5FirstRequest,
	function(request, packet, body, requestor)
		-- check
		--print(Core.sprint_table(request))
		assert(request.Param1 == 4)
		assert(request.Param2 == false)
		assert(request.Param3 == "s4")
		Service.ReplyEasy(requestor, packet, protocol.S5FirstRespond, {
			Result1 =50,
			Result2 =true,
			Result3 ="from s5"	
		}, true);
	end	
);
Service.On(
	protocol.S5SecondRequest,
	function(request, packet, body, requestor)
		-- check
		assert(request.Param1 == 4)
		assert(request.Param2 == false)
		assert(request.Param3 == "s4")
		Service.ReplyEasy(requestor, packet, protocol.S5FirstRespond, {
			Result1 =50,
			Result2 =true,
			Result3 ="from s5"	
		}, false);
	end	
);
