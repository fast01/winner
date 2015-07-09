/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CALLBACK_COMMAND_DESC_H__
#define H_CORE_CALLBACK_COMMAND_DESC_H__

namespace core{
	/** CallbackCommandDesc **/
	class CallbackCommandDesc: public CommandDesc{
		SUPPORT_NEWABLE
		typedef CommandDesc Super;
	public:
		typedef int64_t (*PFN_CALLBACK)(Command* command);
	public:
		CallbackCommandDesc();
		CallbackCommandDesc(const int64_t res_cmd, PFN_CALLBACK pfn);
		CallbackCommandDesc(const int64_t res_cmd, const bool use_protocol, PFN_CALLBACK pfn);
		virtual ~CallbackCommandDesc();
	public:
		virtual void finalize();
	public:
		void setCallback(PFN_CALLBACK pfn);
		PFN_CALLBACK getCallback();
	protected:
		PFN_CALLBACK m_callback;
	};
}

#endif
