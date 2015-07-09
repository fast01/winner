/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_PACKET_H__
#define H_CORE_PACKET_H__

namespace core{
/*
	PACKET
*/
typedef struct tagPACKET{
	packet_size_t size;
	uint64_t from;
	uint64_t to;
	uint64_t who;
	uint64_t sn;
	uint64_t command;
	uint64_t option;
}PACKET, *PPACKET;

enum{
	OPT_ZOMBIE  =1 << 0,
	OPT_REQUEST =1 << 1,
	OPT_RESPOND =1 << 2,
	OPT_BODY_IS_OBJECT_POINTER =1 << 3
};

enum{
	PACKET_HEADER_SIZE =4,
	PACKET_ENCODE_MAX_SIZE =64
};

inline PACKET make_packet(uint64_t from, uint64_t to, uint64_t who, uint64_t sn, uint64_t command, uint64_t option){
	PACKET packet;
	packet.size =0;
	packet.from =from;
	packet.to =to;
	packet.who =who;
	packet.sn =sn;
	packet.command =command;
	packet.option =option;
	return packet;
}


/*
	MONITOR CONTROL PACKET
*/
class MonitorTarget;
typedef struct tagMONITOR_CTRL_PACKET{
	enum{
		CMD_MONITOR_TARGET,
		CMD_DEMONITOR_TARGET
	};
	uint64_t command;
	uint64_t option;
	MonitorTarget* target;
}MONITOR_CTRL_PACKET, *PMONITOR_CTRL_PACKET;

}
#endif
