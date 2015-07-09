/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** safe read write **/
	int64_t safe_read(const int64_t fd, char* ptr, const int64_t s){
		if(s == 0) return -1;
		if(fd == -1 || !ptr || s<0) return -1;
		int64_t cursor =0;
		while(cursor < s){
			const size_t n =read(fd, ptr+cursor, s - cursor);
			if(n > 0){
				cursor +=n;
			}
			else if(n == 0){ // EOF
				return cursor;
			}
			else{
				const int e =get_last_error();
				if(e == EAGAIN || e == EINTR) continue;
				return -1;
			}
		}
		ASSERT(cursor == s);
		return cursor;
	}
	int64_t safe_write(const int64_t fd, char* ptr, const int64_t s){
		if(s == 0) return -1;
		if(fd == -1 || !ptr || s<0) return -1;
		int64_t cursor =0;
		while(cursor < s){
			const size_t n =write(fd, ptr+cursor, s - cursor);
			if(n > 0){
				cursor +=n;
			}
			else if(n == 0){ // EOF
				return cursor;
			}
			else{
				const int e =get_last_error();
				if(e == EAGAIN || e == EINTR) continue;
				return -1;
			}
		}
		ASSERT(cursor == s);
		return cursor;
	}
	/** read write string **/
	String* read_string(const char* path){
		if(!path) return 0;
		if(Bytes* bs =read_bytes(path)){
			return String::NewString(bs->c_str(), bs->size());
		}
		else{
			return 0;
		}
	}
	int64_t write_string(const char* path, String* content){
		if(!path || !content) return -1;
		if(FILE* fp =fopen(path, "w")){
			const size_t s =(size_t)content->size();
			const char* ptr =content->c_str();
			size_t cursor =0;
			while(cursor < s){
				const size_t n =fwrite(ptr+cursor, 1, s-cursor, fp);
				if(n < 0){
					ERROR("fail to write %s, %s", path, get_last_error_desc());
					fclose(fp);
					return -1;
				}
				cursor +=n;
			}
			fclose(fp);
			ASSERT(cursor == s);
			return cursor;
		}
		else{
			ERROR("fail to open %s, %s", path, get_last_error_desc());
			return -1;
		}
	}

	/** read write bytes **/
	Bytes* read_bytes(const char* path){
		if(!path) return 0;
		if(FILE* fp =fopen(path, "rb")){
			if(0 != fseek(fp, 0, SEEK_END)){
				ERROR("fail read %s, %s", path, get_last_error_desc());
				fclose(fp);
				return 0;
			}

			const int64_t s =ftell(fp);
			if(s == -1){
				ERROR("fail read %s, %s", path, get_last_error_desc());
				fclose(fp);
				return 0;
			}

			if(0 != fseek(fp, 0, SEEK_SET)){
				ERROR("fail read %s, %s", path, get_last_error_desc());
				fclose(fp);
				return 0;
			}

			Bytes* content =SafeNew<Bytes>();
			content->resize(s);
			char* ptr =content->c_str();
			size_t cursor =0;
			while(cursor < (size_t)s){
				const size_t n =fread(ptr+cursor, 1, (size_t)s-cursor, fp);
				if(n > 0){
					cursor +=n;
				}
				else if(n == 0){
					if(ferror(fp)){
						return 0;
					}
					if(feof(fp)){
						return content;
					}
				}
				else{
					ERROR("fail read %s, %s", path, get_last_error_desc());
					fclose(fp);
					return 0;
				}
			}
			fclose(fp);
			ASSERT(cursor == (size_t)s);
			return content;
		}
		else{
			ERROR("fail to open %s, %s", path, get_last_error_desc());
			return 0;
		}
	}
	int64_t write_bytes(const char* path, Bytes* content){
		if(!path || !content) return -1;
		if(FILE* fp =fopen(path, "wb")){
			char* ptr =content->c_str();
			size_t cursor =0;
			const size_t s =(size_t)content->size();
			while(cursor < s){
				const size_t n =fwrite(ptr+cursor, 1, s-cursor, fp);
				if(n > 0){
					cursor +=n;
				}
				else if(n == 0){
					if(ferror(fp)){
						return -1;
					}
					if(feof(fp)){
						return cursor;
					}
				}
				else{
					ERROR("fail to write %s, %s", path, get_last_error_desc());
					fclose(fp);
					return -1;
				}
			}
			fclose(fp);
			ASSERT(cursor == s);
			return cursor;
		}
		else{
			ERROR("fail to open %s, %s", path, get_last_error_desc());
			return -1;
		}
	}

	/** FileSystem **/
	String* FileSystem::CleanPath(String* path){
		if(!path) return 0;
		if(path->is(".") || path->is("..")){
			return path;
		}
		path =path->replace("\\", "/", true);
		while(1){
			const int64_t lo =path->size();
			path =path->replace("//", "/", true);
			const int64_t ln =path->size();
			if(lo == ln){
				break;
			}
		}
		return path;
	}
	String* FileSystem::ParseDir(String* path){
		if(!path) return 0;
		if(path->is(".") || path->is("..")){
			return path;
		}
		const int64_t idx =path->lastIndexOf("/");
		if(idx == -1){
			return STR(".");
		}
		else{
			return path->subString(0, idx+1);
		}
	}
	String* FileSystem::ParseExt(String* path){
		if(!path) return 0;
		if(path->is(".") || path->is("..")){
			return STR("");
		}
		const int64_t idx =path->lastIndexOf(".");
		if(idx == -1){
			return STR("");
		}
		else{
			const int64_t tmp =path->lastIndexOf("/");
			if(tmp > idx){
				return STR(""); // "../" or "./" or "../x" or "./x"
			}
			else{
				return path->subString(idx+1, -1);
			}
		}
	}
	String* FileSystem::ParseName(String* path){
		if(!path) return 0;
		if(path->is(".") || path->is("..")){
			return STR("");
		}
		const int64_t idx =path->lastIndexOf("/");
		if(idx == -1){
			return path;
		}
		else{
			return path->subString(idx+1, -1);
		}
	}
	String* FileSystem::Join(String* a, String* b){
		if(!a || !b) return 0;
		if(b->hasPrefix("/")){
			return 0;
		}
		if(a->hasSubfix("/")){
			return a->append(b);
		}
		else{
			return a->append("/")->append(b);
		}
	}
}
