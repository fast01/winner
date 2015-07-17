--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]

local protocol =require "protocol"

SERVICE_ID =SERVICE_ID_S4_LUA
SERVICE_NAME ="s4 lua service"
SERVICE_DESC ="I'm s4 lua service"

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
		DEBUG("unload s4 lua service")
	end	
)
Service.On(
	protocol.S4FirstRequest,
	function(request, packet, body, requestor)
		-- check
		--print(Core.sprint_table(request))
		assert(request.Param1 == 3)
		assert(request.Param2 == false)
		assert(request.Param3 == "s3")

		-- request s5
		local res, err =Service.Rpc(
			{
				who =who,
				to =SERVICE_ID_S5_LUA,
				command =protocol.S5FirstRequest
			},
			{
				Param1 =4,
				Param2 =false,
				Param3 ="s4"
			},
			protocol.GroupId
		);
		assert(res and res.Result1==50 and res.Result2==true and res.Result3=="from s5", err)

		-- reply
		Service.ReplyEasy(requestor, packet, protocol.S4FirstRespond, {
			Result1 =40,
			Result2 =true,
			Result3 ="from s4"	
		}, true);
	end	
);
Service.On(
	protocol.S4SecondRequest,
	function(request, packet, body, requestor)
		-- check
		assert(request.Param1 == 3)
		assert(request.Param2 == false)
		assert(request.Param3 == "s3")

		-- request s5
		Core.Parallel(
			function()
				local res, err =Service.Rpc(
					{
						who =who,
						to =SERVICE_ID_S5_LUA,
						command =protocol.S5SecondRequest
					},
					{
						Param1 =4,
						Param2 =false,
						Param3 ="s4"
					},
					protocol.GroupId
				);
				assert(res and res.Result1==50 and res.Result2==true and res.Result3=="from s5", err)
			end,
			function()
				Service.Sleep(1)
			end,
			function()
				DEBUG("parallel")
			end
		);

		-- reply
		Service.ReplyEasy(requestor, packet, protocol.S4SecondRespond, {
			Result1 =40,
			Result2 =true,
			Result3 ="from s4"	
		}, false);
	end	
);
