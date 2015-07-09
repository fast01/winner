/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_STRING_TABLE__
#define H_STRING_TABLE__
namespace core{
	/** StringTable **/
	class StringTable : public Object{
		enum{
			MAX_STRING_LENGTH =64	
		};
		SUPPORT_NEWABLE
		DECLARE_PROCESS_LOCAL_SINGLETON(StringTable)
	private:
		StringTable();
		virtual ~StringTable();
	public:
		virtual void init();
		virtual void finalize();
	public:
		String* get(String* str);
		String* get(const char* str);
		void clear();
	private:
		String* _get(String* str);
		String* _get(const char* str);
		void _clear();
	public:
		static String* Get(String* str);
		static String* Get(const char* str);
	private:
		Hash* m_pStrTable;
		std::mutex m_mutex;
	};
}

#endif
