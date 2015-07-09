--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
module "Core"

UnitTest.Add(function()
	DEBUG("-- core_util checking --")

	-- deep copy --
	do
		local tb ={
			name ="fool",
			age =100,
			sex =true,
			address ={
				1, 2, 3, {"a", "b", "c"},
				street ="ghl"
			}
		}
		local clone =deep_copy(tb)
		tb.name ='winne'
		tb.address.street ='lhl'
		tb.address[2] =20

		assert(clone.name == 'fool')
		assert(clone.age == 100)
		assert(clone.sex == true)
		assert(clone.address[1] == 1)
		assert(clone.address[2] == 2)
		assert(clone.address[3] == 3)
		assert(clone.address[4][1] == 'a')
		assert(clone.address[4][2] == 'b')
		assert(clone.address[4][3] == 'c')
		assert(clone.address.street == 'ghl')

		assert(deep_copy(99) == 99)
		assert(deep_copy(false) == false)
		assert(deep_copy(true) == true)
		assert(deep_copy("a") == "a")
	end

	-- array concate --
	do
		local ls =array_concate({10, 20, 30}, nil, {40, 50, 60}, {70, 80, 90})
		assert(#ls == 9)
		for i=1, 9 do
			assert(ls[i] == i*10)
		end

		local ls =array_concate({10, 20, 30}, "", {40, 50, 60}, {70, 80, 90})
		assert(not ls)
	end

	-- time --
	do
		local str =time_to_string(1415661368)
		assert(str == "2014-11-11 07:16:08")
	end
end);

