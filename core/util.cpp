/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/** fd **/
	void close_fd(int fd){
		while(::close(fd) != 0){
			if(get_last_error() != EINTR){
				break;
			}
		}
	}
	void delete_fd_from_epoll(int epollfd, int fd){
		if(0 != epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0)){
			if(get_last_error() == ENOENT){
				return;
			}
			else{
				ERROR("fail to call epoll_ctl EPOLL_CTL_DEL, %s", get_last_error_desc());
			}
		}
	}
	bool add_fd_to_epoll(int epollfd, int fd, int events, void* context){
		struct epoll_event evt;
		memset(&evt, 0, sizeof(evt));
		evt.events =events;
		evt.data.ptr =context;
		if(0 != epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &evt)){
			if(get_last_error() == EEXIST){
				return modify_fd_in_epoll(epollfd, fd, events, context);
			}
			else{
				ERROR("fail to call epoll_ctl EPOLL_CTL_ADD, %s", get_last_error_desc());
				return false;
			}
		}
		return true;
	}
	bool modify_fd_in_epoll(int epollfd, int fd, int events, void* context){
		struct epoll_event evt;
		memset(&evt, 0, sizeof(evt));
		evt.events =events;
		evt.data.ptr =context;
		if(0 != epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &evt)){
			if(get_last_error() != ENOENT){
				ERROR("fail to call epoll_ctl EPOLL_CTL_ADD, %s", get_last_error_desc());
			}
			return false;
		}
		return true;
	}

	/** sleep **/
	void sleep_sec(const int64_t sec){
		usleep(1000000 * sec);
	}
	void sleep_ms(const int64_t ms){
		usleep(1000 * ms);
	}
	void sleep_us(const int64_t us){
		usleep(us);
	}

	/** print stack **/
	void print_stack(){
		// prepare symbols
		void* arr[64] ={0};
		const size_t s =backtrace(arr, 64);
		if(s == 0){
			OutputDebug("print stack error");
			return;
		} 
		char** syms =backtrace_symbols(arr, s);

		// make str
		OutputDebug("print stack:\n");
		OutputDebug("\tstack size is %d.\n", (int)s);
		if(syms){
			for(size_t i=0; i<s; ++i){
				OutputDebug("\t[%d] %s\n", (int)i, (syms[i] ? syms[i] : ""));
			}
			free(syms);
		}
	}

	/** string to number **/
	bool string_to_boolean(const char* str, bool& v){
		if(!str) return false;
		if(strcasecmp(str, "true") == 0){
			v =true;
			return true;
		}
		else if(strcasecmp(str, "false") == 0){
			v =false;
			return true;
		}
		else{
			return false;
		}
	}
	bool string_to_int64(const char* str, int64_t& v){
		if(!str || *str==0) return false;
		char* ep =0;
		v =static_cast<int64_t>(strtoll(str, &ep, 0));
		return ep && *ep==0;
	}
	bool string_to_uint64(const char* str, uint64_t& v){
		if(!str || *str==0) return false;
		char* ep =0;
		v =static_cast<int64_t>(strtoull(str, &ep, 0));
		return ep && *ep==0;
	}
	bool string_to_float64(const char* str, float64_t& v){
		if(!str || *str==0) return false;
		char* ep =0;
		v =static_cast<float64_t>(strtod(str, &ep));
		return ep && *ep==0;
	}
}
