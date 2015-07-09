/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_EVENT_H__
#define H_CORE_EVENT_H__

namespace core{
	/** Event **/
	class Event: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	protected:
		Event();
		~Event();
	public:
		void setId(const int64_t id);
		int64_t getId();

		void setTimestamp(const int64_t timestamp);
		int64_t getTimestamp();
	private:
		int64_t m_id;
		int64_t m_timestamp;
	};
}

#endif
