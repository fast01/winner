/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_S2_CALLBACK_SERVICE_H__
#define H_S2_CALLBACK_SERVICE_H__

namespace service{
	/** S2CallbackService **/
	class S2CallbackService: public core::CallbackService{
		typedef core::CallbackService Super;
		SUPPORT_NEWABLE
		DECLARE_CLASS_INFO
	public:
		enum{
			SERVICE_ID = SERVICE_ID_S2_CALLBACK,
		};
	protected:
		S2CallbackService();
		virtual ~S2CallbackService();
	public:
		static Service* New();
	protected:
		virtual bool on_load();
		virtual void on_unload();
		virtual void on_update(const int64_t now);
		virtual void register_command();
	private:
		DECLARE_COMMAND(on_first_request)
		static int64_t on_second_request(Command* command);
	private:
		static int64_t _on_first_rpc_respond(Object* param, Object* context);
		static int64_t _on_second_rpc_respond(Object* param, Object* context);
	private:
		int64_t m_version =1;
	};
}

#endif
