/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_SERVICE_IDS_H__
#define H_SERVICE_IDS_H__

namespace service{
	/** service ids **/
	enum{
		SERVICE_ID_UNUSED,
		SERVICE_ID_LOG  =::core::LogService::SERVICE_ID,
		SERVICE_ID_S1_LUA =17,
		SERVICE_ID_S2_CALLBACK =18,
		SERVICE_ID_S3_COROUTINE =19,
		SERVICE_ID_S4_LUA =20,
		SERVICE_ID_S5_LUA =21,
	};

	/** log ids **/
	enum{
		LOG_ID_TEST =1,
		LOG_ID_S1_LUA =2,
		LOG_ID_S2_CALLBACK =3,
		LOG_ID_S3_COROUTINE =4,
		LOG_ID_S4_LUA =5,
		LOG_ID_S5_LUA =6
	};
}

#endif
