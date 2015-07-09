/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ENDIAN_H__
#define H_CORE_ENDIAN_H__

namespace core{
	packet_size_t net_to_host_packet_size(packet_size_t v);
	packet_size_t host_to_net_packet_size(packet_size_t v);

	/** Endian **/
	class Endian{
	public:
		static uint16_t Net2HostU16(const uint16_t v);
		static uint32_t Net2HostU32(const uint32_t v);

		static uint16_t Host2NetU16(const uint16_t v);
		static uint32_t Host2NetU32(const uint32_t v);
	};
}


#endif
