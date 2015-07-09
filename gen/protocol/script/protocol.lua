module 'protocol'
-- group --
GroupId =17;
Group ={};

-- protocol ids --
S2FirstRequest =1001;
S2FirstRespond =1002;
S2SecondRequest =1003;
S2SecondRespond =1004;
S3FirstRequest =1005;
S3FirstRespond =1006;
S3SecondRequest =1007;
S3SecondRespond =1008;
S5FirstRequest =1009;
S5FirstRespond =1010;
S5SecondRequest =1011;
S5SecondRespond =1012;
S4FirstRequest =1013;
S4FirstRespond =1014;
S4SecondRequest =1015;
S4SecondRespond =1016;
Family =1017;
Game =1018;
Human =1019;

-- protocol list --
-- S2FirstRequest
Group[S2FirstRequest] ={
	id =S2FirstRequest,
	name ='S2FirstRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S2FirstRespond
Group[S2FirstRespond] ={
	id =S2FirstRespond,
	name ='S2FirstRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S2SecondRequest
Group[S2SecondRequest] ={
	id =S2SecondRequest,
	name ='S2SecondRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S2SecondRespond
Group[S2SecondRespond] ={
	id =S2SecondRespond,
	name ='S2SecondRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S3FirstRequest
Group[S3FirstRequest] ={
	id =S3FirstRequest,
	name ='S3FirstRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S3FirstRespond
Group[S3FirstRespond] ={
	id =S3FirstRespond,
	name ='S3FirstRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S3SecondRequest
Group[S3SecondRequest] ={
	id =S3SecondRequest,
	name ='S3SecondRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S3SecondRespond
Group[S3SecondRespond] ={
	id =S3SecondRespond,
	name ='S3SecondRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S5FirstRequest
Group[S5FirstRequest] ={
	id =S5FirstRequest,
	name ='S5FirstRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S5FirstRespond
Group[S5FirstRespond] ={
	id =S5FirstRespond,
	name ='S5FirstRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S5SecondRequest
Group[S5SecondRequest] ={
	id =S5SecondRequest,
	name ='S5SecondRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S5SecondRespond
Group[S5SecondRespond] ={
	id =S5SecondRespond,
	name ='S5SecondRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S4FirstRequest
Group[S4FirstRequest] ={
	id =S4FirstRequest,
	name ='S4FirstRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S4FirstRespond
Group[S4FirstRespond] ={
	id =S4FirstRespond,
	name ='S4FirstRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- S4SecondRequest
Group[S4SecondRequest] ={
	id =S4SecondRequest,
	name ='S4SecondRequest',
	field ={
		{
			type ='int64',
			count=1,
			name ='Param1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Param2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Param3',
			required =false,
			init_value ="",
			min_length =1,
			max_length =128,
		},
	},
};

-- S4SecondRespond
Group[S4SecondRespond] ={
	id =S4SecondRespond,
	name ='S4SecondRespond',
	field ={
		{
			type ='int64',
			count=1,
			name ='Error',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Result1',
			required =false,
			init_value =0,
		},
		{
			type ='boolean',
			count=1,
			name ='Result2',
			required =false,
			init_value =false,
		},
		{
			type ='string',
			count=1,
			name ='Result3',
			required =false,
			init_value ="",
		},
	},
};

-- Family
Group[Family] ={
	id =Family,
	name ='Family',
	field ={
		{
			type ='Human',
			count=1,
			name ='Father',
			required =false,
			proto =Human,
		},
		{
			type ='Human',
			count=1,
			name ='Mother',
			required =false,
			proto =Human,
		},
		{
			type ='Human',
			count=0,
			name ='Children',
			required =false,
			proto =Human,
		},
	},
};

-- Game
Group[Game] ={
	id =Game,
	name ='Game',
	field ={
		{
			type ='int64',
			count=1,
			name ='Id',
			required =false,
			init_value =99,
			min_value =0,
			max_value =1000000,
		},
		{
			type ='string',
			count=1,
			name ='Name',
			required =false,
			init_value ="fool",
			min_length =0,
			max_length =100,
		},
		{
			type ='string',
			count=1,
			name ='Desc',
			required =false,
			init_value ="",
		},
	},
};

-- Human
Group[Human] ={
	id =Human,
	name ='Human',
	field ={
		{
			type ='int64',
			count=1,
			name ='Id',
			required =false,
			init_value =0,
		},
		{
			type ='string',
			count=1,
			name ='Name',
			required =false,
			init_value ="",
		},
		{
			type ='string',
			count=1,
			name ='Desc',
			required =false,
			init_value ="",
		},
		{
			type ='string',
			count=0,
			name ='Book',
			required =false,
			min_length =0,
			max_length =100,
		},
		{
			type ='Game',
			count=3,
			name ='Favorite',
			required =false,
			proto =Game,
		},
	},
};

