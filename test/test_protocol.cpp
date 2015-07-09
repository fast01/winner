/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "test.h"
#include "../gen/protocol/protocol.h"

namespace test{
	void test_protocol(){
		OPH();
		lua_State* L =luaL_newstate();
		if(!L){
			ERROR("fail to new lua state");
		}
		CHECK_EXIT(L!=0, 1);
		DEBUG("testing protocol ......");

		// Game
		::protocol::Game* game =SafeNew<::protocol::Game>();
		game->setId(1987);
		game->setName(STR("football"));
		game->setDesc(STR("foot ball"));

		Bytes* bytes =game->toBytes(0);
		game =0;
		CHECK_EXIT(FromBytes< ::protocol::Game* >(bytes, game), 1);
		CHECK_EXIT(game->getId()==1987, 1);
		CHECK_EXIT(game->getName()->is("football"), 1);
		CHECK_EXIT(game->getDesc()->is("foot ball"), 1);
		
		ToLua< ::protocol::Game* >(L, game);
		game =0;
		CHECK_EXIT(FromLua< ::protocol::Game* >(L, -1, game), 1);
		CHECK_EXIT(game->getId()==1987, 1);
		CHECK_EXIT(game->getName()->is("football"), 1);
		CHECK_EXIT(game->getDesc()->is("foot ball"), 1);

		// Human
		::protocol::Human* human =core::SafeNew<::protocol::Human>();
		human->setFavorite(1, game);
		human->appendBook(STR("c++"));
		human->appendBook(STR("c"));
		human->appendBook(STR("java"));

		bytes =human->toBytes(0);
		CHECK_EXIT(FromBytes< ::protocol::Human* >(bytes, human), 1);
		CHECK_EXIT(human->getId()==0, 1);
		CHECK_EXIT(human->getName()->empty(), 1);
		CHECK_EXIT(human->getDesc()->empty(), 1);
		CHECK_EXIT(human->getBookCount()==3, 1);
		CHECK_EXIT(human->getBook(0)->is("c++"), 1);
		CHECK_EXIT(human->getBook(1)->is("c"), 1);
		CHECK_EXIT(human->getBook(2)->is("java"), 1);
		CHECK_EXIT(human->getFavorite(0)==0, 1);
		CHECK_EXIT(human->getFavorite(1)!=0, 1);
		CHECK_EXIT(human->getFavorite(2)==0, 1);
		game =human->getFavorite(1);
		CHECK_EXIT(game->getId()==1987, 1);
		CHECK_EXIT(game->getName()->is("football"), 1);
		CHECK_EXIT(game->getDesc()->is("foot ball"), 1);
		
		ToLua< ::protocol::Human* >(L, human);
		human =0;
		CHECK_EXIT(FromLua< ::protocol::Human* >(L, -1, human), 1);
		CHECK_EXIT(human->getId()==0, 1);
		CHECK_EXIT(human->getName()->empty(), 1);
		CHECK_EXIT(human->getDesc()->empty(), 1);
		CHECK_EXIT(human->getBookCount()==3, 1);
		CHECK_EXIT(human->getBook(0)->is("c++"), 1);
		CHECK_EXIT(human->getBook(1)->is("c"), 1);
		CHECK_EXIT(human->getBook(2)->is("java"), 1);
		CHECK_EXIT(human->getFavorite(0)==0, 1);
		CHECK_EXIT(human->getFavorite(1)!=0, 1);
		CHECK_EXIT(human->getFavorite(2)==0, 1);
		game =human->getFavorite(1);
		CHECK_EXIT(game->getId()==1987, 1);
		CHECK_EXIT(game->getName()->is("football"), 1);
		CHECK_EXIT(game->getDesc()->is("foot ball"), 1);

		Bytes* str =core::SafeNew<Bytes>();
		human->appendToString(str, -1);
		// puts(str->c_str());

		// clean
		lua_close(L);
	}
}
