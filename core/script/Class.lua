--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local setmetatable =setmetatable
local rawget =rawget
local table =table
local _G =_G
module "Core"

-- Class --
Class ={
	class_table ={}
}
function Class.DefClass(name)
	if not is_string(name) then
		local err ="fail to define class, invalid name"
		ERROR(err)
		return nil, err
	end
	return function(cls)
		-- check arg
		if not is_table(cls) then
			local err =sprintf("fail to define class %s, invalid class prototype", name)
			ERROR(err)
			return nil, err
		end
		
		-- check not exist
		if Class.class_table[name] then
			local err =sprintf("fail to define class %s, already exist", name)
			ERROR(err)
			return nil, err
		end

		-- set basic
		cls.class_name =name

		-- set super
		local super_name =cls.super
		cls.super =nil
		if is_string(super_name) then
			local super_cls =Class.class_table[super_name]
			-- check exist
			if not super_cls then
				local err =sprintf("fail to set class %s super, super %s not exist", name, super_name)
				ERROR(err)
				return nil, err
			end
			-- check loop
			local it =super_cls
			while it do
				if it.class_name == name then
					local err =sprintf("fail to set class %s super, loop", name)
					ERROR(err)
					return nil, err
				end
				it =it.super
			end
			-- link
			cls.super =super_cls
			setmetatable(cls, super_cls)
		elseif not is_nil(super_name) then
			local err =sprintf("fail to set class %s super, super is not a string or nil", name)
			ERROR(err)
			return nil, err
		end

		-- set init
		cls['$init'] ={};
		if cls.super then
			for i=1, #cls.super['$init'] do
				table.insert(cls['$init'], cls.super['$init'][i])
			end
		end
		local init =rawget(cls, 'init')
		if is_function(init) then
			table.insert(cls['$init'], init)
		elseif not is_nil(init) then
			local err =sprintf("fail to set class %s $init, init is not a function or nil", name)
			ERROR(err)
			return nil, err
		end

		-- set finalize
		cls['$finalize'] ={}
		if cls.super then
			for i=1, #cls.super['$finalize'] do
				table.insert(cls['$finalize'], cls.super['$finalize'][i])
			end
		end
		local finalize =rawget(cls, 'finalize')
		if is_function(finalize) then
			table.insert(cls['$finalize'], finalize)
		elseif not is_nil(finalize) then
			local err =sprintf("fail to set class %s $finalize, finalize is not a function or nil", name)
			ERROR(err)
			return nil, err
		end

		-- set metatable
		cls.__index =cls
		cls.__gc =function(obj)
			if obj['$instance_of'] then
				local finalize_ls =obj['$instance_of']['$finalize']
				for i=#finalize_ls, 1, -1 do
					local fn =finalize_ls[i]
					fn(obj)
				end
			end
		end

		-- register
		Class.class_table[name] =cls
		return true
	end
end
function Class.NewObject(name, ...)
	-- check arg
	if not is_string(name) then
		local err ="fail to new class instance, invalid class name"
		ERROR(err)
		return nil, err
	end
	if not Class.class_table[name] then
		local err =sprintf("fail to new class instance, calss %s not found", name)
		ERROR(err)
		return nil, err
	end

	-- new obj
	local cls =Class.class_table[name]
	local obj ={
		['$instance_of'] =cls
	};
	setmetatable(obj, cls)

	-- init
	local init_ls =cls['$init'];
	for i=1, #init_ls do
		local fn =init_ls[i]
		local result =fn(obj, ...)
		if is_boolean(result) and not result then
			local err =sprintf("fail to new class %s instance, call init #%d error", name, i)
			ERROR(err);
			return nil, err
		end
	end
	return obj
end

-- export --
_G['class'] =Class.DefClass
_G['new'] =Class.NewObject
export{
	Class =Class
}
