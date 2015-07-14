--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
local pcall =pcall
local pairs =pairs
local print =print
local require =require
local assert =assert
local table =table
local coroutine =coroutine
local math =math
local string =string
local _G =_G
local setmetatable =setmetatable
local getmetatable =getmetatable
module "Core"

--
-- global var
--

--
-- protocol group
--
local _protocol_group_id =0
local function set_protocol_group_id(id)
	_protocol_group_id =id
end
local function get_protocol_group_id()
	return _protocol_group_id
end

-- time
local _now =0
local function stable_now()
	local t =DateTime.Now()
	if t > _now then
		_now =t
	end
	return _now
end

--
-- mission
--
local MISSION_TTL     =30 -- 30 secs
local function do_mission(func, arg)
	assert(is_function(func), "do mission failed, invalid arg");
	local cr =coroutine.create(function()
		local state, err =pcall(func, arg)
		if not state then
			ERROR(err)
		end
	end);
	local state, err =coroutine.resume(cr)
	assert(state, err)
end

--
-- timer
--
local _timer_table ={}
local function set_timer(desc)
	assert(is_table(desc)
        and is_number(desc.id) 
		and is_number(desc.interval)
        and (is_number(desc.counter) or is_nil(desc.counter))
        and is_function(desc.func)
		, "set timer failed, invalid arg"
	);
	local timer ={
		id =math.floor(desc.id),
		interval =math.max(1, math.floor(desc.interval)),
		func =desc.func,
	}
	if desc.counter then
		timer.counter =math.floor(desc.counter)
	end
	timer.wakeup_time =stable_now() + timer.interval
	_timer_table[timer.id] =timer
end
local function del_timer(id)
	_timer_table[id] =nil
end
local function clear_timer()
	_timer_table ={}
end
local function update_timer(now)
	local ls ={}
	for k, v in pairs(_timer_table) do
		if now >= v.wakeup_time then
			if v.counter then
				v.counter = v.counter - 1
				if v.counter <= 0 then
					del_timer(k)
				else
					v.wakeup_time =now + v.interval
				end
			else
				v.wakeup_time =now + v.interval
			end
			table.insert(ls, v)
		end
	end
	for i=1, #ls do
		do_mission(ls[i].func, ls[i]);
	end
end

--
-- sleep
--
local _sleeper_id =0
local _sleeper_table ={}
local function sleep(t)
	-- check sleep time
	t =t or 0
	if t <= 0 then
		return true
	end

	-- prepare coroutine
	local cr =coroutine.running()
	assert(cr, "sleep failed, in main thread")

	-- make sleeper
	_sleeper_id =_sleeper_id + 1
	local sleeper ={
		cr =cr,
		wakeup_time =stable_now() + t
	}
	_sleeper_table[_sleeper_id] =sleeper

	-- sleep
	local result =coroutine.yield()
	if result and result.errcode == ErrorCode.OK then
		return true
	else
		return nil, "sleep failed"
	end
end
local function update_sleeper(now)
	local ls ={}
	for k, v in pairs(_sleeper_table) do
		if now >= v.wakeup_time then
			_sleeper_table[k] =nil
			table.insert(ls, v)
		end
	end
	for i=1, #ls do
		coroutine.resume(ls[i].cr, { errcode =ErrorCode.OK })
	end
end

--
-- task
--
local TASK_IDLE_TIMER =300 -- 300 secs
local _task_table      ={}
local function do_command(cmd_desc, requestor, packet, body, request)
	assert(is_table(cmd_desc)
		and is_function(cmd_desc.func)
		and (is_userdata(requestor) or is_nil(requestor))
		and is_table(packet)
		and is_number(packet.who)
		, "do command failed, invalid arg"
	);
	-- prepare task
	local who =packet.who
	local task =_task_table[who]
	if not task then
		task ={
			who =who,
			queue ={},
			cr =coroutine.create(function()
				local queue =task.queue
				while true do
					local mission =queue[1]
					task.quit_time =nil
					local state, err =pcall(mission.func, mission.request, mission.packet, mission.body, mission.requestor)
					if not state then
						ERROR(err)
					end
					table.remove(queue, 1)
					if #queue == 0 then
						task.quit_time =stable_now() + TASK_IDLE_TIMER
						local quit =coroutine.yield()
						if quit then
							DEBUG("task go die")
							break
						end
					end
				end
				_task_table[who] =nil
			end)
		};
		_task_table[who] =task
	end

	-- decode request
	local err
	if cmd_desc.use_protocol then
		if 0 ~= BitOp.And(packet.option, Packet.OPT_BODY_IS_OBJECT_POINTER) then
			request, err =ProtocolManager.ObjectToTable(request)
		else
			request, err =ProtocolManager.Decode(get_protocol_group_id(), packet.command, body)
		end
		if err then
			error(sprintf("protocol decode failed, %s", err))
		end
	end

	-- add cmd to queue
	table.insert(task.queue, { func =cmd_desc.func, request =request, packet =packet, body =body, requestor =requestor })

	-- exec
	if #task.queue == 1 then
		local state, err =coroutine.resume(task.cr)
		assert(state, err)
	end
	return true
end
local function update_task(now)
	local ls ={}
	for k, v in pairs(_task_table) do
		if #v.queue==0 and (not v.quit_time or now>=v.quit_time) then
			table.insert(ls, v)
		end
	end
	for i=1, #ls do
		local state, err =coroutine.resume(ls[i].cr, true)
		assert(state, err)
	end
end

--
-- rpc
--
local RPC_TTL =30 -- 30 secs
local _rpc_id =0
local _rpc_table ={}
local function do_rpc(packet, msg, respond_protocol_group_id, force_bytes)
	-- check
	local cr =coroutine.running()
	assert(cr, "do rpc faield, in main thread")
	-- new rpc
	_rpc_id =_rpc_id + 1
	local sn =_rpc_id
	packet.sn =sn
	local rpc ={
		cr =cr,
		expire_time =stable_now() + RPC_TTL
	};
	-- encode
	local body, err =ProtocolManager.Encode(get_protocol_group_id(), packet.command, msg, force_bytes)
	assert(body, sprintf("do rpc failed [encode], %s", err))
	local object =body
	local is_obj =err

	-- request
	local state, err
	packet.from =Service.Id()
	if is_obj then
		state, err =DispatcherManager.RequestByObject(Service.Self(), packet, object)
	else
		state, err =DispatcherManager.Request(Service.Self(), packet, body)
	end
	if not state then
		local e =sprintf("do rpc failed [request], %s", err)
		WARN(e)
		return nil, e
	end
	-- print(get_protocol_group_id())
	-- print(sprint_table(packet))
	-- print(sprint_table(msg))
	-- set rpc
	_rpc_table[sn] =rpc
	-- wait
	local res_packet, res_body, res_object =coroutine.yield()
	_rpc_table[sn] =nil
	if not res_packet then
		local e =sprintf("do rpc failed [yield], %s", res_body or "")
		WARN(e)
		return nil, e
	end
	-- decode
	if 0 ~= BitOp.And(res_packet.option, Packet.OPT_BODY_IS_OBJECT_POINTER) then
		return ProtocolManager.ObjectToTable(res_object)
	else
		local respond, err =ProtocolManager.Decode(respond_protocol_group_id, res_packet.command, res_body)
		if err then
			ERROR("do rpc failed [respond], %s", err)
		else
			return respond
		end
	end
end
local function do_resume_rpc(packet, body, object)
	local rpc =_rpc_table[packet.sn]
	if not rpc then
		WARN("do resume rpc failed, rpc %d not found", packet.sn)
		return
	end
	local state, err =coroutine.resume(rpc.cr, packet, body, object)
	assert(state, err)
end
local function update_rpc(now)
	local ls ={}
	for k, v in pairs(_rpc_table) do
		if now >= v.expire_time then
			table.insert(ls, v)
		end
	end
	for i=1, #ls do
		local state, err =coroutine.resume(ls[i].cr, nil, "timeout")
		assert(state, err)
	end
end

--
-- notify
--
local function do_notify(packet, msg)
	-- check
	local cr =coroutine.running()
	assert(cr, "do notify faield, in main thread")
	-- encode
	local body, err =ProtocolManager.Encode(get_protocol_group_id(), packet.command, msg, true)
	assert(body, sprintf("do notify failed [encode], %s", err))
	-- notify
	packet.from =Service.Id()
	local state, err =DispatcherManager.Notify(packet, body) 
	if not state then
		local e =sprintf("do notify failed [notify], %s", err)
		WARN(e)
		return nil, e
	end
	return true
end

--
-- reply
--
local function do_reply(requestor, packet, msg, force_bytes)
	-- encode
	local body, err =ProtocolManager.Encode(get_protocol_group_id(), packet.command, msg, force_bytes)
	assert(body, sprintf("do rpc failed, %s", err))
	local object =body
	local is_obj =err
	-- reply
	if is_obj then
		return requestor:replyByObject(packet, object)
	else
		return requestor:reply(packet, body)
	end
end
local function do_reply_easy(requestor, req_packet, res_cmd, msg, force_bytes)
	local res_packet =Packet.RequestToRespond(req_packet, res_cmd);
	return do_reply(requestor, res_packet, msg, force_bytes)
end

--
-- http
--
local g_http_event_root
local g_http_event_map ={
	get ={},
	post ={}
}
local function http_code_to_msg(code)
	if code == 200 then
		return "OK"
	elseif code == 400 then
		return "Bad Request"
	elseif code == 404 then
		return "Not Found"
	elseif code == 503 then
		return "Service Unavailable"
	else
		return "Unknown"
	end
end
local function on_http_event(method, path, func)
	-- check
	if #path == 0 then
		ERROR("set http event failed, path `%s` is empty", path)
		return
	end
	if not method or not g_http_event_map[method] then
		ERROR("set http event failed, method `%s` is invalid, must be get or post", method or 'nil')
		return
	end

	-- special for root
	if path == '/' then
		g_http_event_root =func
		return
	end

	-- for others
	local tb =g_http_event_map[method]
	local ls =string_split(path, '/', true)
	for i=1, #ls-1 do
		local n =ls[i]
		if is_nil(tb[n]) then
			tb[item] ={}
			tb =tb[item]
		elseif is_table(tb[n]) then
			tb =tb[item]
		elseif is_function(tb[n]) then
			ERROR("set http event failed, path `%s` conflict", path)
			return
		else
			ERROR("set http event failed, wtf")
			return
		end
	end
	local n =ls[#ls]
	tb[n] =func
end
local function http_respond_write(res, ...)
	if not res then
		ERROR("http respond write failed, res is nil")
		return nil, 'res is nil'
	end
	res.content =res.content or ''
	res.content =res.content .. sprintf(...)
	return true
end
local function http_respond_flush(res)
	if not res then
		ERROR("http respond flush failed, res is nil")
		return nil, 'res is nil'
	end
	if res.flushed then
		return
	end
	---- prepare
	local str

	---- head line
	str =sprintf("%s %d %s\r\n", res.version, res.code, res.msg or http_code_to_msg(res.code))

	---- header
	if not res.header['Content-Type'] then
		str =str .. 'Content-Type: text/html\r\n'
	end
	if not res.header['Content-Length'] then
		str =str .. sprintf("Content-Length: %d\r\n", res.content and #res.content or 0)
	end
	for k, v in pairs(res.header) do
		if is_string(k) and is_string(v) then
			str =str .. sprintf('%s: %s\r\n', k, v)
		end
	end
	for k, v in pairs(res.cookie) do
		if is_string(k) and is_table(v) and is_string(v.value) then
			local s =sprintf("Set-Cookie: %s=%s", k, v.value)
			if v.domain then
				s =s .. sprintf(';Domain=%s', v.domain)
			end
			if v.path then
				s =s .. sprintf(';Path=%s', v.path)
			end
			if v.secure then
				s =s .. ';Secure'
			end
			if v.max_age then
				s =s .. sprintf(';Max-Age=%d', v.max_age)
			end
			str =str .. s
		end
	end
	str =str .. '\r\n'

	---- content
	if res.content then
		assert(is_string(res.content))
		str =str .. res.content
	end
	
	---- send
	res.requestor:send(str)
	res.flushed =true
end
local function process_http_request(request)
	-- check
	local path =request.path
	assert(path and request.requestor)
	local method =request.method
	assert(is_string(method))
	method =string.lower(method)
	assert(g_http_event_map[method])

	print(path)
	-- new respond
	local respond ={
		requestor =request.requestor,
		version ='HTTP/1.1',
		code =200,
		header ={},
		cookie ={},
		write =http_respond_write,
		flush =http_respond_flush
	};

	-- parse request
	request.request ={}
	request.get =request.get or {}
	for k, v in pairs(request.get) do
		request.request[k] =v
	end
	request.post =request.post or {}
	for k, v in pairs(request.post) do
		request.request[k] =v
	end
	request.requestor =nil

	-- dispatch
	local tb =g_http_event_map[method]
	local ls =string_split(path, '/', true)
	for i=1, #ls do
		local n =ls[i]
		if is_nil(tb[n]) then
			break
		elseif is_table(tb[n]) then
			tb =tb[n]
		elseif is_function(tb[n]) then
			tb[n](request, respond);
			respond:flush()
			return true
		else
			ERROR("process http request failed, wtf")
			return nil, 'wtf'
		end
	end

	-- default
	if g_http_event_root then
		g_http_event_root(request, respond)
		respond:flush()
		return true
	else
		ERROR("process http request failed, path `%s` not config", path)
		return nil, sprintf("path `%s` not config", path)
	end
end

--
-- event
--
local _event_tb ={}
local function on_event(a, b, c)
	if is_number(a) and is_function(b) then
		local evt =a
		local func =b
		local not_use_protocol =c

		_event_tb[evt] ={
			func =func,
			use_protocol =not_use_protocol and false or true
		}
	elseif is_string(a) and (a=='update' or a=='load' or a=='unload') and is_function(b) then
		local evt =a
		local func =b

		_event_tb[evt] =func
	elseif is_string(a) and is_string(b) and is_function(c) then
		local method =a
		local path =b
		local func =c

		on_http_event(method, path, func)
	else
		ERROR("on event failed, not match")
	end
end

--
-- export to host
--
local function do_load(path)
	local fn =_event_tb.load
	if is_function(fn) then
		fn(path)
	end
end
local function do_update(now)
	update_timer(now)
	update_sleeper(now)
	update_task(now)
	update_rpc(now)

	local fn =_event_tb.update
	if is_function(fn) then
		do_mission(fn, now)
	end
end
local function do_logic_message(requestor, packet, body, object)
	if 0 ~= BitOp.And(packet.option, Packet.OPT_REQUEST) then
		return do_command(_event_tb[packet.command], requestor, packet, body, object)
	elseif 0 ~= BitOp.And(packet.option, Packet.OPT_RESPOND) then
		return do_resume_rpc(packet, body, object)
	else
		ERROR("wtf")
	end
end
local function do_http_message(request)
	do_mission(process_http_request, request)
end
local function do_unload()
	local fn =_event_tb.unload
	if is_function(fn) then
		fn()
	end
end

--
-- Service --
--
Service ={
	-- time
	Now =stable_now,

	-- mission
	Go =do_mission,

	-- protocol group
	SetProtocolGroupId =set_protocol_group_id,
	GetProtocolGroupId =get_protocol_group_id,

	-- event
	On =on_event,
	SetListener =on_event,

	-- timer
	SetTimer =set_timer,
	DelTimer =del_timer,
	ClearTimer =clear_timer,

	-- sleeper
	Sleep =sleep,

	-- rpc
	Rpc =do_rpc,

	-- notify
	Nofify =do_notify,

	-- reply
	Reply =do_reply,
	ReplyEasy =do_reply_easy,

	-- Debug
	Debug =function()
		WARN("service debug");
		for k, v in pairs(_rpc_table) do
			WARN("rpc key %d", k)
		end
	end
};

-- basic --
function Service.Self()
	return _G['SERVICE_INSTANCE']
end
function Service.Name()
	return _G['SERVICE_NAME']
end
function Service.Desc()
	return _G['SERVICE_DESC']
end
function Service.Id()
	return _G['SERVICE_ID']
end

-- export --
export{
	Service = Service,
	on_load =do_load,
	on_update =do_update,
	on_logic_message =do_logic_message,
	on_http_message =do_http_message,
	on_unload =do_unload,
};
