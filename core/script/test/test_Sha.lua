--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
module "Core"

UnitTest.Add(function()
	DEBUG("-- Sha checking --")
	local text ="fool love winne";

	local code =Sha.Sum(Sha.SHA1, text);
	assert(code == '3bf12598a43022c8974fd994ca88b970c4f51329')

	local code =Sha.Sum(Sha.SHA224, text);
	assert(code == '9a51197fa327644a4e947edece71fc35ac2b8785513ffbf832f08f16')

	local code =Sha.Sum(Sha.SHA256, text);
	assert(code == 'ce7170ce9a0fdc867a5f632d4340dc6429f532109f3a6bc110b45c501eb9eea0')

	local code =Sha.Sum(Sha.SHA384, text);
	assert(code == '9135a6e76bd6e2da641825aa01af4e93274055603ef9fd9b7b8b6279992d160afce2ddf3e40604d1dd7b531041b13b93')

	local code =Sha.Sum(Sha.SHA512, text);
	assert(code == '90a4ae59151641f6d08f21db49e4ec8f539293f635c1935fc9f8d78c1f9f864ce041a7deabf3112182aaef3e2c777f35f8ef81fd5ea52a0b848b9b54470a3b4e')

end);
