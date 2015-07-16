/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COMMAND_SERVICE_H__
#define H_CORE_COMMAND_SERVICE_H__

namespace core{
	/** CommandService **/
	class CommandService: public Service{
		typedef Service Super;
	public:
		enum{
			LOGIN_REQUEST  =1,
			LOGIN_RESPOND =2,
			LOGOUT_REQUEST  =3,
			LOGOUT_RESPOND =4,
			USER_COMMAND_MIN =16,
		};
	private:
		enum{
			OPTIMIZE_THRESHOLD =8192,
			OPTIMIZE_TIMER =120, // 120 SECS
			PROCESS_TIMEOUT_TIMER =300, // 300 secs
		};
	protected:
		CommandService();
		virtual ~CommandService();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual void on_update(const int64_t now);
		virtual void on_message(Requestor* requestor, const PACKET& packet, void* body, const int64_t body_len);
	public:
		Command* getProcessingCommand();
		void setProtocolGroupId(const int64_t proto_grp_id);
		int64_t getProtocolGroupId();
	protected:
		virtual int64_t on_start_command(Command* command);
	protected:
		int64_t set_rpc(RpcInfo* rpc);
		void del_rpc(const int64_t rpc_id);
		RpcInfo* _get_rpc(const int64_t rpc_id);
		int64_t _gen_rpc_id();
	public:
		static int64_t GenRpcId();
	private:
		void _append_request(Command* command);
		void _process_request(const int64_t who);
		void _process_respond(Command* command);
		void _process_timeout(const int64_t now);
		void _optimize(const int64_t now);
	protected:
		// command queue
		Hash* m_queue_tb;
		int64_t m_command_id;
		Command* m_processing_command;

		// rpc table
		Hash* m_rpc_tb;
		int64_t m_rpc_id;

		// protocol
		int64_t m_protocol_group_id;

		// time
		int64_t m_process_timeout_last_time;
		int64_t m_optimize_last_time;
	};
}

#endif
