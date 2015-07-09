--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- Mysql checking --")

	local mysql =Mysql.New()
	-- local ok, err =mysql:connect("root", "fool_winne", "localhost", 3306)
	local ok, err =mysql:connect("root", "fool_winne", "/var/run/mysqld/mysqld.sock")
	assert(ok, err)
	
	-- select db --
	local ok, err =mysql:exec("USE game")
	assert(ok, err)

	-- drop table --
	local ok, err =mysql:exec("DROP TABLE IF EXISTS test_mysql_for_lua")
	assert(ok, err)

	-- create table --
	local ok, err =mysql:exec("CREATE TABLE `test_mysql_for_lua`(id BIGINT NOT NULL AUTO_INCREMENT, name VARCHAR(64), `desc` CHAR(64), age INT, `createtime` TINYINT, PRIMARY KEY(id))")
	assert(ok, err)

	-- insert --
	local ok, err =mysql:exec("INSERT INTO `test_mysql_for_lua`(name, `desc`, age, createtime)VALUES('fool', 'winne', 99, 100),('winne','fool',999,10)")
	assert(ok, err)
	local num, err =mysql:lastInsertId()
	assert(num and num==1, err)

	-- update --
	local ok, err =mysql:exec("UPDATE `test_mysql_for_lua` SET age=1000 WHERE name='fool'")
	assert(ok, err)
	local num, err =mysql:affectedRows()
	assert(num and num==1, err)

	local ok, err =mysql:exec("UPDATE `test_mysql_for_lua` SET age=1000 WHERE name='foo'")
	assert(ok, err)
	local num, err =mysql:affectedRows()
	assert(num and num==0, err)

	-- query --
	local rows, err =mysql:query("SELECT * FROM test_mysql_for_lua")
	assert(rows, err)
	assert(#rows == 2)
	assert(rows[1].name == 'fool')
	assert(rows[2].name == 'winne')

	assert(rows[1].desc == 'winne')
	assert(rows[2].desc == 'fool')

	assert(rows[1].age == 1000)
	assert(rows[2].age == 999)

	assert(rows[1].createtime == 100)
	assert(rows[2].createtime == 10)

	for i=1, 2 do
		local row =rows[i]
		for k, v in pairs(row) do
			assert(k=='id' or k=='name' or k=='desc' or k=='age' or k=='createtime', 'unexpected field ' .. k)
		end
	end
end);
