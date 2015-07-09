module "config" 
config ={
	Id =1,
	Namespace ={"core", "cpplua"},
	OutputName ='core_cpplua',
	OutputPath ='../../../core/gen/cpplua',
	LuaModuleName ="Core",
	IncludeFile ="../../core.h",
}
return config
