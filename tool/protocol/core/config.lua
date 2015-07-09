module "config" 
config ={
	Id =1,
	Namespace ={"core", "protocol"},
	OutputName ='core_protocol',
	OutputPath ='../../../core/gen/protocol',
	IncludeFile ="../../core.h",
}
return config
