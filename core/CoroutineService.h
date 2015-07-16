/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COROUTINE_SERVICE_H__
#define H_CORE_COROUTINE_SERVICE_H__

namespace core{
	/** CoroutineService **/
	class CoroutineService : public CommandService{
		typedef CommandService Super;
		SUPPORT_NEWABLE
		friend class Coroutine;
		friend class CoroutineRpcInfo;
	protected:
		enum{
			AUTO_FLUSH_THRESHOLD =1024 * 1024
		};
	protected:
		CoroutineService();
		virtual ~CoroutineService();
	public:
		static Service* New();
	protected:
		virtual bool on_load();
		virtual void on_unload();
		virtual int64_t on_start_command(Command* command);
		virtual void on_update(const int64_t now);
	protected:
		virtual void register_command();
		virtual void update(const int64_t now);
		void on(const int64_t cmd, CoroutineCommandDesc* desc);
		void on(const int64_t cmd, const bool is_use_protocol, Coroutine::PFN_COROUTINE_TASK pfn);
		void on(const int64_t cmd, Coroutine::PFN_COROUTINE_TASK pfn);

		Object* rpc(const int64_t who, const int64_t to, const int64_t cmd, const int64_t res_proto_grp_id, Object* req_param);
		bool sleep(const int64_t secs);
		int64_t resume_coroutine(const int64_t cr_id, Object* param, const int64_t sign);
	public:
		CoroutinePool* getCoroutinePool();
	private:
		int64_t _resume_coroutine(const int64_t cr_id, Object* param, const int64_t sign);
		static void _update(Object* arg);
	protected:
		Hash* m_command_desc_table;
		Hash* m_sleeper_table;
		CoroutinePool* m_cr_pool;
	};
}
#endif
