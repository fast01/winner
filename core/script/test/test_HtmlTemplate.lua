--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- HtmlTemplate checking --")
	do return; end
	-- simple --
	local source =[[
	<html>
		<head>
			<title> hello </title>
		</head>
		<body>
			hahahahahha
		</body>
	</html>
	]]
	print(HtmlTemplate.run(source))

	-- with code --
	local source =[[
	<html>
		<head>
			<title> hello </title>
		</head>
		<body>
			<#
				out("hello world\n")
			#>
		</body>
	</html>
	]]
	print(HtmlTemplate.run(source))

	-- with code and param --
	local source =[[
	<html>
		<head>
			<title> hello </title>
		</head>
		<body>
			<#
				out("hello %s", param.name)
			#>
		</body>
	</html>
	]]
	print(HtmlTemplate.run(source, { name ='fool' }))

	-- with more code block and param --
	local source =[[
	<html>
		<head>
			<title> hello </title>
		</head>
		<body>
			<#
				local x=9
				for i=1, 3 do
			#>
			<#
				out("hello #%d %s", i, param.name)
			#>
			<#
				end
				if not GLOBAL_NUM then
					GLOBAL_NUM =99
				end
				GLOBAL_NUM =GLOBAL_NUM + 1
				out("%d\n", GLOBAL_NUM)
				out("%d\n", x)
			#>
		</body>
	</html>
	]]
	local fn, code =HtmlTemplate.compile(source, {return_middle_code =true})
	-- print(code)
	print(fn({ name ='fool' }))
	print(fn({ name ='fool' }))
end);
