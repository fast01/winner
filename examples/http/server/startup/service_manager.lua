Config ={
	Service ={
		{
			Path ="LogService://",
		},
		{
			Path ="S6HttpService://",
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
