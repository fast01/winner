module 'core_protocol'
-- group --
GroupId =1;
Group ={};

-- protocol ids --
ActorConfig =1;
ActorManagerConfig =2;
DataServiceConfig =3;
LogServiceConfig =4;
LogTargetConfig =5;
MysqlConfig =6;
NetworkConfig =7;
NetworkConnectionConfig =8;
NetworkListenerConfig =9;
NetworkRouteConfig =10;
RedisConfig =11;
ServiceConfig =12;
ServiceManagerConfig =13;

-- protocol list --
-- ActorConfig
Group[ActorConfig] ={
	id =ActorConfig,
	name ='ActorConfig',
	field ={
		{
			type ='string',
			count=1,
			name ='TemplateName',
			required =false,
			init_value ="",
		},
		{
			type ='string',
			count=1,
			name ='ClassName',
			required =false,
			init_value ="",
		},
		{
			type ='string',
			count=0,
			name ='Component',
			required =false,
		},
		{
			type ='string',
			count=0,
			name ='Observer',
			required =false,
		},
	},
};

-- ActorManagerConfig
Group[ActorManagerConfig] ={
	id =ActorManagerConfig,
	name ='ActorManagerConfig',
	field ={
		{
			type ='ActorConfig',
			count=0,
			name ='Actor',
			required =false,
			proto =ActorConfig,
		},
	},
};

-- DataServiceConfig
Group[DataServiceConfig] ={
	id =DataServiceConfig,
	name ='DataServiceConfig',
	field ={
		{
			type ='MysqlConfig',
			count=1,
			name ='Mysql',
			required =false,
			proto =MysqlConfig,
		},
		{
			type ='RedisConfig',
			count=1,
			name ='Redis',
			required =false,
			proto =RedisConfig,
		},
	},
};

-- LogServiceConfig
Group[LogServiceConfig] ={
	id =LogServiceConfig,
	name ='LogServiceConfig',
	field ={
		{
			type ='LogTargetConfig',
			count=0,
			name ='Target',
			required =false,
			proto =LogTargetConfig,
		},
	},
};

-- LogTargetConfig
Group[LogTargetConfig] ={
	id =LogTargetConfig,
	name ='LogTargetConfig',
	field ={
		{
			type ='int64',
			count=1,
			name ='Type',
			required =false,
			init_value =0,
			min_value =1,
		},
		{
			type ='string',
			count=1,
			name ='FileName',
			required =false,
			init_value ="",
			min_length =1,
		},
		{
			type ='int64',
			count=1,
			name ='FileLifeTime',
			required =false,
			init_value =1800,
			min_value =60,
		},
		{
			type ='int64',
			count=1,
			name ='FlushMode',
			required =false,
			init_value =0,
			min_value =0,
		},
	},
};

-- MysqlConfig
Group[MysqlConfig] ={
	id =MysqlConfig,
	name ='MysqlConfig',
	field ={
		{
			type ='string',
			count=1,
			name ='Host',
			required =false,
			init_value ="",
		},
		{
			type ='int64',
			count=1,
			name ='Port',
			required =false,
			init_value =0,
		},
		{
			type ='string',
			count=1,
			name ='User',
			required =false,
			init_value ="",
		},
		{
			type ='string',
			count=1,
			name ='Passwd',
			required =false,
			init_value ="",
		},
	},
};

-- NetworkConfig
Group[NetworkConfig] ={
	id =NetworkConfig,
	name ='NetworkConfig',
	field ={
		{
			type ='int64',
			count=1,
			name ='SlaveCount',
			required =false,
			init_value =0,
		},
		{
			type ='NetworkListenerConfig',
			count=0,
			name ='Listener',
			required =false,
			proto =NetworkListenerConfig,
		},
		{
			type ='NetworkConnectionConfig',
			count=0,
			name ='Remote',
			required =false,
			proto =NetworkConnectionConfig,
		},
		{
			type ='NetworkRouteConfig',
			count=0,
			name ='Route',
			required =false,
			proto =NetworkRouteConfig,
		},
	},
};

-- NetworkConnectionConfig
Group[NetworkConnectionConfig] ={
	id =NetworkConnectionConfig,
	name ='NetworkConnectionConfig',
	field ={
		{
			type ='string',
			count=1,
			name ='Type',
			required =false,
			init_value ="",
		},
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
			name ='Host',
			required =false,
			init_value ="localhost",
		},
		{
			type ='int32',
			count=1,
			name ='Port',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='RunnerBegin',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='RunnerRange',
			required =false,
			init_value =10000000,
		},
	},
};

-- NetworkListenerConfig
Group[NetworkListenerConfig] ={
	id =NetworkListenerConfig,
	name ='NetworkListenerConfig',
	field ={
		{
			type ='string',
			count=1,
			name ='Type',
			required =false,
			init_value ="",
		},
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
			name ='Path',
			required =false,
			init_value ="localhost",
		},
		{
			type ='int32',
			count=1,
			name ='Port',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='DeliverBegin',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='DeliverRange',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='RunnerBegin',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='RunnerRange',
			required =false,
			init_value =10000000,
		},
	},
};

-- NetworkRouteConfig
Group[NetworkRouteConfig] ={
	id =NetworkRouteConfig,
	name ='NetworkRouteConfig',
	field ={
		{
			type ='int64',
			count=1,
			name ='IdService',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='IdBegin',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='IdEnd',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Algrithm',
			required =false,
			init_value =0,
		},
	},
};

-- RedisConfig
Group[RedisConfig] ={
	id =RedisConfig,
	name ='RedisConfig',
	field ={
		{
			type ='string',
			count=1,
			name ='Host',
			required =false,
			init_value ="",
		},
		{
			type ='int64',
			count=1,
			name ='Port',
			required =false,
			init_value =0,
		},
	},
};

-- ServiceConfig
Group[ServiceConfig] ={
	id =ServiceConfig,
	name ='ServiceConfig',
	field ={
		{
			type ='int64',
			count=1,
			name ='IdService',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='IdBegin',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='IdEnd',
			required =false,
			init_value =0,
		},
		{
			type ='int64',
			count=1,
			name ='Algrithm',
			required =false,
			init_value =1,
		},
		{
			type ='string',
			count=1,
			name ='Path',
			required =false,
			init_value ="",
		},
	},
};

-- ServiceManagerConfig
Group[ServiceManagerConfig] ={
	id =ServiceManagerConfig,
	name ='ServiceManagerConfig',
	field ={
		{
			type ='ServiceConfig',
			count=0,
			name ='Service',
			required =false,
			proto =ServiceConfig,
		},
	},
};

