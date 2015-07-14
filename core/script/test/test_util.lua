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

	-- string_split --
	do
		local str ="/usr/bin/ls"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='usr' and ls[2]=='bin' and ls[2]=='ls')

		local str ="/user/setinfo/basic.lua"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='user' and ls[2]=='setinfo' and ls[2]=='basic.lua')

		local str ="user/setinfo/basic.lua"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='user' and ls[2]=='setinfo' and ls[2]=='basic.lua')

		local str ="user/setinfo/basic.lua/"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='user' and ls[2]=='setinfo' and ls[2]=='basic.lua')

		local str ="/user/setinfo/basic.lua/"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='user' and ls[2]=='setinfo' and ls[2]=='basic.lua')

		local str ="/user///setinfo/basic.lua/"
		local ls =string_split(str, "/", true)
		assert(#ls == 3 and ls[1]=='user' and ls[2]=='setinfo' and ls[2]=='basic.lua')
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

	-- table merge --
	do
		local tb1 ={ a =8, b =9, 1, 2, 3 }
		local tb2 ={ c =80, d =90, 10, 20, 30, 40, 50, 60 }
		local tb3 =table_merge(tb1, tb2)
		assert(tb3.a == 8)
		assert(tb3.b == 9)
		assert(tb3.c == 80)
		assert(tb3.d == 90)

		assert(tb3[1] == 10)
		assert(tb3[2] == 20)
		assert(tb3[3] == 30)
		assert(tb3[4] == 40)
		assert(tb3[5] == 50)
		assert(tb3[6] == 60)
		for k, v in pairs(tb3) do
			assert( k=='a' or k=='b' or k=='c' or k=='d' or k==1 or k==2 or k==3 or k==4 or k==5 or k==6 );
		end
	end
end);
