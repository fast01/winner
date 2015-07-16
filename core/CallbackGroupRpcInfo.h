/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CALLBACK_GROUP_RPC_INFO_H__
#define H_CORE_CALLBACK_GROUP_RPC_INFO_H__

namespace core{
	/** CallbackGroupRpcInfo **/
	class CallbackGroupRpcInfo: public RpcInfo{
		SUPPORT_NEWABLE
		typedef RpcInfo Super;
	public:
		enum{
			STATE_ERROR =-1,
			STATE_PROCESSING =0,
			STATE_COMPLETED =1,
		};
		typedef int64_t (*PFN_CALLBACK)(CallbackGroupRpcInfo* self);
	protected:
		CallbackGroupRpcInfo(Object* context);
		CallbackGroupRpcInfo(Object* context, PFN_CALLBACK callback);
		virtual ~CallbackGroupRpcInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t timeout();
		virtual int64_t invoke(Object* param);
	public:
		void setContext(Object* context);
		Object* getContext();

		void setCallback(PFN_CALLBACK pfn);
		PFN_CALLBACK getCallback();
	public:
		bool begin();
		void cancel();
		void end();
	public:
		bool rpc(PACKET& packet, Object* req_param, CallbackRpcInfo* rpc_info);
		bool rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, CallbackRpcInfo* rpc_info);
	public:
		int64_t getCount();
		int64_t getState(const int64_t idx);
		Object* getValue(const int64_t idx);
		CallbackRpcInfo* getRpcInfo(const int64_t idx);
		int64_t getSuccessCount();
		int64_t getErrorCount();
	private:
		Object* m_context;
		PFN_CALLBACK m_callback;

		Array* m_rpc_array;
		Int64Array* m_state_array;
		Array* m_value_array;

		int64_t m_success_count;
		int64_t m_error_count;

		int64_t m_sn;
	};
}

#endif
