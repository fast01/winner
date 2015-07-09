/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LUA_COMMAND_H__
#define H_CORE_LUA_COMMAND_H__

namespace core{
	/** predeclare  **/
	class Service;

	/** Command **/
	class Command : public Object {
		typedef Object Super;
		SUPPORT_NEWABLE
	public:
		enum{
			TIMEOUT_SECS =30 // 30 secs
		};
		enum{
			STATE_INIT =-2,
			STATE_ERROR =-1,
			STATE_COMPLETE =0,
			STATE_PROCESSING =1,
		};
	public:
		typedef int64_t (*PFN_CALLBACK)(Command* command, Command* rpc_res);
	protected:
		Command(const int64_t id, const PACKET& packet, Bytes* body, Requestor* requestor, Service* service);
		virtual ~Command();
	public:
		virtual void init();
		virtual void finalize();
	public:
		int64_t getId();
		Requestor* getRequestor();
		Bytes* getBody();
		Service* getService();
		int64_t getTimestamp();
		bool isTimeout(const int64_t now);
	public:
		PACKET getPacket();
		int64_t getFrom();
		int64_t getTo();
		int64_t getWho();
		int64_t getCommand();
		int64_t getOption();
		int64_t getSn();
	public:
		void setState(const int64_t state);
		int64_t getState();
		bool isInit();
		bool isProcessing();
		bool isCompleted();
	public:
		void setRequest(Object* request);
		Object* getRequest();

		int64_t getRequestCommand();
	public:
		void setRespond(Object* respond);
		Object* getRespond();

		void setRespondCommand(const int64_t cmd);
		int64_t getRespondCommand();
	public:
		bool reply();
		bool reply(const int64_t res_cmd);
		bool reply(const int64_t res_cmd, void* body, const int64_t body_len);
	protected:
		int64_t m_id; // unique id in each service
		Requestor* m_requestor;
		PACKET m_packet;
		Bytes* m_body;
		int64_t m_state;
		int64_t m_timestamp;

		Object* m_request;
		Object* m_respond;

		int64_t m_respond_command;

		Service* m_service; // weak pointer
	};
}

#endif

