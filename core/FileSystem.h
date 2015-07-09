/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FILESYSTEM_H__
#define H_CORE_FILESYSTEM_H__

namespace core{
	/** safe read write **/
	int64_t safe_read(const int64_t fd, char* ptr, const int64_t s);
	int64_t safe_write(const int64_t fd, char* ptr, const int64_t s);

	/** read write string **/
	String* read_string(const char* path);
	int64_t write_string(const char* path, String* content);

	/** read write bytes **/
	Bytes* read_bytes(const char* path);
	int64_t write_bytes(const char* path, Bytes* content);

	/** FileSystem **/
	class FileSystem{
	public:
		static String* CleanPath(String* path);
		static String* ParseDir(String* path);
		static String* ParseExt(String* path);
		static String* ParseName(String* path);
		static String* Join(String* a, String* b);
	};

}

#endif
