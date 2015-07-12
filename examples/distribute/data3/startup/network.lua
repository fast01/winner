--[[
Config ={
	Listener ={
		Host ="localhost",
		Port =1987,
	},
	Remote ={
		{
			Id =1,
			Host ="127.0.0.1",
			Port =1999
		}
	},
	ThreadCount =1
}
]]

Config ={
	SlaveCount =1,
	Listener ={
		{
			Id =1,
			Type ='TcpListener',
			Host ="localhost",
			Port =19874,
		}
	},
	--[[
	Remote ={
		{
			Id =1,
			Host ="127.0.0.1",
			Port =19871
		}
	},
	Route ={
		{
			IdService =18,
			IdBegin =1,
			IdEnd =2
		}
	}
	]]
}
