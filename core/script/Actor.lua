--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local setmetatable =setmetatable
local pairs =pairs
local tostring =tostring
local table =table
local _G =_G
module "Core"

-- helper --
local function get_actor_template(actor_name)
	Actor.template_table[actor_name] =Actor.template_table[actor_name] or {
		name =actor_name,
		component ={},
	};
	return Actor.template_table[actor_name]
end
local function get_actor_from_component(com)
	return com.actor
end
local function get_actor_component(actor, com_name)
	-- check arg
	if not is_table(actor) or not is_table(actor.template) then
		ERROR("fail to get actor component, invalid actor")
		return nil
	end
	if not is_string(com_name) then
		ERROR("fail to get actor %s component, invalid com_name", actor.template.name)
		return nil
	end
	-- check destroyed
	if actor:isDestroyed() then
		ERROR("fail to get actor %s component, actor destroyed", actor.template.name)
		return nil
	end
	-- just return if already exist
	if actor.component[com_name] then
		return actor.component[com_name]
	end
	-- prepare component template
	local actor_template =actor.template
	local com_template =actor_template.component[com_name]
	if not com_template then
		ERROR("fail to get actor component, actor %s not have component %s", actor_template.name, com_name)
		return nil
	end
	-- create component instance
	local com_instance ={};
	for k, v in pairs(com_template) do
		if is_string(k) then
			com_instance[k] =deep_copy(v)
		end
	end
	com_instance.template =com_template
	com_instance.actor =actor
	if com_instance.init then
		com_instance:init()
	end
	-- add component
	actor.loaded_component =actor.loaded_component or {}
	actor.component[com_name] =com_instance
	table.insert(actor.loaded_component, com_instance)
	return com_instance
end
local function is_actor_destroyed(actor)
	return actor["$destroyed"]
end
local function destroy_actor(actor)
	-- check arg
	if not is_table(actor) then
		ERROR("fail to destroy actor, invalid actor")
		return nil
	end
	-- set destory flag
	if is_actor_destroyed(actor) then
		WARN("Actor %s try destroy much times", actor.template.name)
		return nil
	end
	actor['$destroyed'] =true
	-- clean component
	if actor.loaded_component then
		local ls ={}
		for i=1, #actor.loaded_component do
			local com =actor.loaded_component[i]
			if com.finalize then
				table.insert(ls, com)
			end
		end
		for i=#ls, 1, -1 do
			local com =ls[i]
			com:finalize()
		end
	end
	-- clean self
	if actor.template.finalize then
		actor.template.finalize(actor)
	end
	return true
end

-- Actor --
Actor ={
	template_table ={},
	Register=function(actor_name, init, finalize)
		-- check arg
		if not is_string(actor_name) then
			ERROR("fail to register actor, invalid actor_name")
			return false
		end
		if not is_function(init) and not is_nil(init) then
			ERROR("fail to register actor, invalid init")
			return false
		end
		if not is_function(finalize) and not is_nil(finalize) then
			ERROR("fail to register actor, invalid finalize")
			return false
		end
		-- prepare actor template
		local actor_template =get_actor_template(actor_name)
		-- set init finalize
		actor_template.init =init
		actor_template.finalize =finalize
		return true
	end,
	RegisterComponent =function(actor_name, com_name, com)
		-- check arg
		if not is_string(actor_name) then
			ERROR("fail to register actor component, invalid actor_name")
			return false
		end
		if not is_string(com_name) then
			ERROR("fail to register actor component, invalid com_name")
			return false
		end
		if not is_table(com) then
			ERROR("fail to register actor component, invalid com")
			return false
		end
		if not is_function(com.init) and not is_nil(com.init) then
			ERROR("fail to register actor component, invalid init")
			return false
		end
		if not is_function(com.finalize) and not is_nil(com.finalize) then
			ERROR("fail to register actor component, invalid finalize")
			return false
		end
		-- prepare actor template
		local actor_template =get_actor_template(actor_name)
		if actor_template.component[com_name] then
			WARN("register actor component, actor %s duplicate component %s", actor_name, com_name)
			return false
		end
		-- set component
		com.name =com_name
		com.getActor =get_actor_from_component
		-- add component
		actor_template.component[com_name] =com
		return true
	end,
	New =function(actor_name, not_use_gc)
		-- check arg
		if not is_string(actor_name) then
			ERROR("fail to new actor instance, invalid actor_name")
			return nil
		end
		-- get actor
		local actor_template =Actor.template_table[actor_name]
		if not actor_template then
			ERROR("fail to new actor instance, actor template %s not exist", actor_name)
			return nil
		end
		-- new instance
		local instance ={
			template =actor_template,
			component ={},
			getComponent =get_actor_component,
			isDestroyed =is_actor_destroyed,
			destroy =destroy_actor
		};
		if actor_template.init then
			actor_template.init(instance)
		end
		if not not_use_gc then
			setmetatable(instance, {
				__gc =destroy_actor
			});
		end
		return instance
	end,
	NewManager =function(actor_name)
		-- check arg
		if not is_string(actor_name) then
			ERROR("fail to new actor manager, invalid actor_name")
			return nil
		end
		-- get actor template
		local actor_template =Actor.template_table[actor_name]
		if not actor_template then
			ERROR("fail to new actor manager , actor template %s not exist", actor_name)
			return nil
		end
		-- new manager
		local manager ={
			template =actor_template,
			actor_table ={},
			foreach =function(self, fn)
				-- check arg
				if not is_function(fn) then
					ERROR("fail to foreach actor %s component, fn invalid", self.template.name)
					return nil
				end
				-- foreach
				local ls ={}
				for k, v in pairs(self.actor_table) do
					table.insert(ls, v)
				end
				for i=1, #ls do
					fn(ls[i])
				end
				return true
			end,
			count =function(self)
				local cnt =0
				for k, v in pairs(self.actor_table) do
					cnt =cnt + 1
				end
				return cnt
			end,
			all =function(self)
				return self.actor_table
			end,
			get =function(self, actor_id)
				-- check arg
				if not is_number(actor_id) then
					ERROR("fail to get actor %s component, actor_id invalid", self.template.name)
					return nil
				end
				-- prepare actor instance
				if self.actor_table[actor_id] then
					return self.actor_table[actor_id]
				else
					local actor_instance =Actor.New(self.template.name, true)
					actor_instance.id =actor_id
					self.actor_table[actor_id] =actor_instance
					return actor_instance
				end
			end,
			getComponent =function(self, actor_id, com_name)
				local actor_instance =self:get(actor_id)
				if actor_instance then
					return actor_instance:getComponent(com_name)
				else
					return nil
				end
			end,
			destroy =function(self, actor_id)
				-- check arg
				if not is_number(actor_id) then
					ERROR("fail to destroy actor %s, actor_id invalid", self.template.name)
					return nil
				end
				-- destroy
				local actor_instance =self.actor_table[actor_id]
				if actor_instance then
					self.actor_table[actor_id] =nil
					return actor_instance:destroy()
				else
					WARN("fail to destroy actor %s, actor_id %d not exist", self.template.name, actor_id)
					return nil
				end
			end,
			destroyAll =function(self)
				local tb =self.actor_table
				self.actor_table ={}
				for k, v in pairs(tb) do
					v:destroy()
				end
			end,
		};
		return manager
	end
};
