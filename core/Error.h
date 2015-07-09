/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ERROR_H__
#define H_CORE_ERROR_H__

namespace core{
	/** Error **/
	class Error: public Object {
		typedef Object Super;
		SUPPORT_NEWABLE
	protected:
		Error();
		Error(const int64_t code);
		Error(String* desc, const int64_t code);
		virtual ~Error();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void setDesc(String* desc);
		virtual String* getDesc();

		void setCode(const int64_t code);
		virtual int64_t getCode();
	private:
		String* m_desc;
		int64_t m_code;
	};
}

#endif
