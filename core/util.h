/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_UTIL_H__
#define H_CORE_UTIL_H__

namespace core{
	/** fd **/
	void close_fd(int fd);
	void delete_fd_from_epoll(int epollfd, int fd);
	bool add_fd_to_epoll(int epollfd, int fd, int events, void* context);
	bool modify_fd_in_epoll(int epollfd, int fd, int events, void* context);

	/** sleep **/
	void sleep_sec(const int64_t sec);
	void sleep_ms(const int64_t ms);
	void sleep_us(const int64_t us);

	/** print stack **/
	void print_stack();

	/** string to number **/
	bool string_to_boolean(const char* str, bool& v);
	bool string_to_int64(const char* str, int64_t& v);
	bool string_to_uint64(const char* str, uint64_t& v);
	bool string_to_float64(const char* str, float64_t& v);
}


#endif
