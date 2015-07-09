/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_IDS_H__
#define H_CORE_IDS_H__

namespace core{
	/** command **/
	enum{
		CTRL_ADD_CONNECTION =1,
		CTRL_DEL_CONNECTION =2,
		CTRL_ADD_CLIENT =3,
		CTRL_DEL_CLIENT =4,
		CTRL_DISCONNECT =5,
		CTRL_QUIT =6,

		USER_COMMAND_MIN =10000
	};
	/** misc **/
	enum{
		INVALID_FD =-1
	};
}

#endif
