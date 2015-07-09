--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local collectgarbage =collectgarbage
local assert =assert
local pairs =pairs
local print =print
local debug =debug
local class =class
local new =new
module "Core"

UnitTest.Add(function()
	DEBUG("-- Class checking --")
	--[[
	print("-- Basic --")
	class "Basic" {
		init =function(self, value)
			print('Basic init')
			self.age =value
			self.name ='basic instance'
		end,
		finalize =function(self)
			print('Basic finalize')
		end,
		show =function(self)
			print("Basic.show", self.age)
		end
	};
	local ins =new("Basic", 100);
	ins:show()
	ins =nil
	collectgarbage("collect")

	-- Sub 1
	print("-- Sub1 --")
	class "Sub1" {
		super ='Basic',
		init =function(self, value)
			print('Sub1 init')
		end,
		finalize =function(self)
			print('Sub1 finalize')
		end,
	};
	local ins =new("Sub1", 101);
	ins:show()
	ins =nil
	collectgarbage("collect")

	-- Sub 2
	print("-- Sub2 --")
	class "Sub2" {
		super ='Sub1',
		show =function(self)
			print("Sub2.show", self.age)
		end
	}
	local ins =new("Sub2", 102);
	ins:show()
	ins =nil
	collectgarbage("collect")

	-- Sub 3
	print("-- Sub3 --")
	class "Sub3" {
		super ="Sub2",
		init =function(self, value)
			print('Sub3 init')
		end,
		finalize =function(self)
			print('Sub3 finalize')
		end,
	};
	local ins =new("Sub3", 103);
	ins:show()
	ins =nil
	collectgarbage("collect")
	]]
end);

