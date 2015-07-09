--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
-- prepare lang var --
local error =error
local print =print
local tostring =tostring
local load =load
local io =io
local table =table
local string =string
local package =package
local _G =_G

-- save package path --
local old_path =package.path
local old_cpath =package.cpath

-- set package path, for load core --
package.path =package.path .. ";../core/script/?.lua"
package.path =package.path .. ";../core/script/?.lua;../core/script/test/?.lua;../core/gen/protocol/script/?.lua;../core/gen/data/script/?.lua"
package.cpath =package.cpath .. ";../core/script/lib/?.so;../core/script/lib/test/?.so"

if LUA_SCRIPT_VERSION >= 520 then
	require =function(name)
		--// prepare var & func
		local module_prefix_content =[[
			local ___fool_love_winne_module___ =true
			local tostring =tostring
			local print =print
			local type =type
			local _G =_G
			local function module(name)
				if not _G[name] then
					_G[name] =_G[name] or {}
					local m =_G[name]
					m.M =m
					m._M =m
					m._MODULE_NAME =name
					___fool_love_winne_module___ =m;
				end
				_ENV =_G[name]
				-- print("new env " .. tostring(_ENV))
			end
		]];
		local module_subfix_content =[[
			if type(___fool_love_winne_module___) == 'table' then
				return ___fool_love_winne_module___;
			end
		]];
		local lang_require =require
		local function string_split(str, sep)
			local str_len =#str
			local sep_len =#sep
			local ls ={}
			local pos =1
			while pos <= str_len do
				local b, e =string.find(str, sep, pos, true)
				if b then
					if b > pos then
						table.insert(ls, string.sub(str, pos, b-1))
					end
					pos =e + 1
				else
					table.insert(ls, string.sub(str, pos, str_len))
					pos =str_len + 1
				end
			end
			return ls
		end
		--// try find from loaded table
		if _G[name] then
			return _G[name]
		end

		--// search
		local search_history =string.format("\tno field _G['%s']", name)
		local file_type
		local file_path
		local file_content
		-- try search from lua
		local path_ls =string_split(package.path, ";")
		for i=1, #path_ls do
			local p =string.gsub(path_ls[i], "?", name)
			local f =io.open(p, r)
			if f then
				file_type ="lua"
				file_path =p
				file_content =f:read("*all")
				io.close(f)
				if not file_content then
					error(string.format("fail to read from file '%s'", p))
				end
				break
			end
			search_history =search_history .. string.format("\n\tno file '%s'", p)
		end
		-- try search from c
		if not file_type then
			local cpath_ls =string_split(package.cpath, ";")
			for i=1, #cpath_ls do
				local p =string.gsub(cpath_ls[i], "?", name)
				local f =io.open(p, r)
				if f then
					io.close(f)
					file_type = "c"
					break
				end
				search_history =search_history .. '\n' .. p
			end
		end
		if not file_type then
			error(search_history)
			return nil
		end

		--// load
		if file_type == 'lua' then
			local mode ='t'
			if #file_content >= 4 then
				local b1, b2, b3, b4 = string.byte(file_content, 1, 4)
				if b1==0x1b and b2==0x4c and b3==0x75 and b4==0x61 then
					mode ='b'
				end
			end
			if mode == 't' then
				file_content =module_prefix_content .. file_content .. module_subfix_content
			end
			local fn, err =load(file_content, name .. ".lua" .. (mode=='t' and " line offset 17" or ''), mode)
			if err then
				error(err)
			end
			local m =fn() or true
			if not _G[name] then
				_G[name] =m
			end
		else
			local m =lang_require(name) or true
			if not _G[name] then
				_G[name] =m
			end
		end
		return _G[name]
	end
end
--[[
print("in core.lua _G is " .. tostring(_G))
print("in core.lua _ENV is " .. tostring(_ENV))
]]
-- load core --
require "core_setup"
-- print("in core.lua _ENV is " .. tostring(_ENV))
-- print("in core.lua Core is " .. tostring(Core))
require "core_info"
require "core_ids"
require "core_type"
require "core_util"
require "core_init"
require "core_protocol"
require "Logger"
require "Xml"
require "CharsetConvert"
require "Class"
require "Actor"
require "Parallel"
require "ProtocolManager"
require "StringManager"
require "Service"
require "Packet"
require "core_test"

-- restore package path --
package.path =old_path
package.cpath =old_cpath

-- call init --
Core.Init()
