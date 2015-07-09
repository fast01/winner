--[[
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
]]
module "Core"

Packet ={
	OPT_ZOMBIE  =1,
	OPT_REQUEST =2,
	OPT_RESPOND =4,
	OPT_BODY_IS_OBJECT_POINTER =8
}

Packet.Check =function(packet)
	packet.from =packet.from or 0
	packet.option =packet.option or 0
	return is_table(packet) 
		and is_number(packet.from)
		and is_number(packet.to)
		and is_number(packet.sn)
		and is_number(packet.who)
		and is_number(packet.command)
		and is_number(packet.option);
end

Packet.RequestToRespond =function(packet, res_command)
	if not Packet.Check(packet) then
		ERROR("fail to call Packet.RequestToRespond, invalid packet")
		return nil
	end
	if not is_number(res_command) then
		ERROR("fail to call Packet.RequestToRespond, invalid res_command")
		return nil
	end
	local res_packet ={
		from =packet.to,
		to =packet.from,
		sn =packet.sn,
		who =packet.who,
		command =res_command,
		option =Packet.OPT_RESPOND
	};
	return res_packet
end
