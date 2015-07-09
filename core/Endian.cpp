/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** packet size **/
	packet_size_t net_to_host_packet_size(packet_size_t v){
		return ntohl(v);
	}
	packet_size_t host_to_net_packet_size(packet_size_t v){
		return htonl(v);
	}

	/** Endian **/
	uint16_t Endian::Net2HostU16(const uint16_t v){
		return ntohs(v);
	}
	uint32_t Endian::Net2HostU32(const uint32_t v){
		return ntohl(v);
	}

	uint16_t Endian::Host2NetU16(const uint16_t v){
		return htons(v);
	}
	uint32_t Endian::Host2NetU32(const uint32_t v){
		return htonl(v);
	}
}
