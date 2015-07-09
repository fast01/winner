/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_MD5_H__
#define H_CORE_MD5_H__

namespace core{
	/** Md5 **/
	class Md5{
	public:
		enum{
			BUFFER_LENGTH =(MD5_DIGEST_LENGTH) * 2
		};
	public:
		static bool Sum(const void* bytes, const int64_t len, char* pbuffer);
		static bool RegisterToLua(lua_State* L);
	private:
		static int _Sum(lua_State* L);
	};
}

#endif
