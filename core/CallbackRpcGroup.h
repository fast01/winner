/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CALLBACK_RPC_GROUP_H__
#define H_CORE_CALLBACK_RPC_GROUP_H__

namespace core{
	/** predeclare **/
	class CallbackService;

	/** CallbackRpcGroup **/
	class CallbackRpcGroup: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		enum{
			STATE_ERROR =-1,
			STATE_PROCESSING =0,
			STATE_COMPLETED =1,
		};
		typedef int64_t (*PFN_CALLBACK)(CallbackRpcGroup* self);
	protected:
		CallbackRpcGroup(Object* context, PFN_CALLBACK callback);
		virtual ~CallbackRpcGroup();
	public:
		virtual void init();
		virtual void finalize();
	public:
		int64_t set(const int64_t idx, Object* param);
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
		bool rpc(PACKET& packet, Object* req_param, CallbackItemRpcInfo* rpc_info);
		bool rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, CallbackItemRpcInfo* rpc_info);
	public:
		int64_t getCount();
		int64_t getState(const int64_t idx);
		Object* getValue(const int64_t idx);
		int64_t getSuccessCount();
		int64_t getErrorCount();
	private:
		Object* m_context;
		PFN_CALLBACK m_callback;

		Int64Array* m_state_array;
		Array* m_value_array;

		int64_t m_success_count;
		int64_t m_error_count;

		CallbackService* m_service; // weak ptr
	};
}

#endif
