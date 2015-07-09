/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COROUTINE_COMMAND_DESC_H__
#define H_CORE_COROUTINE_COMMAND_DESC_H__

namespace core{
	/** CoroutineCommandDesc **/
	class CoroutineCommandDesc: public CommandDesc{
		SUPPORT_NEWABLE
		typedef CommandDesc Super;
	public:
		CoroutineCommandDesc();
		CoroutineCommandDesc(const int64_t res_cmd, Coroutine::PFN_COROUTINE_TASK pfn);
		CoroutineCommandDesc(const int64_t res_cmd, const bool use_protocol, Coroutine::PFN_COROUTINE_TASK pfn);
		virtual ~CoroutineCommandDesc();
	public:
		virtual void finalize();
	public:
		Coroutine::PFN_COROUTINE_TASK getCallback();
		void setCallback(Coroutine::PFN_COROUTINE_TASK pfn);
	private:
		Coroutine::PFN_COROUTINE_TASK m_callback;
	};
}

#endif
