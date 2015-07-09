/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"

namespace test{
	void test_file_system(){
		OPH();
		DEBUG("testing file system ......");
		// clean path
		ASSERT(FileSystem::CleanPath(STR("."))->is("."));	
		ASSERT(FileSystem::CleanPath(STR(".."))->is(".."));	
		ASSERT(FileSystem::CleanPath(STR(".////"))->is("./"));	
		ASSERT(FileSystem::CleanPath(STR("..//"))->is("../"));	
		ASSERT(FileSystem::CleanPath(STR("/"))->is("/"));	
		ASSERT(FileSystem::CleanPath(STR("////x"))->is("/x"));	
		ASSERT(FileSystem::CleanPath(STR("///x///"))->is("/x/"));	
		ASSERT(FileSystem::CleanPath(STR("/x///y"))->is("/x/y"));	
		ASSERT(FileSystem::CleanPath(STR("/x//y///"))->is("/x/y/"));	

		// parse dir
		ASSERT(FileSystem::ParseDir(STR("."))->is("."));	
		ASSERT(FileSystem::ParseDir(STR(".."))->is(".."));	
		ASSERT(FileSystem::ParseDir(STR("./"))->is("./"));	
		ASSERT(FileSystem::ParseDir(STR("../"))->is("../"));	
		ASSERT(FileSystem::ParseDir(STR("/"))->is("/"));	
		ASSERT(FileSystem::ParseDir(STR("/x"))->is("/"));	
		ASSERT(FileSystem::ParseDir(STR("/x/y"))->is("/x/"));	
		ASSERT(FileSystem::ParseDir(STR("/x/y/"))->is("/x/y/"));	
		ASSERT(FileSystem::ParseDir(STR("/x/y//"))->is("/x/y//"));	

		// parse name
		ASSERT(FileSystem::ParseName(STR("."))->is(""));	
		ASSERT(FileSystem::ParseName(STR(".."))->is(""));	
		ASSERT(FileSystem::ParseName(STR("./"))->is(""));	
		ASSERT(FileSystem::ParseName(STR("../"))->is(""));	
		ASSERT(FileSystem::ParseName(STR("./x"))->is("x"));	
		ASSERT(FileSystem::ParseName(STR("../x"))->is("x"));	
		ASSERT(FileSystem::ParseName(STR("/"))->is(""));	
		ASSERT(FileSystem::ParseName(STR("/x"))->is("x"));	
		ASSERT(FileSystem::ParseName(STR("/x/y"))->is("y"));	
		ASSERT(FileSystem::ParseName(STR("/x/y/"))->is(""));	
		ASSERT(FileSystem::ParseName(STR("/x/y//"))->is(""));	

		// parse ext
		ASSERT(FileSystem::ParseExt(STR("."))->is(""));	
		ASSERT(FileSystem::ParseExt(STR(".."))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("./"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("../"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("./x"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("../x"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("./x.lua"))->is("lua"));	
		ASSERT(FileSystem::ParseExt(STR("../x.cpp"))->is("cpp"));	
		ASSERT(FileSystem::ParseExt(STR("/"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("/.c"))->is("c"));	
		ASSERT(FileSystem::ParseExt(STR("/x"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("/x/y"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("/x/y/"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("/x/y//"))->is(""));	
		ASSERT(FileSystem::ParseExt(STR("/x.c"))->is("c"));	
		ASSERT(FileSystem::ParseExt(STR("/x/y.c"))->is("c"));	
		ASSERT(FileSystem::ParseExt(STR("/x/y/.c"))->is("c"));	
		ASSERT(FileSystem::ParseExt(STR("/x/y//.c"))->is("c"));	


		// join
		ASSERT(FileSystem::Join(STR(".."), STR("x/y"))->is("../x/y"));
		ASSERT(FileSystem::Join(STR("."), STR("x/y"))->is("./x/y"));
		ASSERT(FileSystem::Join(STR("../"), STR("x/y"))->is("../x/y"));
		ASSERT(FileSystem::Join(STR("./"), STR("x/y"))->is("./x/y"));
		ASSERT(FileSystem::Join(STR("/"), STR("x/y"))->is("/x/y"));
	}
}
