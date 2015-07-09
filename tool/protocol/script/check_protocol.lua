function check_value(field, v)
	local lua_value, cpp_value
	if field.is_string then
		v =v or ''
		if not is_string(v) then
			return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
		end
		lua_value =string.format('"%s"', encode_quat_string(v))
		cpp_value =string.format('::core::String::NewString("%s")', encode_quat_string(v))
	elseif field.is_boolean then
		lua_value =v and 'true' or 'false'
		cpp_value =lua_value
	elseif field.is_number then
		v =v or 0
		if not is_number(v) then
			return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
		end
		lua_value =v .. ""
		cpp_value =lua_value
	else
		lua_value ='nil'
		cpp_value ='0'
	end
	return lua_value, cpp_value
end
function check_protocol_internal(protocol)
	if not is_table(protocol) then
   		return nil, "check_protocol fail, protocol is not a table"
	end
	if not is_id(protocol.name) then
		return nil, "check_protocol fail, protocol name is not a valid id"
	end
	if not is_table(protocol.field) then
		return nil, sprintf("check_protocol %s fail, protocol field is not a table", protocol.name)
	end
	if string.upper(protocol.name) == "UNKNOWN" then
		return nil, sprintf("check_protocol %s fail, protocol name can't be UNKNOWN", protocol.name)
	end

	-- check field
	for i=1, #protocol.field do
		local field =protocol.field[i]
		field.count =field.count or 1
		field.desc  =field.desc  or ""

		if field.count < 0 then
			return nil, sprintf("check_protocol %s fail, field[%d].count < 0", protocol.name, i)
		end
		-- name
		if not is_id(field.name) then
			return nil, sprintf("check_protocol %s fail, field[%d] name invalid", protocol.name, i)
		end
		-- type
		if not is_id(field.type) then
			return nil, sprintf("check_protocol %s fail, field[%d] type invalid", protocol.name, i)
		end
		-- count
		if not is_number(field.count) then
			return nil, sprintf("check_protocol %s fail, field[%d] count invalid", protocol.name, i)
		end
		field.count =math.floor(field.count)
		field.count =math.max(0, field.count)

		-- required
		field.required =field.required and true or false

		-- desc
		if not is_string(field.desc) then
			return nil, sprintf("check_protocol %s fail, field[%d] desc invalid", protocol.name, i)
		end

		-- enum
		field.enum =field.enum or {}
		for j=1, #field.enum do
			local enum =field.enum[j]
			if not is_id(enum.name) then
				return nil, sprintf("check_protocol %s fail, field[%d].enum[%d] name invalid", protocol.name, i, j)
			end
			if not is_number(enum.value) then
				return nil, sprintf("check_protocol %s fail, field[%d].enum[%d] value invalid", protocol.name, i, j)
			end
		end

		-- origin type
		field.origin_type =field.type

		-- process type
		if field.type == 'string' then
			-- basic
			field.is_object =true
			field.is_number =false
			field.is_boolean =false
			field.is_string =true

			field.is_build_in =true

			-- for cpp
			field.array_type ="::core::Array*"
			field.type ="::core::String*"
			field.cast ="dynamic_cast"
			field.zero_value ="0"

			-- min, max
			field.min_value =nil
			field.max_value =nil

			-- length min, max
			if type(field.min_length)~='nil' and not is_number(field.min_length) then
				return nil, sprintf("check_protocol %s fail, field[%d] min_length invalid", protocol.name, i)
			end
			if type(field.max_length)~='nil' and not is_number(field.max_length) then
				return nil, sprintf("check_protocol %s fail, field[%d] max_length invalid", protocol.name, i)
			end

			-- init
			if type(field.init_value)~='nil' and not is_string(field.init_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
			end
		elseif field.type == 'boolean' then
			-- basic
			field.is_object =false
			field.is_number =false
			field.is_boolean =true
			field.is_string =false

			field.is_build_in =true

			-- for cpp
			field.array_type ="::core::BooleanArray*"
			field.type =field.type .. "_t"
			field.cast ="static_cast"
			field.zero_value ="false"

			-- min, max
			field.min_value =nil
			field.max_value =nil

			-- length min, max
			field.min_length =nil
			field.max_length =nil

			-- init
			if type(field.init_value)~='nil' and not is_boolean(field.init_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
			end
		elseif is_build_in_type(field.type) then
			-- basic
			field.is_object =false
			field.is_number =true
			field.is_boolean =false
			field.is_string =false

			field.is_build_in =true

			-- for cpp
			field.array_type ="::core::" .. string.upper(string.sub(field.type, 1 ,1)) .. string.sub(field.type, 2, -1) .. "Array*"
			field.type =field.type .. "_t"
			field.cast ="static_cast"
			field.zero_value ="0"

			-- min, max
			if type(field.min_value)~='nil' and not is_number(field.min_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] min_value invalid", protocol.name, i)
			end
			if type(field.max_value)~='nil' and not is_number(field.max_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] max_value invalid", protocol.name, i)
			end

			-- length min, max
			field.min_length =nil
			field.max_length =nil

			-- init
			if type(field.init_value)~='nil' and not is_number(field.init_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
			end
			if field.min_value and field.init_value and field.init_value<field.min_value then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid, can't less than min value", protocol.name, i)
			end
			if field.max_value and field.init_value and field.init_value>field.max_value then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid, can't greater than max value", protocol.name, i)
			end
		else
			-- basic
			field.is_object =true
			field.is_number =false
			field.is_boolean =false
			field.is_string =false

			field.is_build_in =false

			-- for cpp
			field.array_type ="::core::Array*"
			field.type =field.type .. "*"
			field.cast ="dynamic_cast"
			field.zero_value ="0"

			-- init
			field.init_value =nil

			-- min, max
			field.min_value =nil
			field.max_value =nil

			-- length min, max
			field.min_length =nil
			field.max_length =nil

			-- init
			if type(field.init_value)~='nil' and not is_table(field.init_value) then
				return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
			end
		end

		-- array count
		if field.count == 0 then
			field.is_object =true
			-- count min, max
			if type(field.min_count)~='nil' and not is_number(field.min_count) then
				return nil, sprintf("check_protocol %s fail, field[%d] min_count invalid", protocol.name, i)
			end
			if type(field.max_count)~='nil' and not is_number(field.max_count) then
				return nil, sprintf("check_protocol %s fail, field[%d] max_count invalid", protocol.name, i)
			end
		else
			field.min_count =nil
			field.max_count =nil
		end
		-- init value
		if field.is_build_in then
			if field.count == 1 then
				local lua_val, cpp_val =check_value(field, field.init_value)
				if not lua_val then
					local err =cpp_val
					return nil, err
				end
				field.init_value_for_lua, field.init_value =lua_val, cpp_val
			else
				if type(field.init_value)~='nil' and not is_table(field.init_value) then
					return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid", protocol.name, i)
				end
				if field.init_value then
					local n =#field.init_value
					if field.count>1 and n~=field.count then
						return nil, sprintf("check_protocol %s fail, field[%d] init_value invalid, init_value count is not equal field.count", protocol.name, i)
					end
					field.init_value_for_lua ={}
					for j=1, n do
						local lua_val, cpp_val =check_value(field, field.init_value[j])
						if not lua_val then
							local err =cpp_val
							return nil, err
						end
						field.init_value_for_lua[j], field.init_value[j] =lua_val, cpp_val
					end
				else
					field.init_value =nil
					field.init_value_for_lua =nil
				end
			end
		else
			field.init_value =nil
			field.init_value_for_lua =nil
		end
	end	
	return true
end

function check_protocol(protocol)
	-- check internal
	local ok, err = check_protocol_internal(protocol)
	if not ok then
		perror(err)
	end
end
