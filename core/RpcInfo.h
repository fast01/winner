/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_RPC_INFO_H__
#define H_CORE_RPC_INFO_H__

namespace core{
	/** predecl  **/
	class Command;

	/** RpcInfo **/
	class RpcInfo: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		enum{
			TTL =30 // 30 secs
	   	};
	protected:
		RpcInfo();
		virtual ~RpcInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setId(const int64_t id);
		int64_t getId();
	public:
		void setCommand(Command* cmd);
		Command* getCommand();
	public:
		void setExpireTime(const int64_t expire_time);
		int64_t getExpireTime();
	public:
		void set(const int64_t id, Command* cmd, const int64_t expire_time);
		virtual int64_t invoke(Object* param) =0;
	protected:
		int64_t m_id;
		Command* m_command;
		int64_t m_expire_time;
	};
}

#endif
