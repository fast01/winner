--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]

local protocol =require "protocol"

SERVICE_ID =SERVICE_ID_S7_LUA_HTTP
SERVICE_NAME ="s7 lua http service"
SERVICE_DESC ="I'm s7 lua http service"

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
		DEBUG("unload s7 lua service")
	end	
)
Service.On('get', '/', function(request, respond)
	-- http rpc
	local res =Service.HttpGet("http://www.baidu.com/")
	print(Core.sprint_table(res or {'empty'}))

	-- logic rpc
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
	DEBUG(Core.sprint_table(res))

	-- respond
	respond:write([[
	<html>
		<head>
			<title> hello winner </title>
		</head>
		<body>
			<h1> i love winner </h1>
		</body>
	</html>	
	]])
end)

Service.On('get', '/index.lua', function(request, respond)
	local content =Core.load_data('html/view/index.lua')
	print(Core.sprint_table(request))
	local str =Core.HtmlTemplate.run(content, request.request)
	respond:write(str)
end)
