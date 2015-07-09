--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local assert =assert
local pairs =pairs
local print =print
local debug =debug
module "Core"

UnitTest.Add(function()
	DEBUG("-- Xml checking --")
	local text =[[
		<root type="man" status="good">
			<name>fool</name>
			<age>100</age>
			<books count="3">
				<book>c</book>
				<book>c++</book>
				<book>111</book>
			</books>
		</root>
	]]

	local tb, err =Xml.Decode(text);
	assert(tb, err)
	-- DEBUG(sprint_table(tb))

	local text1, err =Xml.Encode(tb);
	assert(text1, err)
	-- DEBUG(text1)

	local tb, err =Xml.Decode(text1);
	assert(tb, err)

	assert(tb.tag == 'root')
	assert(tb.child[3].text == nil)
	assert(tb.attr.type == 'man')
	assert(tb.attr.status == 'good')
	assert(#tb.child == 3)

	assert(tb.child[1].tag == 'name')
	assert(tb.child[1].text == 'fool')
	assert(#tb.child[1].child == 0)

	assert(tb.child[2].tag == 'age')
	assert(tb.child[2].text == '100')
	assert(#tb.child[2].child == 0)

	assert(tb.child[3].tag == 'books')
	assert(tb.child[3].text == nil)
	assert(#tb.child[3].child == 3)
	assert(tb.child[3].attr.count == '3')

	assert(tb.child[3].child[1].tag == 'book')
	assert(tb.child[3].child[1].text == 'c')
	assert(#tb.child[3].child[1].child == 0)

	assert(tb.child[3].child[2].tag == 'book')
	assert(tb.child[3].child[2].text == 'c++')
	assert(#tb.child[3].child[2].child == 0)

	assert(tb.child[3].child[3].tag == 'book')
	assert(tb.child[3].child[3].text == '111')
	assert(#tb.child[3].child[3].child == 0)
end);
