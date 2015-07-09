module "config" 
config ={
	Id =1,
	Namespace ={"cpplua"},
	OutputName ='cpplua',
	OutputPath ='../../../gen/cpplua',
	LuaModuleName ="CppLua",
	IncludeFile ="../../core/core.h",
}
return config
