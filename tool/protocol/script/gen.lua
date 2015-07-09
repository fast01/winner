#!/usr/local/bin/lua
package.path =package.path .. ";./?.lua;../../common/?.lua;../script/?.lua"

require "build_in_type"
require "check_protocol"
require "util"
require "out"

----
---- func
----
function gen_field_initilizer(protocol, field)
	outcpp("void %s::_init_field_%s(){", protocol.name, field.name)
	if field.is_build_in then
		if field.count == 1 then
			if field.is_string then
				outcpp("	m_%s =%s;", field.name, field.init_value)
				outcpp("	RETAIN_POINTER(m_%s);", field.name)
			else
				outcpp("	m_%s =%s;", field.name, field.init_value)
			end
		elseif field.count > 1 then
			outcpp("	memset(m_%s, 0, sizeof(m_%s));", field.name, field.name)
			if field.init_value then
				for i=1, #field.init_value do
					if field.is_string then
						outcpp("	m_%s[%d] =%s;", field.name, i, field.init_value[i])
						outcpp("	RETAIN_POINTER(m_%s[%d]);", field.name, i)
					else
						outcpp("	m_%s[%d] =%s;", field.name, i, field.init_value[i])
					end
				end
			end
		else
			if field.init_value then
				outcpp("	m_%s =::core::SafeNew<Array>());", field.name)
				outcpp("	RETAIN_POINTER(m_%s);", field.name)
				for i=1, #field.init_value do
					outcpp("	m_%s->push_back(%s);", field.name, i, field.init_value[i])
				end
			else
				outcpp("	m_%s =0;", field.name)
			end
		end
	else
		if field.count > 1 then
			outcpp("	memset(m_%s, 0, sizeof(m_%s));", field.name, field.name)
		else
			outcpp("	m_%s =0;", field.name)
		end
	end
	outcpp("}")
end
function gen_class_define(protocol)
	outinc("/** define  %s **/", protocol.name)
	outinc("class %s: public ::core::ProtocolBase{", protocol.name)
	outinc("	typedef ::core::ProtocolBase Super;")
	outinc("	SUPPORT_NEWABLE")
	outinc("public:")
	outinc("	%s();", protocol.name)
	outinc("	virtual ~%s();", protocol.name)
	outinc("public:")
	outinc("	virtual void init();")
	outinc("	virtual void finalize();")
	outinc("public:")
	outinc("	static ::core::ProtocolBase* Creator();")
	outinc("public:")
	outinc("	virtual int64_t group();")
	outinc("	virtual int64_t id();")
	outinc("	virtual const char* name();")
	outinc("	virtual bool check();")
	outinc("	virtual void clean();")
	outinc("	virtual ::core::Bytes* toBytes(::core::Bytes* bytes);")
	outinc("	virtual bool fromBytes(::core::Bytes* bytes);")
	outinc("	virtual bool toLua(lua_State* L);")
	outinc("	virtual bool fromLua(lua_State* L, const int64_t idx);")
	outinc("	virtual bool appendToString(::core::Bytes* str, int64_t tab_count);")

	-- class field
	outinc("public:")
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outinc("	/* field %s */", field.name)
		if field.count == 1 then
			outinc("	DECLARE_PROPERTY(%s, %s);", field.type, field.name)
		elseif field.count < 1 then
			outinc("	DECLARE_PROPERTY_DYNAMIC_ARRAY(%s, %s, %s);", field.array_type, field.type, field.name)
		elseif field.count > 1 then
			outinc("	DECLARE_PROPERTY_FIXED_ARRAY(%s, %s, %d);", field.type, field.name, field.count)
		else
			perror("never run")
		end
		outinc("	void _init_field_%s();", field.name)
		if #field.enum > 0 then
			outinc("public:")
			outinc("	enum{")
			for i=1, #field.enum do
				local enum =field.enum[i]
				outinc("		%s =%d,", enum.name, enum.value)
			end
			outinc("	};")
		end
	end

	-- end
	outinc("};")
end
function gen_class_impl(protocol)
	outcpp("/** impl %s **/", protocol.name)
	outcpp("%s::%s(){", protocol.name, protocol.name)
	outcpp("}")

	outcpp("%s::~%s(){", protocol.name, protocol.name)
	outcpp("}")

	outcpp("void %s::init(){", protocol.name)
	outcpp("	Super::init();")
		for i=1, #protocol.field do
			local field =protocol.field[i]
			--[[
			if field.count <= 1 then
				outcpp("	m_%s =%s;", field.name, field.init_value)
				if field.is_object then
					outcpp("	RETAIN_POINTER(m_%s);", field.name)
				end
			else
				if field.init_value then
					init_field_from_init(field)
				else
					outcpp("	memset(m_%s, 0, sizeof(m_%s));", field.name, field.name)
				end
			end
			]]
			outcpp("	_init_field_%s();", field.name)
		end
	outcpp("}")

	outcpp("void %s::finalize(){", protocol.name)
	outcpp("	clean();")
	outcpp("	Super::finalize();")
	outcpp("}")

	outcpp("::core::ProtocolBase* %s::Creator(){", protocol.name)
	outcpp("	return ::core::SafeNew<%s>();", protocol.name)
	outcpp("}")

	outcpp("void %s::clean(){", protocol.name)
		for i=1, #protocol.field do
			local field =protocol.field[i]
			outcpp("	// clean %s ", field.name);
			if field.is_object then
				if field.count <= 1 then
					outcpp("	CLEAN_POINTER(m_%s);", field.name)
				else
					outcpp("	for(int64_t i=0; i<%d; ++i){", field.count)
					outcpp("		CLEAN_POINTER(m_%s[i]);", field.name)
					outcpp("	}")
				end
			else
				if field.count == 1 then
					outcpp("	m_%s =%s;", field.name, field.zero_value)
				else
					outcpp("	memset(m_%s, 0, sizeof(m_%s));", field.name, field.name)
				end
			end
		end
	outcpp("}")

	outcpp("int64_t %s::group(){", protocol.name)
	outcpp("	return %d;", ID)
	outcpp("}")
	outcpp("int64_t %s::id(){", protocol.name)
	outcpp("	return %d;", protocol.id)
	outcpp("}")
	outcpp("const char* %s::name(){", protocol.name)
	outcpp('	return "%s";', protocol.name)
	outcpp("}")
	
	outcpp("bool %s::check(){", protocol.name)
		for i=1, #protocol.field do
			local field =protocol.field[i]
			outcpp("	//// check %s ", field.name);
			if field.min_count and field.count == 0 then
				outcpp('	// check array count')
				outcpp('	if((m_%s ? m_%s->size() : 0) < %d){', field.name, field.name, field.min_count)
				outcpp('		ERROR("protocol %s check field, count(%s) < %d");', protocol.name, field.name, field.min_count)
				outcpp('	}')
			end
			if field.max_count and field.count == 0 then
				outcpp('	// check array count')
				outcpp('	if((m_%s ? m_%s->size() : 0) > %d){', field.name, field.name, field.max_count)
				outcpp('		ERROR("protocol %s check field, count(%s) > %d");', protocol.name, field.name, field.max_count)
				outcpp('	}')
			end
			outcpp('	// check value')
			if field.is_build_in then
				if field.min_value then
					if field.count == 0 then
						outcpp('	const int64_t n_lv_%s =(m_%s ? m_%s->size() : 0);', field.name, field.name, field.name)
						outcpp('	for(int64_t i=0; i<n_lv_%s; ++i){', field.name)
						outcpp('		if(m_%s->get(i) < %d){', field.name, field.min_value)
						outcpp('			ERROR("protocol %s check field, %s[%%lld] < %d", (long long)i);', protocol.name, field.name, field.min_value)
						outcpp('			return false;')
						outcpp('		}')
						outcpp('	}')
					elseif field.count == 1 then
						outcpp('	if(m_%s < %d){', field.name, field.min_value)
						outcpp('		ERROR("protocol %s check field, %s < %d");', protocol.name, field.name, field.min_value)
						outcpp('		return false;')
						outcpp('	}')
					else
						outcpp('	for(int64_t i=0; i<%d; ++i){', field.count)
						outcpp('		if(m_%s[i] < %d){', field.name, field.min_value)
						outcpp('			ERROR("protocol %s check field, %s[%%lld] < %d", (long long)i);', protocol.name, field.name, field.min_value)
						outcpp('			return false;')
						outcpp('		}')
						outcpp('	}')
					end
				end
				if field.max_value then
					if field.count == 0 then
						outcpp('	const int64_t n_hv_%s =(m_%s ? m_%s->size() : 0);', field.name, field.name, field.name)
						outcpp('	for(int64_t i=0; i<n_hv_%s; ++i){', field.name)
						outcpp('		if(m_%s->get(i) > %d){', field.name, field.max_value)
						outcpp('			ERROR("protocol %s check field, %s[%%lld] > %d", (long long)i);', protocol.name, field.name, field.max_value)
						outcpp('			return false;')
						outcpp('		}')
						outcpp('	}')
					elseif field.count == 1 then
						outcpp('	if(m_%s > %d){', field.name, field.max_value)
						outcpp('		ERROR("protocol %s check field, %s > %d");', protocol.name, field.name, field.max_value)
						outcpp('		return false;')
						outcpp('	}')
					else
						outcpp('	for(int64_t i=0; i<%d; ++i){', field.count)
						outcpp('		if(m_%s[i] > %d){', field.name, field.max_value)
						outcpp('			ERROR("protocol %s check field, %s[%%lld] > %d", (long long)i);', protocol.name, field.name, field.max_value)
						outcpp('			return false;')
						outcpp('		}')
						outcpp('	}')
					end
				end
				if field.min_length then
					if field.is_string then
						if field.count == 0 then
							outcpp('	const int64_t n_ll_%s =(m_%s ? m_%s->size() : 0);', field.name, field.name, field.name)
							outcpp('	for(int64_t i=0; i<n_ll_%s; ++i){', field.name)
							outcpp('		if(::core::String::Length(dynamic_cast< ::core::String* >(m_%s->get(i))) < %d){', field.name, field.min_length)
							outcpp('			ERROR("protocol %s check field, length(%s[%%lld]) < %d", (long long)i);', protocol.name, field.name, field.min_length)
							outcpp('			return false;')
							outcpp('		}')
							outcpp('	}')
						elseif field.count == 1 then
							outcpp('	if(::core::String::Length(m_%s) < %d){', field.name, field.min_length)
							outcpp('		ERROR("%%s %%lld", m_%s?m_%s->c_str():"null", ::core::String::Length(m_%s));', field.name, field.name, field.name)
							outcpp('		ERROR("protocol %s check field, length(%s) < %d");', protocol.name, field.name, field.min_length)
							outcpp('		return false;')
							outcpp('	}')
						else
							outcpp('	for(int64_t i=0; i<%d; ++i){', field.count)
							outcpp('		if(core::String::Length(m_%s[i]) < %d){', field.name, field.min_length)
							outcpp('			ERROR("protocol %s check field, length(%s[%%lld]) < %d", (long long)i);', protocol.name, field.name, field.min_length)
							outcpp('			return false;')
							outcpp('		}')
							outcpp('	}')
						end
					end
				end
				if field.max_length then
					if field.is_string then
						if field.count == 0 then
							outcpp('	const int64_t n_hl_%s =(m_%s ? m_%s->size() : 0);', field.name, field.name, field.name)
							outcpp('	for(int64_t i=0; i<n_hl_%s; ++i){', field.name)
							outcpp('		if(::core::String::Length(dynamic_cast< ::core::String* >(m_%s->get(i))) > %d){', field.name, field.max_length)
							outcpp('			ERROR("protocol %s check field, length(%s[%%lld]) > %d", (long long)i);', protocol.name, field.name, field.max_length)
							outcpp('			return false;')
							outcpp('		}')
							outcpp('	}')
						elseif field.count == 1 then
							outcpp('	if(::core::String::Length(m_%s) > %d){', field.name, field.max_length)
							outcpp('		ERROR("%%s %%lld", m_%s?m_%s->c_str():"null", ::core::String::Length(m_%s));', field.name, field.name, field.name)
							outcpp('		ERROR("protocol %s check field, length(%s) > %d");', protocol.name, field.name, field.max_length)
							outcpp('		return false;')
							outcpp('	}')
						else
							outcpp('	for(int64_t i=0; i<%d; ++i){', field.count)
							outcpp('		if(core::String::Length(m_%s[i]) > %d){', field.name, field.max_length)
							outcpp('			ERROR("protocol %s check field, length(%s[%%lld]) > %d", (long long)i);', protocol.name, field.name, field.max_length)
							outcpp('			return false;')
							outcpp('		}')
							outcpp('	}')
						end
					end
				end
			else
				if field.count == 0 then
					outcpp('	const int64_t n_so_%s =(m_%s ? m_%s->size() : 0);', field.name, field.name, field.name)
					outcpp('	for(int64_t i=0; i<n_so_%s; ++i){', field.name)
					outcpp('		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_%s->get(i))){', field.name)
					outcpp('			if(o->check() == false){')
					outcpp('				ERROR("protocol %s check field, check the %%lldth %s error", (long long)i);', protocol.name, field.name)
					outcpp('				return false;')
					outcpp('			}')
					outcpp('		}')
					outcpp('	}')
				elseif field.count > 1 then
					outcpp('	for(int64_t i=0; i<%d; ++i){', field.count)
					outcpp('		if(::core::ProtocolBase* o =dynamic_cast< ::core::ProtocolBase* >(m_%s[i])){', field.name)
					outcpp('			if(o->check() == false){')
					outcpp('				ERROR("protocol %s check field, check the %%lldth %s error", (long long)i);', protocol.name, field.name)
					outcpp('				return false;')
					outcpp('			}')
					outcpp('		}')
					outcpp('	}')
				end
			end
		end
		outcpp('	return true;')
	outcpp("}")
	outcpp("::core::Bytes* %s::toBytes(::core::Bytes* bytes){", protocol.name)
	outcpp("	if(!check()){")
	outcpp("		return 0;")
	outcpp("	}")
	outcpp("	if(!bytes){ bytes =::core::SafeNew<::core::Bytes>(); }")
	outcpp("	const int64_t write_cursor_beg =bytes->getWriteCursor();")
	outcpp("	packet_size_t length =0;;")
	outcpp("	bytes->write(&length, sizeof(length));")
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outcpp("	/// %s", field.name)
		if field.count == 1 then
			outcpp('	::core::ToBytes< %s >(bytes, m_%s);', field.type, field.name)
		elseif field.count > 1 then
			outcpp("	for(int64_t i=0; i<%d; ++i){", field.count)
			outcpp('		::core::ToBytes< %s >(bytes, m_%s[i]);', field.type, field.name)
			outcpp("	}")
		else
			outcpp("	const int64_t %s_cnt =(m_%s ? (m_%s->size()) : 0);", field.name, field.name, field.name)
			outcpp("	::core::ToBytes<int64_t>(bytes, %s_cnt);", field.name)
			outcpp("	for(int64_t i=0; i<%s_cnt; ++i){", field.name)
			outcpp("		%s item =%s< %s >(m_%s->get(i));", field.type, field.cast, field.type, field.name)
			outcpp('		::core::ToBytes< %s >(bytes, item);', field.type)
			outcpp("	}")
		end
	end
	outcpp("	/// reset length")
	outcpp("	const int64_t write_cursor_end =bytes->getWriteCursor();")
	outcpp("	length =host_to_net_packet_size(write_cursor_end - write_cursor_beg);")
	outcpp("	*reinterpret_cast< packet_size_t* >(bytes->c_str() + write_cursor_beg) =length;")
	outcpp("	return bytes;")
	outcpp("}")

	outcpp("bool %s::fromBytes(::core::Bytes* bytes){", protocol.name)
	outcpp("	clean();")
	outcpp("	if(!bytes){ return false; }")
	outcpp("	const int64_t read_cursor_beg =bytes->getReadCursor();")
	outcpp("	int64_t read_cursor_cur =0;")
	outcpp("	int64_t read_cnt =0;")
	outcpp("	packet_size_t length =0;")
	outcpp("	if(!bytes->read(&length, sizeof(length))){ return false; }")
	outcpp("	length =net_to_host_packet_size(length);")
	outcpp("	if(0 == length){ return true; }")
	outcpp("	bool just_init =false;")
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outcpp("	/// %s", field.name)
		outcpp("	if(just_init){")
		outcpp("		_init_field_%s();", field.name)
		outcpp("	}")
		outcpp("	else{")
		if field.count == 1 then
			outcpp('		if(!::core::FromBytes< %s >(bytes, m_%s)){ clean(); return false; }', field.type, field.name)
			if field.is_object then
				outcpp('		RETAIN_POINTER(m_%s);', field.name)
			end
		elseif field.count > 1 then
			outcpp("		for(int64_t i=0; i<%d; ++i){", field.count)
			outcpp('			if(!::core::FromBytes< %s >(bytes, m_%s[i])){ clean(); return false; }', field.type, field.name)
			if field.is_object then
				outcpp('			RETAIN_POINTER(m_%s[i]);', field.name)
			end
			outcpp("		}")
		else
			outcpp('		if(m_%s == 0){', field.name)
			outcpp('			m_%s =::core::SafeNew< std::remove_pointer< %s >::type >();', field.name, field.array_type)
			outcpp('			RETAIN_POINTER(m_%s);', field.name)
			outcpp('		}')
			outcpp("		int64_t %s_cnt =0;", field.name)
			outcpp('		if(!::core::FromBytes<int64_t>(bytes, %s_cnt)){ clean(); return false; }', field.name)
			outcpp("		for(int64_t i=0; i<%s_cnt; ++i){", field.name)
			outcpp("			%s item =%s;", field.type, field.zero_value)
			outcpp('			if(!::core::FromBytes< %s >(bytes, item)){ clean(); return false; }', field.type)
			outcpp('			m_%s->push_back(item);', field.name)
			outcpp("		}")
		end
		outcpp("		// check read cursor")
		outcpp("		read_cursor_cur =bytes->getReadCursor();")
		outcpp("		read_cnt =read_cursor_cur - read_cursor_beg;")
		outcpp("		if(read_cnt == (int64_t)length){")
		outcpp("			just_init =true;")
		outcpp("		}")
		outcpp("		else if(read_cnt > (int64_t)length){")
		outcpp('			ERROR("read too much bytes when unmashal %s from bytes");', protocol.name)
		outcpp("			return false;")
		outcpp("		}")
		outcpp("	}")
	end
	outcpp("	/// skip remaind bytes")
	outcpp("	if(read_cnt < (int64_t)length){")
	outcpp("		bytes->skip(length - read_cnt);")
	outcpp("	}")
	outcpp("	return check();")
	outcpp("}")

	outcpp("bool %s::toLua(lua_State* L){", protocol.name)
	outcpp("	if(!check()){")
	outcpp("		return false;")
	outcpp("	}")
	outcpp("	lua_createtable(L, 0, %d);", #protocol)
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outcpp("	// %s", field.name)
		if field.count == 1 then
			outcpp('	::core::ToLua< %s >(L, m_%s);', field.type, field.name)
			outcpp('	lua_setfield(L, -2, "%s");', field.name)
		elseif field.count > 1 then
			outcpp("	lua_createtable(L, %d, 0);", field.count)
			outcpp("	for(int64_t i=0; i<%d; ++i){", field.count)
			outcpp('		::core::ToLua< %s >(L, m_%s[i]);', field.type, field.name)
			outcpp('		lua_rawseti(L, -2, i+1);')
			outcpp("	}")
			outcpp('	lua_setfield(L, -2, "%s");', field.name)
		else
			outcpp("	const int64_t %s_cnt =(m_%s ? (m_%s->size()) : 0);", field.name, field.name, field.name)
			outcpp("	lua_createtable(L, (int)%s_cnt, 0);", field.name)
			outcpp("	for(int64_t i=0; i<%s_cnt; ++i){", field.name)
			outcpp("		%s item =%s< %s >(m_%s->get(i));", field.type, field.cast, field.type, field.name)
			outcpp('		::core::ToLua< %s >(L, item);', field.type)
			outcpp('		lua_rawseti(L, -2, i+1);')
			outcpp("	}")
			outcpp('	lua_setfield(L, -2, "%s");', field.name)
		end
	end
	outcpp("	return true;")
	outcpp("}")

	outcpp("bool %s::fromLua(lua_State* L, const int64_t idx){", protocol.name)
	outcpp("	clean();")
	outcpp("	::core::LuaTopHelper lth(L);")
	outcpp("	const int old_top =lua_gettop(L);")
	outcpp("	if(old_top <= 0){ return false; }")
	outcpp('	if(!lua_istable(L, idx)){ return false; }')
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outcpp("	// %s", field.name)
		outcpp('	lua_getfield(L, idx, "%s");', field.name)
		if field.count == 1 then
			outcpp('	if(lua_isnil(L, -1)){')
			if field.required then
				outcpp('		ERROR("missing %s")', field.name)
				outcpp("		return false;")
			else
				outcpp("		_init_field_%s();", field.name)
			end
			outcpp('	}')
			outcpp('	else{')
			outcpp('		if(!::core::FromLua< %s >(L, -1, m_%s)){ clean(); lua_settop(L, old_top); return false; }', field.type, field.name)
			if field.is_object then
				outcpp('		RETAIN_POINTER(m_%s);', field.name)
			end
			outcpp('	}')
		elseif field.count > 1 then
			outcpp('	if(lua_isnil(L, -1)){')
			if field.required then
				outcpp('		ERROR("missing %s")', field.name)
				outcpp("		return false;")
			else
				outcpp("		_init_field_%s();", field.name)
			end
			outcpp('	}')
			outcpp('	else if(lua_istable(L, -1)){')
			outcpp('		if(lua_istable(L, -1)){')
			outcpp("			for(int64_t i=0; i<%d; ++i){", field.count)
			outcpp('				lua_rawgeti(L, -1, i+1);')
			outcpp('				if(!::core::FromLua< %s >(L, -1, m_%s[i])){ clean(); lua_settop(L, old_top); return false; }', field.type, field.name)
			outcpp('				lua_pop(L, 1);')
			if field.is_object then
				outcpp('				RETAIN_POINTER(m_%s[i]);', field.name)
			end
			outcpp("			}")
			outcpp("		}")
			outcpp("	}")
			outcpp("	else{")
			outcpp("		return false;")
			outcpp("	}")
		else
			outcpp('	if(lua_isnil(L, -1)){')
			if field.required then
				outcpp('		ERROR("missing %s")', field.name)
				outcpp("		return false;")
			else
				outcpp("		_init_field_%s();", field.name)
			end
			outcpp('	}')
			outcpp('	else if(lua_istable(L, -1)){')
			outcpp('		if(lua_istable(L, -1)){')
			outcpp("			const int64_t %s_cnt =lua_objlen(L, -1);", field.name)
			outcpp('			if(m_%s == 0){', field.name)
			outcpp('				m_%s =::core::SafeNew< std::remove_pointer< %s >::type >();', field.name, field.array_type)
			outcpp('				RETAIN_POINTER(m_%s);', field.name)
			outcpp('			}')
			outcpp("			for(int64_t i=0; i<%s_cnt; ++i){", field.name)
			outcpp("				%s item =0;", field.type)
			outcpp('				lua_rawgeti(L, -1, i+1);')
			outcpp('				if(!::core::FromLua< %s >(L, -1, item)){ clean(); lua_settop(L, old_top); return false; }', field.type)
			outcpp('				lua_pop(L, 1);')
			outcpp('				m_%s->push_back(item);', field.name)
			outcpp("			}")
			outcpp("		}")
			outcpp("	}")
			outcpp("	else{")
			outcpp("		return false;")
			outcpp("	}")
		end
		outcpp("	lua_settop(L, old_top);")
	end
	outcpp("	return check();")
	outcpp("}")

	outcpp("bool %s::appendToString(::core::Bytes* str, int64_t tab_count){", protocol.name)
	outcpp("	if(tab_count < 0){ tab_count =0; }")
	outcpp([[	str->appendString(::core::String::Format("%%s{\n", tab(tab_count)));]])
	for i=1, #protocol.field do
		local field =protocol.field[i]
		outcpp("	// %s", field.name)
		if field.count == 1 then
			if field.is_build_in then
				outcpp('	str->appendString(::core::String::Format("%%s%s =%%s,\\n", tab(tab_count+1), ::core::ToString< %s >(m_%s)->c_str()));', field.name, field.type, field.name)
			else
				outcpp("	if(m_%s){", field.name)
				outcpp('		str->appendString(::core::String::Format("%%s%s =\\n", tab(tab_count+1)));', field.name)
				outcpp("		m_%s->appendToString(str, tab_count + 1);", field.name)
				outcpp("	}")
			end
		elseif field.count > 1 then
			outcpp('	str->appendString(::core::String::Format("%%s%s ={\\n", tab(tab_count+1)));', field.name)
			outcpp("	for(int64_t i=0; i<%d; ++i){", field.count, field.name)
			if field.is_build_in then
				outcpp('		str->appendString(::core::String::Format("%%s%%s,\\n", tab(tab_count+2), ::core::ToString< %s >(m_%s[i])->c_str()));', field.type, field.name)
			else
				outcpp("		if(m_%s[i]){ m_%s[i]->appendToString(str, tab_count + 2); }", field.name, field.name)
			end
			outcpp("	}")
			outcpp('	str->appendString(::core::String::Format("%%s},\\n", tab(tab_count+1)));')
		else
			outcpp('	str->appendString(::core::String::Format("%%s%s ={\\n", tab(tab_count+1)));', field.name)
			outcpp("	const int64_t %s_cnt =(m_%s ? (m_%s->size()) : 0);", field.name, field.name, field.name)
			outcpp("	for(int64_t i=0; i<%s_cnt; ++i){", field.name)
			if field.is_build_in then
				outcpp("		%s item =%s< %s >(m_%s->get(i));", field.type, field.cast, field.type, field.name)
				outcpp('		str->appendString(::core::String::Format("%%s%%s,\\n", tab(tab_count+2), ::core::ToString< %s >(item)->c_str()));', field.type)
			else
				outcpp("		if(%s item =dynamic_cast< %s >(m_%s->get(i))){", field.type, field.type, field.name)
				outcpp("			item->appendToString(str, tab_count + 2);", field.name)
				outcpp("		}")
			end
			outcpp("	}")
			outcpp('	str->appendString(::core::String::Format("%%s},\\n", tab(tab_count+1)));')
		end
	end
	outcpp([[	str->appendString(::core::String::Format("%%s},\n", tab(tab_count)));]])
	outcpp([[	str->appendNull();]])
	outcpp("	return true;")
	outcpp("}")

	-- class field
	for i=1, #protocol.field do
		local field =protocol.field[i]
		local decorator =field.is_object and "_P" or ""
		if field.count == 1 then
			outcpp("DEFINE_PROPERTY%s(%s, %s, %s)", decorator, protocol.name, field.type, field.name)
		elseif field.count < 1 then
			outcpp("DEFINE_PROPERTY_DYNAMIC_ARRAY(%s, %s, %s, %s)", protocol.name, field.array_type, field.type, field.name)
		elseif field.count > 1 then
			outcpp("DEFINE_PROPERTY_FIXED_ARRAY%s(%s, %s, %s, %d)", decorator, protocol.name, field.type, field.name, field.count)
		else
			perror("never run")
		end
		gen_field_initilizer(protocol, field)
	end
end
function gen_lua(protocol_list)
	set_lua_output(OUTPUT_PATH .. string.format("/script/%s.lua", OUTPUT_NAME));
	outlua("module '%s'", OUTPUT_NAME)
	outlua("-- group --")
	outlua("GroupId =%d;", ID)
	outlua("Group ={};")
	outlua('')
	outlua("-- protocol ids --")
	for i=1, #protocol_list do
		local proto =protocol_list[i]
		outlua("%s =%d;", proto.name, proto.id)
	end
	if GEN_LUA_ONLY_IDS then
		return
	end
	outlua('')
	outlua("-- protocol list --")
	for i=1, #protocol_list do
		local proto =protocol_list[i]
		outlua("-- %s", proto.name)
		outlua("Group[%s] ={", proto.name)
		outlua("	id =%s,", proto.name)
		outlua("	name ='%s',", proto.name)
		outlua("	field ={")
		for j=1, #proto.field do
			local field =proto.field[j]
			outlua("		{")
			-- basic
			outlua("			type ='%s',", field.origin_type)
			outlua("			count=%d,", field.count)
			outlua("			name ='%s',", field.name)
			outlua("			required =%s,", field.required and 'true' or 'false')
			-- init
			if type(field.init_value_for_lua)=='table' then
				outlua("			init_value ={")
				for k=1, #field.init_value_for_lua do
					outlua("				%s,", field.init_value_for_lua[k])
				end
				outlua("			},")
			elseif type(field.init_value_for_lua) == 'string' then
				outlua("			init_value =%s,", field.init_value_for_lua)
			end
			-- protocol
			if not field.is_build_in  then
				outlua("			proto =%s,", field.origin_type)
			end
			-- check
			if field.min_value then
				outlua("			min_value =%s,", field.min_value)
			end
			if field.max_value then
				outlua("			max_value =%s,", field.max_value)
			end
			if field.min_length then
				outlua("			min_length =%s,", field.min_length)
			end
			if field.max_length then
				outlua("			max_length =%s,", field.max_length)
			end
			if field.min_count then
				outlua("			min_count =%s,", field.min_count)
			end
			if field.max_count then
				outlua("			max_count =%s,", field.max_count)
			end
			outlua("		},")
		end
		outlua("	},")
		outlua("};")
		outlua("")
	end
end
function gen_inc(protocol_list)
	-- output
	set_inc_output(OUTPUT_PATH .. string.format("/%s.h", OUTPUT_NAME));
	outinc("#ifndef H_AUTO_GEN_CODE_%s_H__", string.upper(OUTPUT_NAME))
	outinc("#define H_AUTO_GEN_CODE_%s_H__", string.upper(OUTPUT_NAME))
	if INCLUDE_FILE then
		outinc('#include "%s"', INCLUDE_FILE)
	end
	outinc("")
	-- namespace begin
	for i=1, #NAMESPACE do
		outinc("namespace %s{", NAMESPACE[i])
	end
	-- pre var
	local proto_cnt =#protocol_list

	-- id
	outinc("/*** id ***/")
	outinc("enum{ ID=%d };", ID)

	-- gen register
	outinc("/*** register ***/")
	outinc("bool register_protocol();")

	-- gen creator
	outinc("/*** creator ***/")
	outinc("typedef ::core::ProtocolBase* (*PFN_PROTOCOL_CREATOR)();")
	outinc("::core::ProtocolBase* create_protocol(const int64_t id);")
	outinc("::core::ProtocolBase* create_protocol_from_file(const int64_t id, const char* path);")
	outinc("::core::ProtocolBase* create_protocol_from_bytes(const int64_t id, ::core::Bytes* bytes);")
	outinc("::core::ProtocolBase* create_protocol_from_lua_file(const int64_t id, const char* path);")
	outinc("")

	-- gen predeclare
	outinc("/*** predeclare ***/")
	for i=1, proto_cnt do
		local protocol =protocol_list[i]
		outinc("class %s;", protocol.name)
	end
	outinc("")

	-- gen enum
	outinc("/*** enum ***/")
	outinc("enum{")
	outinc("	PROTOCOL____,")
	local protocol_max_id =0
	for i=1, proto_cnt do
		local protocol =protocol_list[i]
		local transform_name ="PROTOCOL_"
		for j=1, #protocol.name do
			local last_ch =string.sub(transform_name, -1, -1)
			local ch =string.sub(protocol.name, j, j)
			if (last_ch~='_') and (ch>='A' and ch<='Z') then
				transform_name =transform_name .. "_" .. ch
			else
				transform_name =transform_name .. ch
			end
		end
		outinc("	%s=%d,", string.upper(transform_name), protocol.id)
		if protocol.id > protocol_max_id then
			protocol_max_id =protocol.id
		end
	end
	outinc("	PROTOCOL_MAX_ID =%d", protocol_max_id)
	outinc("};")
	outinc("")

	-- gen class define
	outinc("/*** class define ***/")
	for i=1, proto_cnt do
		local protocol =protocol_list[i]
		gen_class_define(protocol)
	end

	-- namespace close
	for i=1, #NAMESPACE do
		outinc("}")
	end
	outinc("#endif")
end

function gen_cpp(protocol_list)
	set_cpp_output(OUTPUT_PATH .. string.format("/%s.cpp", OUTPUT_NAME));
	outcpp('#include "%s.h"', OUTPUT_NAME)
	outcpp("")
	-- namespace begin
	for i=1, #NAMESPACE do
		outcpp("namespace %s{", NAMESPACE[i])
	end
	outcpp("/*** register ***/")
	outcpp("static PFN_PROTOCOL_CREATOR g_protocol_creator[PROTOCOL_MAX_ID];")
	outcpp("bool register_protocol(){")
	outcpp("	// Need't set as zero, because g_protocol_creator will be set zero when program started, but i have compulsion!!!")
	outcpp("	memset(g_protocol_creator, 0, sizeof(g_protocol_creator));")
	for i=1, #protocol_list do
		local protocol =protocol_list[i]
		outcpp("	g_protocol_creator[%d - 1] =%s::Creator; ", protocol.id, protocol.name)
	end
	outcpp("	return ::core::ProtocolManager::Instance()->registerProtocolGroup(ID, &create_protocol);")
	outcpp("}")
	outcpp("/*** create protocol ***/")
	outcpp("::core::ProtocolBase* create_protocol(const int64_t id){")
	outcpp("	if(id>=1 && id<=PROTOCOL_MAX_ID){ return g_protocol_creator[id-1](); }")
	outcpp("	return 0;")
	outcpp("}")
	outcpp("::core::ProtocolBase* create_protocol_from_bin_file(const int64_t id, const char* path){")
	outcpp("	if(::core::Bytes* bs =::core::read_bytes(path)){ return create_protocol_from_bytes(id, bs); }")
	outcpp("	else { return 0; }")
	outcpp("}")
	outcpp("::core::ProtocolBase* create_protocol_from_bytes(const int64_t id, ::core::Bytes* bytes){")
	outcpp("	if(!bytes){ return 0; }")
	outcpp("	if(::core::ProtocolBase* proto =create_protocol(id)){")
	outcpp("		if(proto->fromBytes(bytes)){")
	outcpp("			return proto;")
	outcpp("		}")
	outcpp("		else{")
	outcpp('			ERROR("fail to call %%s, unmarsh bytes error", __FUNCTION__);')
	outcpp("			return 0;")
	outcpp("		}")
	outcpp("	}")
	outcpp("	else{")
	outcpp('		ERROR("fail to call %%s, protocol %%lld not exist", __FUNCTION__, (long long)id);')
	outcpp("		return 0;")
	outcpp("	}")
	outcpp("}")
	outcpp("::core::ProtocolBase* create_protocol_from_lua_file(const int64_t id, const char* path){")
	outcpp("	lua_State* L =luaL_newstate();")
	outcpp("	if(!L){")
	outcpp('		ERROR("fail to call %%s, lua_newstate return 0", __FUNCTION__);')
	outcpp("		return 0;")
	outcpp("	}")
	outcpp("	luaL_openlibs(L);")
	outcpp("	if(0 != luaL_dofile(L, path)){")
	outcpp('		ERROR("fail to call %%s, %%s", __FUNCTION__, lua_tostring(L, -1));')
	outcpp("		lua_close(L);")
	outcpp("		return 0;")
	outcpp("	}")
	outcpp('	lua_getglobal(L, "Config");')
	outcpp("	if(!lua_istable(L, -1)){")
	outcpp('		ERROR("fail to call %%s, Config not a table", __FUNCTION__);')
	outcpp("		lua_close(L);")
	outcpp("		return 0;")
	outcpp("	}")
	outcpp("	if(::core::ProtocolBase* proto =create_protocol(id)){")
	outcpp("		if(proto->fromLua(L, -1)){")
	outcpp("			lua_close(L);")
	outcpp("			return proto;")
	outcpp("		}")
	outcpp("		else{")
	outcpp('			ERROR("fail to call %%s, unmarsh lua error", __FUNCTION__);')
	outcpp("			lua_close(L);")
	outcpp("			return 0;")
	outcpp("		}")
	outcpp("	}")
	outcpp("	else{")
	outcpp('		ERROR("fail to call %%s, protocol %%lld not exist", __FUNCTION__, (long long)id);')
	outcpp("		lua_close(L);")
	outcpp("		return 0;")
	outcpp("	}")
	outcpp("}")

	-- pre var
	local proto_cnt =#protocol_list

	-- gen tab func
	outcpp("/*** tab func ***/")
	outcpp("static const char* tab(int64_t cnt){")
	outcpp("	return ::core::String::Repeat('\t', cnt)->c_str();")
	outcpp("}")


	-- gen class impl
	outcpp("/*** class impl ***/")
	for i=1, proto_cnt do
		local protocol =protocol_list[i]
		outcpp("/** class %s **/", protocol.name)
		gen_class_impl(protocol)
	end

	-- namespace close
	for i=1, #NAMESPACE do
		outcpp("}")
	end
end


----
---- compile
----
-- load protocol and check --
if #arg < 2 then
	print("missing input dir or dir empty")
	os.exit(1)
end
local g_dir =path_ensure_end_without_slash(arg[1])
print("directory is " .. g_dir)

local g_protocol_list ={}
for i=2, #arg do
	local file =arg[i]
	if #file>6 and string.sub(file, -6, -1)==".proto" then
		local module_name =string.sub(file, 1, -7)
		print("compiling " .. module_name)
		dofile(g_dir .. "/" .. file)
		local protocol =_G[module_name];
		check_table(protocol, "moudle is not " .. module_name)
		for k, v in pairs(protocol) do
			if type(k) == 'string' and string.sub(k, 1, 1)>='A' and string.sub(k, 1, 1)<='Z' and type(v) == 'table' then
				print ("parsing " .. k)
				local proto =v
				if not v.field then
					proto ={ field =v, id =v.id }
				end
				proto.name =k
				check_protocol(proto)
				table.insert(g_protocol_list, proto)
			end
		end
	end
end

-- sort by name --
table.sort(g_protocol_list, function(a, b) return a.name < b.name; end)

-- calc max id --
local id_tb ={}
local max_id =0
for i=1, #g_protocol_list do
	local proto =g_protocol_list[i]
	local id = proto.id
	if id then
		check_int(id, "protocol id is not valid")
		max_id =math.max(max_id, id)
		if id_tb[id] then
			perror("id %d duplicate, protocol name %s <=> %s", id, id_tb[i].name, proto.name)
		end
		id_tb[id] =g_protocol_list[i]
	end
end

-- auto gen id
local auto_id =max_id + 1
print(string.format("first auto id is %d", auto_id))
for i=1, #g_protocol_list do
	local id = g_protocol_list[i].id
	if not id then
		g_protocol_list[i].id =auto_id
		auto_id =auto_id + 1
	end
end

-- sort by id --
table.sort(g_protocol_list, function(a, b) return a.id < b.id; end)

-- load config --
local cfg =require "config"
ID           =cfg.Id
NAMESPACE    =cfg.Namespace
OUTPUT_PATH  =cfg.OutputPath
OUTPUT_NAME  =cfg.OutputName
INCLUDE_FILE =cfg.IncludeFile
GEN_LUA_ONLY_IDS =cfg.GenLuaOnlyIds

check_int(ID, "ID is not valid")
check_table(NAMESPACE, "NAMESPACE is not valid")
check_nstring(OUTPUT_PATH, "OUTPUT_PATH is not valid")
check_nstring(OUTPUT_NAME, "OUTPUT_NAME is not valid")
OUTPUT_PATH =path_ensure_end_without_slash(OUTPUT_PATH)

-- gen code --
gen_lua(g_protocol_list)
gen_inc(g_protocol_list)
gen_cpp(g_protocol_list)
print("success")
