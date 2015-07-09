/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_RPC_CALLBACK_H__
#define H_CORE_RPC_CALLBACK_H__

namespace core{
	/** RpcInvoker **/
	class RpcInvoker: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		typedef int64_t (*PFN_FUNC)(Object* arg);
	protected:
		RpcInvoker();
		virtual ~RpcInvoker();
	public:
		virtual void finalize();
	public:
		virtual int64_t run(Object* arg);
	private:
		PFN_FUNC m_pfn;
	};
}

#endif

