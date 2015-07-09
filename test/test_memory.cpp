/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_memory(){
		OPH();
		DEBUG("testing memory ......");
		
		static void* pl[10000] ={0};
		for(int i=0; i< 10000; ++i){
			pl[i] =ALLOCATE(43);
		}
		for(int i=0; i< 10000; ++i){
			DEALLOCATE(pl[i]);
		}

		/* performance check
		const int64_t N =100000;
		auto clk_a =clock();
		for(int64_t i=0; i<N; ++i){
			FREE(MALLOC(rand() % 8096 + 1));
		}
		auto clk_b =clock();
		void* p;
		for(int64_t i=0; i<N; ++i){
			p=(malloc(rand() % 8096+ 1));
		}
		auto clk_c =clock();

		printf("%p %f\n", p, (clk_c-clk_b) / (clk_b+0. - clk_a));
		*/

		// test memcpy
		PACKET src;
		src.size =1;
		PACKET dst;
		dst.size =0;
		PerformanceTest<100000>("memcpy", [&](){
			memcpy(&dst, &src, sizeof(src));
		});

		PerformanceTest<100000>("oph", [&](){
			OPH();
		});
		Object* x =SafeNew< Int64 >();
		PerformanceTest<100000>("SafeNew", [&](){
			OPH();
			x =SafeNew< Bytes >();
		});
	}
}
