/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COROUTINE_RPC_INFO_H__
#define H_CORE_COROUTINE_RPC_INFO_H__

namespace core{
	/** predeclare **/
	class CoroutineService;

	/** CoroutineRpcInfo **/
	class CoroutineRpcInfo: public RpcInfo{
		SUPPORT_NEWABLE
		typedef RpcInfo Super;
	private:
		CoroutineRpcInfo(const int64_t group_id, const int64_t cr_id);
		virtual ~CoroutineRpcInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t timeout();
		virtual int64_t invoke(Object* param);
	public:
		int64_t getProtocolGroupId();
		int64_t getCoroutineId();
	public:
		void set(const int64_t rpc_id, CoroutineService* srv);
	private:
		int64_t m_protocol_group_id;
		int64_t m_coroutine_id;
		int64_t m_rpc_id;
		CoroutineService* m_coroutine_service; // weak ptr
	};
}

#endif
