/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CALLBACK_SERVICE_H__
#define H_CORE_CALLBACK_SERVICE_H__

namespace core{
	/** CallbackService **/
	class CallbackService : public CommandService{
		typedef CommandService Super;
		SUPPORT_NEWABLE
	protected:
		CallbackService();
		virtual ~CallbackService();
	public:
		static Service* New();
	protected:
		virtual bool on_load();
		virtual void on_unload();
		virtual int64_t on_start_command(Command* command);
		virtual void on_update(const int64_t now);
	public:
		bool rpc(PACKET& packet, Object* req_param, RpcInfo* info);
		bool rpc(const int64_t who, const int64_t to, const int64_t cmd, Object* req_param, RpcInfo* info);
	public:
		int64_t setRpcGroup(CallbackGroupRpcInfo* info);
		void removeRpcGroup(const int64_t id);
	protected:
		virtual void register_command();
		void on(const int64_t cmd, CallbackCommandDesc* desc);
	protected:
		Hash* m_command_desc_table;
	};
}
#endif
