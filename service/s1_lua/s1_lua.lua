--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]

local protocol =require "protocol"

SERVICE_ID =SERVICE_ID_S1_LUA
SERVICE_NAME ="s1 lua service"
SERVICE_DESC ="I'm s1 lua service"

function test_first(who)
	local res, err =Service.Rpc(
		{
			who =who,
			to =SERVICE_ID_S2_CALLBACK,
			command =protocol.S2FirstRequest
		},
		{
			Param1 =1,
			Param2 =false,
			Param3 ="s1"
		},
		protocol.GroupId
	);
	assert(res and res.Result1==20 and res.Result2==true and res.Result3=="from s2", err)
	print(Core.sprint_table(res))
	Core.LogService.Log(2, 0, "test_first\n");
end
function test_second()
	local res, err =Service.Rpc(
		{
			to =SERVICE_ID_S2_CALLBACK,
			command =protocol.S2SecondRequest
		},
		{
			Param1 =1,
			Param2 =false,
			Param3 ="s1"
		},
		protocol.GroupId
	);
	assert(res and res.Result1==20 and res.Result2==true and res.Result3=="from s2", err)
	DEBUG(Core.sprint_table(res))
end

Service.On(
	"load",
	function(path)
		Service.SetProtocolGroupId(protocol.GroupId)
		--[[
		do
			return
		end
		]]
		-- timer
		Service.SetTimer{
			id =1,
			interval =3,
			counter =1,
			func =function(timer)
				print("begin")
				local n =10
				local t1 =os.time()
				for i=1, n do
					print("index " .. i)
					test_first(i)
					test_second()
				end
				local t2 =os.time()
				print("end")
				local elapse =t2 - t1
				print(Core.sprintf("elapse %d, fps %f", elapse, n/elapse))
			end
		};
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
		DEBUG("unload s1 lua service")
	end	
)
