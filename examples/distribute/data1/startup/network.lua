Config ={
	SlaveCount =1,
	Listener ={
		{
			Id =1,
			Type ='TcpListener',
			Host ="localhost",
			Port =19872,
		}
	},
	Remote ={
		{
			Id =1,
			Host ="127.0.0.1",
			Port =19873
		},
		{
			Id =2,
			Host ="127.0.0.1",
			Port =19874
		}
	},
	Route ={
		{
			IdService =18,
			IdBegin =1,
			IdEnd =3
		}
	}
}
