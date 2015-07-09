/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_string(){
		OPH();
		DEBUG("testing string ......");

		CHECK_EXIT(String::Format("%d + %d=%d", 1, 1, 1+1)->is("1 + 1=2"), 1);
		CHECK_EXIT(String::NewString("1234567890")->hasPrefix("123"), 1);
		CHECK_EXIT(String::NewString("1234567890")->hasSubfix("890"), 1);
		CHECK_EXIT(String::NewString("1234567890")->hasPrefix("23")==false, 1);
		CHECK_EXIT(String::NewString("1234567890")->hasSubfix("89")==false, 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(0, 2)->is("12"), 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(1, 2)->is("23"), 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(8, 2)->is("90"), 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(0, 10)->is("1234567890"), 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(0, 1000)->is("1234567890"), 1);
		CHECK_EXIT(String::NewString("1234567890")->subString(-110, 1000)->is("1234567890"), 1);
		CHECK_EXIT(String::Repeat('1', 5)->is("11111"), 1);
		CHECK_EXIT(String::NewString("112233")->replace("1", "0", true)->is("002233"), 1);
		CHECK_EXIT(String::NewString("##112233")->replace("1", "0", true)->is("##002233"), 1);
		CHECK_EXIT(String::NewString("112233")->replace("3", "0", true)->is("112200"), 1);
		CHECK_EXIT(String::NewString("112233")->replace("1", "0", false)->is("012233"), 1);
		CHECK_EXIT(String::NewString("##112233")->replace("1", "0", false)->is("##012233"), 1);
		CHECK_EXIT(String::NewString("112233")->replace("3", "0", false)->is("112203"), 1);
		CHECK_EXIT(String::NewString("     112233")->ltrim()->is("112233"), 1);
		CHECK_EXIT(String::NewString("112233   ")->rtrim()->is("112233"), 1);
		CHECK_EXIT(String::NewString(" 112233  ")->ltrim()->is("112233  "), 1);
		CHECK_EXIT(String::NewString("  112233 ")->rtrim()->is("  112233"), 1);
		CHECK_EXIT(String::NewString("     112233")->trim()->is("112233"), 1);
		CHECK_EXIT(String::NewString("112233   ")->trim()->is("112233"), 1);
		CHECK_EXIT(String::NewString("     112233   ")->trim()->is("112233"), 1);
		CHECK_EXIT(String::NewString("        ")->ltrim()->is(""), 1);
		CHECK_EXIT(String::NewString("        ")->rtrim()->is(""), 1);
		CHECK_EXIT(String::NewString("        ")->trim()->is(""), 1);

		String* str =String::NewString(",,,fool,winne,,11,,,,,");
		Array* tks =str->split(",");
		CHECK_EXIT(tks && tks->size()==3, 1);
		CHECK_EXIT(((String*)tks->at(0))->is("fool"), 1);
		CHECK_EXIT(((String*)tks->at(1))->is("winne"), 1);
		CHECK_EXIT(((String*)tks->at(2))->is("11"), 1);

		// fromstring
		int64_t int_val =0;
		CHECK_EXIT(core::FromString< int64_t >(STR("-12345"), int_val) && int_val==-12345, 1);
		CHECK_EXIT(core::FromString< int64_t >(STR("a12345"), int_val) == false, 1);

		uint64_t uint_val =0;
		CHECK_EXIT(core::FromString< uint64_t >(STR("12345"), uint_val) && uint_val==12345, 1);
		CHECK_EXIT(core::FromString< uint64_t >(STR("a12345"), uint_val) == false, 1);

		bool bool_val;
		CHECK_EXIT(core::FromString< bool >(STR("1"), bool_val) && bool_val==true, 1);
		CHECK_EXIT(core::FromString< bool >(STR("true"), bool_val) && bool_val==true, 1);
		CHECK_EXIT(core::FromString< bool >(STR("True"), bool_val) && bool_val==true, 1);
		CHECK_EXIT(core::FromString< bool >(STR("TRUE"), bool_val) && bool_val==true, 1);
		CHECK_EXIT(core::FromString< bool >(STR("0"), bool_val) && bool_val==false, 1);
		CHECK_EXIT(core::FromString< bool >(STR("false"), bool_val) && bool_val==false, 1);
		CHECK_EXIT(core::FromString< bool >(STR("False"), bool_val) && bool_val==false, 1);
		CHECK_EXIT(core::FromString< bool >(STR("FALSE"), bool_val) && bool_val==false, 1);
		CHECK_EXIT(core::FromString< bool >(STR("99"), bool_val)==false, 1);

		float32_t float_val =0;
		CHECK_EXIT(core::FromString< float32_t >(STR("123.45"), float_val) && fabs(float_val-123.45) <= 0.001, 1);
		CHECK_EXIT(core::FromString< float32_t >(STR("a123.45"), float_val) == false, 1);

		// performance test
		PerformanceTest<100000>("String::Format", [](){
			OPH();
			String::Format("select * from x where %d %d %s", 1, 22222, __func__);
		});

		static char sz[1024];
		PerformanceTest<100000>("sprintf", [=](){
			sprintf(sz, "select * from x where %d %d %s", 1, 22222, __func__);
		});

		int x, y;
		PerformanceTest<100000>("sscanf", [&](){
			sscanf("222 987 yyuhgug", "%d %d %s", &x, &y, sz);
		});

		int64_t sum =0;
		const char* s ="assasdasaaaaaaaaaaaaaaaaaaaaaaaa:wasasasasaaaaaaaaaaaaaaaaaaaaaaaaaaaaadasddddd";
		PerformanceTest<100000>("strstr", [&](){
			sum +=strstr(s, "d") - s;
		});

		PerformanceTest<100000>("strchr", [&](){
			sum +=strchr(s, 'd') - s;
		});
	}
}
