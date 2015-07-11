--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local string =string
local math =math
local table =table
local type =type
local print =print
local pairs =pairs
local debug =debug
module "Core"

-- prepare type trait --
local type_trait ={
	boolean ={
		decode ="decodeBoolean",
		encode ="encodeBoolean",
		init_value =false,
	},
	string ={
		decode ="decodeString",
		encode ="encodeString",
		init_value ="",
	},
	float32 ={
		decode ="decodeFloat32",
		encode ="encodeFloat32",
		init_value =0,
	},
	float64 ={
		decode ="decodeFloat64",
		encode ="encodeFloat64",
		init_value =0,
	},
};
local bits =8
for i=1, 4 do
	type_trait["int" .. bits] ={
		decode ="decodeInt64",
		encode ="encodeInt64",
		init_value =0,
	};
	type_trait["uint" .. bits] ={
		decode ="decodeUint64",
		encode ="encodeUint64",
		init_value =0,
	};
	bits =bits * 2
end

-- group table --
local g_group_table ={}

-- encode, decode --
function encode_protocol(group_id, proto_id, msg, bs)
	if not bs then
		bs =Bytes.New()
	end
	-- check arg
	if not is_number(group_id) then
		return nil, string.format("fail to encode protocol, invalid group_id")
	end
	if not is_number(proto_id) then
		return nil, string.format("fail to encode protocol, invalid proto_id")
	end
	if not is_userdata(bs) then
		return nil, string.format("fail to encode protocol %d.%d, invalid bs", group_id, proto_id)
	end
	local proto =g_group_table[group_id] and g_group_table[group_id][proto_id]
	if not proto then
		return nil, string.format("fail to encode protocol %d.%d, protocol not found", group_id, proto_id)
	end

	-- encode 0 if nil
	if not msg then
		bs:encodeUint32(0)
		return bs
	end
	if not is_table(msg) then
		return nil, string.format("fail to encode protocol %s(%d.%d), msg is not a table or nil", proto.name, group_id, proto_id)
	end

	-- encode msg
	local write_cursor_beg =bs:getWriteCursor()
	bs:writeUint32(0)
	for i=1, #proto.field do
		local field =proto.field[i]
		local field_name =field.name
		local field_value =msg[field_name] or field.init_value
		local trait =type_trait[field.type]
		local encode_fn =trait and trait.encode

		if field.count == 1 then
			if is_nil(field_value) then
				return nil, string.format("fail to encode protocol %s(%d.%d), field_value is nil", proto.name, group_id, proto_id)
			end
			if trait then
				bs[encode_fn](bs, field_value)
			else
				local _, err =encode_protocol(group_id, field.protocol_id, field_value, bs);
				if err then
					return nil, err
				end
			end
		elseif field.count > 1 then
			local items =field_value
			if type(items) ~= 'table' or #items~=field.count then
				return nil, string.format("fail to encode protocol %s(%d.%d), field %s is not a table which length is %d", proto.name, group_id, proto_id, field_name, field.count)
			end
			for j=1, field.count do
				if trait then
					bs[encode_fn](bs, items[j])
				else
					local _, err =encode_protocol(group_id, field.protocol_id, items[j], bs);
					if err then
						return nil, err
					end
				end
			end
		else
			local items =field_value
			if type(items) ~= 'table' then
				return nil, string.format("fail to encode protocol %s(%d.%d), field %s is not a table", proto.name, group_id, proto_id, field_name)
			end
			local count =#items
			bs:encodeInt64(count)
			for j=1, count do
				if trait then
					bs[encode_fn](bs, items[j])
				else
					local _, err =encode_protocol(group_id, field.protocol_id, items[j], bs);
					if err then
						return nil, err
					end
				end
			end
		end
	end

	-- reset length
	local write_cursor_end =bs:getWriteCursor()
	local length =write_cursor_end - write_cursor_beg
	length =Endian.Host2NetU32(length)
	bs:setWriteCursor(write_cursor_beg)
	bs:writeUint32(length)
	bs:setWriteCursor(write_cursor_end)

	return bs
end
function decode_protocol(group_id, proto_id, bs)
	-- check arg
	if not is_number(group_id) then
		return nil, string.format("fail to decode protocol, invalid group_id")
	end
	if not is_number(proto_id) then
		return nil, string.format("fail to decode protocol, invalid proto_id")
	end
	if not is_userdata(bs) then
		return nil, string.format("fail to decode protocol %d.%d, invalid bs", group_id, proto_id)
	end
	local proto =g_group_table[group_id] and g_group_table[group_id][proto_id]
	if not proto then
		return nil, string.format("fail to decode protocol %d.%d, protocol not found", group_id, proto_id)
	end

	-- read length
	local read_cursor_beg =bs:getReadCursor()
	local length =bs:readUint32()
	if bs:bad() then
		return nil, string.format("fail to decode protocol %s(%d.%d), fail to read length", proto.name, group_id, proto_id)
	end
	length =Endian.Net2HostU32(length)
	if length == 0 then
		return nil
	end

	-- read field
	local just_init =false
	local msg ={}
	for i=1, #proto.field do
		local field =proto.field[i]
		local trait =type_trait[field.type]
		local decode_fn =trait and trait.decode

		if just_init then
			if field.is_build_in then
				if field.count == 1 then
					msg[field.name] =field.init_value
				else
					msg[field.name] =deep_copy(field.init_value)
				end
			end
		else
			if field.count == 1 then
				if trait then
					msg[field.name] =bs[decode_fn](bs)
				else
					local item, err =decode_protocol(group_id, field.protocol_id, bs)
					if err then
						return nil, err
					end
					msg[field.name] =item
				end
				if bs:bad() then
					return nil, string.format("fail to decode protocol %s(%d.%d), decode %s error", proto.name, group_id, proto_id, field.name)
				end
			elseif field.count > 1 then
				local ls ={}
				for j=1, field.count do
					if trait then
						table.insert(ls, bs[decode_fn](bs))
					else
						local item, err =decode_protocol(group_id, field.protocol_id, bs)
						if err then
							return nil, err
						end
						table.insert(ls, item)
					end
					if bs:bad() then
						return nil, string.format("fail to decode protocol %s(%d.%d), decode %s error", proto.name, group_id, proto_id, field.name)
					end
				end
				msg[field.name] =ls
			else
				local ls ={}
				local count =bs:decodeInt64()
				if bs:bad() then
					return nil, string.format("fail to decode protocol %s(%d.%d), decode count(%s) error", proto.name, group_id, proto_id, field.name)
				end
				for j=1, count do
					if trait then
						table.insert(ls, bs[decode_fn](bs))
					else
						local item, err =decode_protocol(group_id, field.protocol_id, bs)
						if err then
							return nil, err
						end
						table.insert(ls, item)
					end
					if bs:bad() then
						return nil, string.format("fail to decode protocol %s(%d.%d), decode %s error", proto.name, group_id, proto_id, field.name)
					end
				end
				msg[field.name] =ls
			end

			-- check cursor
			local read_cursor_cur =bs:getReadCursor()
			local read_cnt =read_cursor_cur - read_cursor_beg
			if read_cnt == length then
				just_init =true
			elseif read_cnt > length then
				return nil, string.format("fail to decode protocol %s(%d.%d), read too much bytes when decode field %s", proto.name, group_id, proto_id, field.name)
			end
		end
	end

	-- skip remaind
	local read_cursor_end =bs:getReadCursor()
	local read_cnt =read_cursor_end - read_cursor_beg
	if read_cnt < length then
		bs:skip(length - read_cnt)
	end
	return msg
end

function check(msg, proto, group_id, protocol_id)
	if type(proto) ~= 'table' then
		return false, string.format("fail to check protocol %d.%d, proto is not a table", group_id, protocol_id)
	end
	if type(msg) ~= 'table' then
		return false, string.format("fail to check protocol %s(%d.%d), msg is not a table", proto.name, group_id, protocol_id)
	end
	local function check_value(v, field)
		if field.is_build_in then
			if field.min_value then
				if not is_number(v) or v < field.min_value then
					return false, string.format("%s invalid, min value is %d, got %s", field.name, field.min_value, tostring(v))
				end
			end
			if field.max_value then
				if not is_number(v) or v > field.max_value then
					return false, string.format("%s invalid, max value is %d, got %s", field.name, field.max_value, tostring(v))
				end
			end
			if field.min_length then
				if not is_string(v) or #v < field.min_length then
					return false, string.format("%s invalid, min length is %d", field.name, field.min_length)
				end
			end
			if field.max_length then
				if not is_string(v) or #v > field.max_length then
					return false, string.format("%s invalid, max length is %d", field.name, field.max_length)
				end
			end
		else
			return check(v, g_group_table[group_id][field.protocol_id], group_id, field.protocol_id)
		end
		return true
	end
	for i=1, #proto.field do
		local field =proto.field[i]
		local field_value =msg[name]
		local field_count =field.count
		if type(field_value) == 'nil' then
			if field.required then
				return false, string.format("fail to check protocol %s(%d.%d), check field %s error:missing", proto.name, group_id, protocol_id, field.name)
			else
				field_value =field.init_value
			end
		end
		if field_count == 1 then
			local ok, err =check_value(field_value, field)
			if not ok then
				return false, string.format("fail to check protocol %s(%d.%d), check field %s error:%s", proto.name, group_id, protocol_id, field.name, err)
			end
		elseif field_count == 0 or field_count > 1 then
			if type(v) ~= 'table' then
				return false, string.format("fail to check protocol %s(%d.%d), field %s is not a table", proto.name, group_id, protocol_id, field.name)
			end
			local n
			if field_count > 1 then
				n =field_count
			else
				n =#v
				if field.min_count then
					if n < field.min_count then
						return false, string.format("fail to check protocol %s(%d.%d), check field %s error:min count is %d", proto.name, group_id, protocol_id, field.name, field.min_count)
					end
				end
				if field.max_count then
					if n > field.max_count then
						return false, string.format("fail to check protocol %s(%d.%d), check field %s error:max count is %d", proto.name, group_id, protocol_id, field.name, field.max_count)
					end
				end
			end
			for j=1, n do
				local ok, err =check_value(field_value[j], field)
				if not ok then
					return false, string.format("fail to check protocol %s(%d.%d), check field %s[%d] error:%s", proto.name, group_id, protocol_id, field.name, j, err)
				end
			end
		end
	end
	return true
end
-- ProtocolManager --
ProtocolManager ={}
-- when success return two values.
--    second == true  => first is a object;
--    second == false => first is a bytes
-- when fail return nil plus an error message
ProtocolManager.Encode =function(group_id, protocol_id, msg, force_bytes)
	if not msg then
		return nil, "protocol encode failed, msg is not a table"
	end
	if g_group_table[group_id] then
		local proto =g_group_table[group_id][protocol_id]
		if not proto then
			return nil, string.format("fail to encode protocol %d.%d, protocol not found", group_id, protocol_id)
		end
		local ok, err =check(msg, proto, group_id, protocol_id)
		if err then
			return nil, err
		end
		local bs, err =encode_protocol(group_id, protocol_id, msg)
		if err then
			return nil, err
		else
			return bs, false
		end
	else
		if force_bytes then
			local bs, err =ProtocolManager.TableToBytes(group_id, protocol_id, msg)
			if err then
				return nil, err
			else
				return bs, false
			end
		else
			local obj, err =ProtocolManager.TableToObject(group_id, protocol_id, msg)
			if err then
				return nil, err
			else
				return obj, true
			end
		end
	end
end
-- when success return table or nil --
-- when fail return nil plus an error message --
ProtocolManager.Decode =function(group_id, protocol_id, bs)
	if not bs then
		return nil, "protocol decode failed, bs is not a bytes"
	end
	if g_group_table[group_id] then
		local proto =g_group_table[group_id][protocol_id]
		if not proto then
			return nil, string.format("fail to decode protocol %d.%d, protocol not found", group_id, protocol_id)
		end
		local msg, err =decode_protocol(group_id, protocol_id, bs)
		if err then
			return nil, err
		end
		if msg then
			local ok, err =check(msg, proto, group_id, protocol_id)
			if err then
				return nil, err
			end
		end
		return msg
	else
		return ProtocolManager.BytesToTable(group_id, protocol_id, bs)
	end
end
-- when success return table --
-- when fail return nil plus an error message --
ProtocolManager.EncodeAsObject =function(group_id, protocol_id, msg)
	if not msg then
		return nil, "protocol encode as object failed, msg is not a table"
	end
	return ProtocolManager.TableToObject(group_id, protocol_id, msg)
end
function ProtocolManager.Register(group_id, protocol_group)
	local tb ={}
	for k, v in pairs(protocol_group) do
		local proto =v
		if not v.field and #v>0 then
			proto ={ field =v }
		end
		proto.id =v.id
		tb[proto.id] =proto
	end
	g_group_table[group_id] =tb;
end
ProtocolManager.SetDefaultGroupId =function(grp_id)
	ProtocolManager.DEFAULT_GROUP_ID =grp_id
end
