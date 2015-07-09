Config ={
	Service ={
		{
			Path ="LogService://",
		},
		{
			Path ="S2CallbackService://",
			IdService =18,
			IdBegin =100,
			IdEnd =101
		},
		{
			Path ="S3CoroutineService://",
			IdService =19,
			IdBegin =200,
			IdEnd =201
		},
		{
			Path ="../service/s4_lua/s4_lua.lua",
			IdService =20,
			IdBegin =300,
			IdEnd =301
		},
		--[[
		{
			Path ="data://mysql",
		},
		{
			Path ="base://",
		},
		{
			Path ="shop://",
			IdBegin =0,
			IdEnd =0,
		},
		]]
	},
}

--[[
Config ={
	Service ={
		{
			Path ="name",
			IdBegin =0,
			IdEnd =0,
		},
	},
	Proxy ={
		{
			Id =1,
			ConnectionId =1
		}
	}
}
]]

--[[
Config ={
	Service ={
		{
			Path ="time",
			IdBegin =0,
			IdEnd =0,
		},
	},
}
]]
