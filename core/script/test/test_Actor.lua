--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local collectgarbage =collectgarbage
local assert =assert
local pairs =pairs
local print =print
local debug =debug
local class =class
local coroutine =coroutine
local new =new
module "Core"

UnitTest.Add(function()
	DEBUG("-- Actor checking --")
	Actor.Register("Npc",
		function(self)
			print(sprintf("init Actor %s", tostring(self)))
		end,
		function(self)
			print(sprintf("finalize Actor %s", tostring(self)))
		end	
	)
	Actor.RegisterComponent("Npc", "Weapon", {
		atk =99,
		range =100,
		setAtk =function(self, v)
			self.atk =v
		end,
		init =function(self)
			print(sprintf("init Weapon %s", tostring(self)))
		end,
		finalize =function(self)
			print(sprintf("finalize Weapon %s", tostring(self)))
		end,
	});
	Actor.RegisterComponent("Npc", "Armor", {
		defend =99,
		range =100,
		init =function(self)
			print(sprintf("init Armor %s", tostring(self)))
		end,
		finalize =function(self)
			print(sprintf("finalize Armor %s", tostring(self)))
		end,
	});
	Actor.RegisterComponent("Player", "Basic", {
		hp =99,
		mp =100,
		init =function(self)
			print(sprintf("init Player Basic %s", tostring(self)))
		end,
		finalize =function(self)
			print(sprintf("finalize Player Basic %s", tostring(self)))
		end,
	});
	Actor.RegisterComponent("Player", "Skill", {
		type ="jump",
		value =101,
		init =function(self)
			print(sprintf("init Player Skill %s", tostring(self)))
		end,
		finalize =function(self)
			print(sprintf("finalize Player Skill %s", tostring(self)))
		end,
	});

--[[
	local npc =Actor.New("Npc")
	local weapon =npc:getComponent("Weapon")
	local armor =npc:getComponent("Armor")
	
	print(sprint_table(Actor, { show_func =nil }))
	print(sprint_table(npc, { show_func =nil }))
	print(sprint_table(weapon, { show_func =nil }))
	print(sprint_table(armor, { show_func =nil }))
	weapon.range=1000
	weapon:setAtk(198)
	print(weapon.range)
	print(weapon.atk)
	print(weapon.actor)
	print(weapon:getActor())
	npc =nil
	weapon =nil
	armor =nil
	collectgarbage("collect")

	local mgr =Actor.NewManager("Npc")
	local armor =mgr:getComponent(1, "Armor")
	local weapon =mgr:getComponent(1, "Weapon")
	local npc =mgr:get(1)
	print(sprint_table(npc, { show_func =nil }))
	print(sprint_table(weapon, { show_func =nil }))
	mgr:destroyAll()
	npc =nil
	weapon =nil
	armor =nil
	mgr =nil
	collectgarbage("collect")
]]
end);
