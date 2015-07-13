--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local coroutine =coroutine
local pcall =pcall
local assert =assert
local string =string
local load =load
local table =table
local error =error
module "Core"

-- HtmlTemplate --
local g_func_template =[[
	local function f(param)
		local str_____ =''
		local function out(...)
			str_____ =str_____ .. string.format(...)
		end
		%s
		return str_____;
	end
return f
]];
local g_cache ={}
local function _calc_position(source, pos)
	local ln_num =1
	local ch_num =0
	for i=1, #source do
		ch_num =ch_num + 1
		if string.sub(source, i, i) == '\n' then
			ln_num =ln_num + 1
			ch_num =0
		end
		if i >= pos then
			break
		end
	end
	return ln_num, ch_num
end
local function compile(source, option)
	if g_cache[source] then
		return g_cache[source][1], g_cache[source][2]
	end
	local middle_code =''
	local cursor =1
	while cursor <= #source do
		-- find code begin
		local code_beg =string.find(source, "<#", cursor, true)
		if not code_beg then
			local pos =string.find(source, "#>", cursor, true)
			if pos then
				local ln, ch =_calc_position(source, pos)
				error(string.format("invalid syntax, unexpected `#>` near line %d, char %d", ln, ch))
			end
			local str =string.sub(source, cursor)
			middle_code =middle_code .. string.format("out('%s');\n", encode_quat_string(str))
			break
		end

		-- add text between cursor and code_beg-1
		if code_beg > cursor then
			local str =string.sub(source, cursor, code_beg-1)
			middle_code =middle_code .. string.format("out('%s');\n", encode_quat_string(str))
		end
		code_beg =code_beg + 2
		if code_beg > #source then
			local ln, ch =_calc_position(source, code_beg-2)
			error(string.format("invalid syntax, expected `#>` from line %d, char %d", ln, ch))
		end

		-- find code end
		local code_end =string.find(source, "#>", code_beg, true)
		if not code_end then
			local ln, ch =_calc_position(source, code_beg-2)
			error(string.format("invalid syntax, expected `#>` from line %d, char %d", ln, ch))
		end
		local pos =string.find(source, "<#", code_beg, true)
		if pos and pos<code_end then
			local ln, ch =_calc_position(source, pos)
			error(string.format("invalid syntax, unexpected `<#` near line %d, char %d", ln, ch))
		end

		-- output code
		if code_end > code_beg then
			local str =string.sub(source, code_beg, code_end-1)
			middle_code =middle_code .. '\n' .. str .. '\n'
		end

		-- cursor forward
		cursor =code_end + 2
	end

	-- generate func
	middle_code =string.gsub(middle_code, "&lt;", "<")
	middle_code =string.gsub(middle_code, "&gt;", ">")
	local fn, err =load(string.format(g_func_template, middle_code)) 
	assert(fn, err)
	fn =fn()
	
	-- cache
	g_cache[source] ={ fn, middle_code }

	-- return
	return fn, middle_code
end

local function render(fn, param)
	return fn(param)
end

local function run(source, param, option)
	local fn =compile(source, option)
	return render(fn, param)
end

HtmlTemplate ={
	compile =compile,
	render =render,
	run =run,
};
XmlTemplate =HtmlTemplate
