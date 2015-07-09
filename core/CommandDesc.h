/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_COMMAND_DESC_H__
#define H_CORE_COMMAND_DESC_H__

namespace core{
	/** CommandDesc **/
	class CommandDesc: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		CommandDesc();
		CommandDesc(const int64_t res_cmd, const bool use_protocol);
		virtual ~CommandDesc();
	public:
		virtual void finalize();
	public:
		void setRespondCommand(const int64_t cmd);
		int64_t getRespondCommand();
	public:
		bool isUseProtocol();
		void setUseProtocol(const bool use_protocol);
	protected:
		int64_t m_respond_command;
		bool m_is_use_protocol;
	};
}

#endif
