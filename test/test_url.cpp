/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_url(){
		OPH();
		DEBUG("testing url ......");
		Url* url =SafeNew<Url>();
		// parse
		url->parse(STR("http://fool:12345@www.baidu.com/user/setname?param=fool&param1=99#try"));
		ASSERT(url->getProtocol()->is("http"));
		ASSERT(url->getAuth()->is("fool:12345"));
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath()->is("/user/setname"));
		ASSERT(url->getQueryString()->is("param=fool&param1=99"));
		ASSERT(url->getFragment()->is("try"));

		url->parse(STR("http://www.baidu.com/user/setname?param=fool#try"));
		ASSERT(url->getProtocol()->is("http"));
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath()->is("/user/setname"));
		ASSERT(url->getQueryString()->is("param=fool"));
		ASSERT(url->getFragment()->is("try"));

		url->parse(STR("www.baidu.com/user/setname?param=fool#try"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath()->is("/user/setname"));
		ASSERT(url->getQueryString()->is("param=fool"));
		ASSERT(url->getFragment()->is("try"));

		url->parse(STR("www.baidu.com/user/setname?param=fool&x&y&z=8"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath()->is("/user/setname"));
		ASSERT(url->getQueryString()->is("param=fool&x&y&z=8"));
		ASSERT(url->getFragment() == 0);

		url->parse(STR("www.baidu.com/user/setname#try"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath()->is("/user/setname"));
		ASSERT(url->getQueryString() == 0);
		ASSERT(url->getFragment()->is("try"));

		url->parse(STR("www.baidu.com#try"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath() == 0);
		ASSERT(url->getQueryString() == 0);
		ASSERT(url->getFragment()->is("try"));

		url->parse(STR("www.baidu.com?#"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath() == 0);
		ASSERT(url->getQueryString()->is(""));
		ASSERT(url->getFragment()->is(""));

		url->parse(STR("www.baidu.com"));
		ASSERT(url->getProtocol() == 0);
		ASSERT(url->getAuth() == 0);
		ASSERT(url->getHost()->is("www.baidu.com"));
		ASSERT(url->getPath() == 0);
		ASSERT(url->getQueryString() == 0);
		ASSERT(url->getFragment() == 0);

		// build
		Hash* tb =SafeNew<Hash>();
		tb->set("param", STR("fool"));
		tb->set("param1", STR("99"));
		url->build(STR("http"), STR("fool:12345"), STR("www.baidu.com"), STR("/user/setname"), tb, STR("try"));
		ASSERT(url->getUrl()->is("http://fool:12345@www.baidu.com/user/setname?param=fool&param1=99#try")
				|| url->getUrl()->is("http://fool:12345@www.baidu.com/user/setname?param1=99&param=fool#try"));

		url->build(STR("http"), STR("fool:12345"), STR("www.baidu.com"), STR("/user/setname"), STR("param=fool&param1=99"), STR("try"));
		ASSERT(url->getUrl()->is("http://fool:12345@www.baidu.com/user/setname?param=fool&param1=99#try"));
	}	
}

