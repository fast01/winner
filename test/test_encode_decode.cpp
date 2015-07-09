/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_encode_decode(){
		OPH();
		DEBUG("testing encode decode ......");
		// prepare
		PACKET packet0 ={0};
		packet0.size =0;
		packet0.from =10000;
		packet0.to =10000;
		packet0.sn =1000000;
		packet0.who =10000000;
		packet0.command =10000;
		packet0.option =100;
		char data[PACKET_ENCODE_MAX_SIZE] ={0};

		PerformanceTest<100000>("encode/decode PACKET", [&](){
			// encode
			packet0.size =0;
			int64_t len =encode_packet(data, sizeof(data), packet0);
			ASSERT(len > 0);
			packet0.size=len;
			
			// decode
			PACKET packet1 ={0};
			ASSERT(len == decode_packet(data, len, packet1));

			// DEBUG("%llu %llu", packet0.size, packet1.size);
			// DEBUG("%llu %llu", *(uint32_t*)&packet0, *(uint32_t*)&packet1);
			// DEBUG("%d", sizeof(packet0));
			ASSERT(0 == memcmp(&packet0, &packet1, sizeof(packet0)));
		});
	}
}

