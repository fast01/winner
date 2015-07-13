/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** impl Monitor ***/
	/** singleton **/
	DEFINE_THREAD_LOCAL_SINGLETON(Monitor)

	/** ctor & dtor **/
	Monitor::Monitor()
		: m_id(-1)
		, m_epoll_fd(INVALID_FD)
		, m_target_tb(0)
		, m_reborn_tb(0)
		, m_dying_tb(0)
		, m_fd_tb(0)
		, m_close_flag(false){
	}
	Monitor::~Monitor(){
		_close_epoll();
	}
	/** Object **/
	void Monitor::init(){
		Super::init();
		ASSIGN_POINTER(m_fd_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_target_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_reborn_tb, SafeNew<Hash>());
		ASSIGN_POINTER(m_dying_tb, SafeNew<Hash>());
	}
	void Monitor::finalize(){
		_clean();
		Super::finalize();
	}

	/** self **/
	void Monitor::setId(const int64_t id){
		m_id =id;
	}
	int64_t Monitor::getId(){
		return m_id;
	}
	bool Monitor::watch(const int64_t timeout){
		OPH();
		if(m_close_flag){
			return false;
		}
		if(!m_target_tb){
			WARN("monitor already closed");
			return false;
		}
		if(!_check_epoll()){
			return false;
		}
		// prepare
		const int64_t now = DateTime::Now();
		const int64_t reborn_time =now + REBORN_TIMER;

		// wait event
		const int n =epoll_wait(m_epoll_fd, m_event_list, EVENT_LIST_MAX_COUNT, MAX(timeout, 500));
		if(-1 == n){
			if(get_last_error() == EINTR){
				WARN("loop epoll_wait interrupt, %s", get_last_error_desc());
				return true;
			}
			else{
				ERROR("loop epoll_wait error, %s", get_last_error_desc());
				ERROR("monitor closed now");
				_clean();
				return false;
			}
		}
		// process event
		for(int i=0; i<n; ++i){
			struct epoll_event* pevt =m_event_list + i;
			const uint64_t events =_translate_from_epoll_events(pevt->events);
			Array* data =reinterpret_cast< Array* >(pevt->data.ptr);
			MonitorTarget* target =static_cast< MonitorTarget* >(data->get(0));
			Int64* fd =static_cast< Int64* >(data->get(1));

			// log
			// if(dynamic_cast< TcpConnection* >(target)){
			//	printf("event %p %p %lld %llu\n", (void*)data, (void*)target, (long long)(fd->getValue()), (unsigned long long)events);
			// }
			// end

			if(target->onEvent(fd->getValue(), events)){
				if(target->isHeartBeatEnable()){
					target->heartBeat(now);
					m_dying_tb->set(target, target);
				}
			}
			else{
				if(target->canReborn()){
					target->onDetachEvent();
					target->sucide();
					m_reborn_tb->set(target, SafeNew<Int64>(reborn_time));
				}
				else{
					_abandon(target);
				}
			}
		}

		// reborn
		if(m_reborn_tb->size()){
			HashIterator* it =static_cast< HashIterator* >(m_reborn_tb->iterator());
			while(it->next()){
				Int64* t =static_cast< Int64* >(it->getValue());
				if(now >= t->getValue()){
					MonitorTarget* target =static_cast< MonitorTarget* >(it->getKey());
					if(target->reborn() && target->onAttachEvent(this)){
						it->remove();
					}
					else{
						t->setValue(reborn_time);
						target->sucide();
					}
				}
			}
			m_reborn_tb->optimize(m_reborn_tb->size() * 3);
		}

		// remove dead
		if(m_dying_tb->size()){
			HashIterator* it =static_cast< HashIterator* >(m_dying_tb->iterator());
			while(it->next()){
				MonitorTarget* target =static_cast< MonitorTarget* >(it->getKey());
				if(now >= target->getDeadTime()){
					target->onDetachEvent();
					target->onAbandon();
					target->sucide();
					it->remove();
					m_target_tb->remove(target);
					m_reborn_tb->remove(target);
				}
			}
			m_dying_tb->optimize(m_dying_tb->size() * 3);
		}
		// return
		if(m_close_flag){
			_clean();
			return false;
		}
		return true;
	}
	void Monitor::close(){
		m_close_flag =true;
	}
	bool Monitor::monitor(MonitorTarget* target){
		ASSERT(this == Monitor::Instance());
		if(!target || !target->canMonitor()){
			return false;
		}
		if(!m_target_tb){
			WARN("monitor already closed");
			return false;
		}
		if(m_target_tb->has(target)){
			return true;
		}
		if(!_check_epoll()){
			return false;
		}
		if(target->onAttachEvent(this)){
			const int64_t now =DateTime::Now();
			m_target_tb->set(target, SafeNew<Int64>(now));
			if(target->isHeartBeatEnable()){
				target->heartBeat(now);
				m_dying_tb->set(target, target);
			}
			return true;
		}
		else{
			target->sucide();
			if(target->canReborn()){
				const int64_t now =DateTime::Now();
				m_target_tb->set(target, SafeNew<Int64>(now));
				m_reborn_tb->set(target, SafeNew<Int64>(now + REBORN_TIMER));
				if(target->isHeartBeatEnable()){
					target->heartBeat(now);
					m_dying_tb->set(target, target);
				}
				return true;
			}
			else{
				return false;
			}
		}
	}
	void Monitor::demonitor(MonitorTarget* target){
		if(!target){
			return;
		}
		ASSERT(this == Monitor::Instance());
		if(!_check_epoll()){
			return;
		}
		ASSERT(target->getMonitorId()==getId());
		_abandon(target);
	}
	/** event op **/
	bool Monitor::attachEvent(const int fd, const int64_t events, MonitorTarget* target){
		if(fd==INVALID_FD || !events || !target || !_check_epoll()){
			return false;
		}
		// prepare data
		bool already_set;
		Array* data =_make_fd_data(fd, events, target, already_set);
		if(already_set){
			return true;
		}

		// monitor
		struct epoll_event evt;
		memset(&evt, 0, sizeof(evt));
		evt.events =_translate_to_epoll_events(events);
		evt.data.ptr =reinterpret_cast< void* >(data);

		if(0 == epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &evt)){
			// log
			// if(dynamic_cast< TcpConnection* >(target)){
			//	printf("attach add %p %p %lld\n", (void*)data, (void*)target, (long long)fd);
			// }
			return true;
		}
		else{
			if(get_last_error() == EEXIST){
				if(0 == epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &evt)){
					// log
					// if(dynamic_cast< TcpConnection* >(target)){
					//	printf("attach modify %p %p %lld\n", (void*)data, (void*)target, (long long)fd);
					// }
					return true;
				}
				else{
					m_fd_tb->remove(static_cast<int64_t>(fd));
					ERROR("fail to call epoll_ctl EPOLL_CTL_MOD, %s", get_last_error_desc());
					return false;
				}
			}
			else{
				m_fd_tb->remove(static_cast<int64_t>(fd));
				ERROR("fail to call epoll_ctl EPOLL_CTL_ADD, %s", get_last_error_desc());
				return false;
			}
		}
	}
	bool Monitor::modifyEvent(const int fd, const int64_t events, MonitorTarget* target){
		if(fd==INVALID_FD || !events || !target || !_check_epoll()){
			return false;
		}
		// prepare data
		bool already_set;
		Array* data =_make_fd_data(fd, events, target, already_set);
		if(already_set){
			return true;
		}

		// monitor
		struct epoll_event evt;
		memset(&evt, 0, sizeof(evt));
		evt.events =_translate_to_epoll_events(events);
		evt.data.ptr =reinterpret_cast< void* >(data);
		if(0 == epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &evt)){
			// log
			// if(dynamic_cast< TcpConnection* >(target)){
			//	printf("modify mod %p %p %lld\n", (void*)data, (void*)target, (long long)fd);
			// }
			return true;
		}
		else{
			if(get_last_error() == ENOENT){
				if(0 == epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &evt)){
					// log
					// if(dynamic_cast< TcpConnection* >(target)){
					//	printf("modify add %p %p %lld\n", (void*)data, (void*)target, (long long)fd);
					// }
					return true;
				}
				else{
					m_fd_tb->remove(static_cast<int64_t>(fd));
					ERROR("fail to call epoll_ctl EPOLL_CTL_ADD, %s", get_last_error_desc());
					return false;
				}
			}
			else{
				m_fd_tb->remove(static_cast<int64_t>(fd));
				ERROR("fail to call epoll_ctl EPOLL_CTL_MOD, %s", get_last_error_desc());
				return false;
			}
		}
	}
	void Monitor::detachEvent(const int fd){
		if(fd==INVALID_FD || !_check_epoll()){
			return;
		}
		m_fd_tb->remove(fd);

		// log
		// printf("detach rm\n");
		// end

		if(0 != epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, 0)){
			if(get_last_error() == ENOENT){
				return;
			}
			else{
				ERROR("fail to call epoll_ctl EPOLL_CTL_DEL, %s", get_last_error_desc());
			}
		}
	}
	/** helper **/
	bool Monitor::_check_epoll(){
		if(m_epoll_fd != INVALID_FD){
			return true;
		}
		else{
			m_epoll_fd =::epoll_create(EPOLL_SIZE);
			if(m_epoll_fd != INVALID_FD){
				return true;
			}
			else{
				ERROR("fail to call epoll_create, %s", get_last_error_desc());
				return false;
			}
		}
	}
	void Monitor::_close_epoll(){
		if(m_epoll_fd != INVALID_FD){
			close_fd(m_epoll_fd);
			m_epoll_fd =INVALID_FD;
		}
	}
	uint64_t Monitor::_translate_from_epoll_events(const uint64_t evts){
		uint64_t events =0;
		if(evts & EPOLLIN){
			events |= MonitorTarget::EVT_READ;
		}
		if(evts & EPOLLOUT){
			events |= MonitorTarget::EVT_WRITE;
		}
		if(evts&EPOLLERR || evts&EPOLLHUP){
			events |= MonitorTarget::EVT_ERROR;
		}
		return events;
	}
	uint64_t Monitor::_translate_to_epoll_events(const uint64_t evts){
		uint64_t events =0;
		if(evts & MonitorTarget::EVT_READ){
			events |= EPOLLIN;
		}
		if(evts & MonitorTarget::EVT_WRITE){
			events |= EPOLLOUT;
		}
		if(evts & MonitorTarget::EVT_ERROR){
			events |= EPOLLERR;
		}
		return events;
	}
	void Monitor::_abandon(MonitorTarget* target){
		if(target && m_target_tb->has(target)){
			target->onDetachEvent();
			target->onAbandon();
			target->sucide();

			m_target_tb->remove(target);
			m_dying_tb->remove(target);
			m_reborn_tb->remove(target);
		}
	}
	void Monitor::Close(){
		if(auto self =Monitor::Instance()){
			self->close();
		}
	}
	void Monitor::_clean(){
		OPH();
		// clear target
		if(m_target_tb){
			HashIterator* it =static_cast< HashIterator* >(m_target_tb->iterator());
			while(it->next()){
				MonitorTarget* target =static_cast< MonitorTarget* >(it->getKey());
				target->onDetachEvent();
				target->onAbandon();
				target->sucide();
			}
		}

		// close epoll
		_close_epoll();

		// clean data struct
		CLEAN_POINTER(m_fd_tb);
		CLEAN_POINTER(m_target_tb);
		CLEAN_POINTER(m_reborn_tb);
		CLEAN_POINTER(m_dying_tb);

		m_close_flag =true;
	}
	Array* Monitor::_make_fd_data(const int fd, const int64_t events, MonitorTarget* target, bool& already_set){
		already_set =false;
		Array* data =static_cast< Array* >(m_fd_tb->get((int64_t)fd));
		if(data){
			// fd
			Int64* pfd =static_cast< Int64* >(data->get(1));
			ASSERT(pfd && pfd->getValue()==fd);

			// events
			Int64* evts =static_cast< Int64* >(data->get(2));
			ASSERT(evts);
			if(evts->getValue() == events){
				already_set =true;
				// log
				// printf("already set\n");
				// end
				return data;
			}
			else{
				evts->setValue(events);
				return data;
			}
		}
		else{
			data =SafeNew<Array>();
			data->push_back(target);
			data->push_back(SafeNew<Int64>(fd));
			data->push_back(SafeNew<Int64>(events));
			m_fd_tb->set(static_cast<int64_t>(fd), data);
			return data;
		}
	}
}
