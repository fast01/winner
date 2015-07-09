/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CALLBACK_RPC_INFO_H__
#define H_CORE_CALLBACK_RPC_INFO_H__

namespace core{
	/** CallbackRpcInfo **/
	class CallbackRpcInfo: public RpcInfo{
		typedef RpcInfo Super;
		SUPPORT_NEWABLE
	public:
		typedef int64_t (*PFN_CALLBACK)(Object* param, Object* context);
	protected:
		CallbackRpcInfo(const int64_t group_id, Object* context);
		CallbackRpcInfo(const int64_t group_id, Object* context, PFN_CALLBACK callback);
		virtual ~CallbackRpcInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setContext(Object* context);
		Object* getContext();

		void setCallback(PFN_CALLBACK pfn);
		PFN_CALLBACK getCallback();
	public:
		virtual int64_t invoke(Object* param);
	private:
		int64_t _invoke(Object* param);
	protected:
		int64_t m_protocol_group_id;
		Object* m_context;
		PFN_CALLBACK m_callback;
	};
}

#endif

