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
local done =false
Service.On(
	"update",
	function(now)
		if done then
			return
		end
		done =true
		if string.find(DATA_PATH, 'client') then
			DEBUG('client')
			local res =Service.HttpPost("127.0.0.1:19871/index.lua", { name ={ value ='fool' } }, { name='fool' })
			--local res =Service.HttpPost("127.0.0.1:19871/index.lua", { name ='fool' }, { name='fool', age=9999 })
			--print(Core.sprint_table(res or {'empty'}))
		else
			DEBUG('server')
		end
	end	
)
Service.On(
	"unload",
	function(now)
		DEBUG("unload s7 lua service")
	end	
)
Service.On('get', '/', function(request, respond)
	DEBUG("get /")
	--print(Core.sprint_table(request))
	--http rpc
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
	DEBUG("get /index.lua")
	local content =Core.load_data('html/view/index.lua')
	--print(Core.sprint_table(request))
	local str =Core.HtmlTemplate.run(content, request.request)
	respond:write(str)
end)

Service.On('post', '/index.lua', function(request, respond)
	DEBUG("post /index.lua")
	local content =Core.load_data('html/view/index.lua')
	--print(Core.sprint_table(request))
	local str =Core.HtmlTemplate.run(content, request.request)
	respond.cookie.name ={ value ='fool', max_age =1000000000, http_only =1, secure =1, path ='/', domain ='127.0.0.1', age=9999 }
	respond:write(str)
end)
