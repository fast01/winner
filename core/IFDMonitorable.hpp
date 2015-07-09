/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_H_I_FD_MONITORABLE_HPP__
#define H_CORE_H_I_FD_MONITORABLE_HPP__

namespace core{
	/** predeclare **/
	class FDMonitor;

	/** IFDMonitorable **/
	class IFDMonitorable{
	public:
		enum{
			ID =INTERFACE_ID_FD_MONITORABLE
		};
		enum{
			EVT_READ =1 << 0,
			EVT_WRITE =1 << 1,
			EVT_ERROR =1 << 2,
		};
	public:
		virtual Object* toObject() =0;
		virtual bool canReconnect() =0;
		virtual bool onEvent(const int64_t fd, const uint64_t events) =0;
		virtual bool onMonitor() =0;
		virtual void onDemonitor() =0;
		virtual bool reborn() =0;
		virtual void sucide() =0;
	};
}

#endif
