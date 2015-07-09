Config ={
	Service ={
		{
			Path ="LogService://",
			IdService =4,
			IdBegin =100,
			IdEnd =105
		},
		{
			Path ="../service/s1_lua/s1_lua.lua",
		},
		{
			Path ="S2CallbackService://",
		},
		{
			Path ="S3CoroutineService://",
		},
		{
			Path ="../service/s4_lua/s4_lua.lua",
		},
		{
			Path ="../service/s5_lua/s5_lua.lua",
		},
		--[[
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
