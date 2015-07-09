/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_STRING__
#define H_CORE_STRING__
namespace core{
	/** predecl **/
	class Array;
	class Object;
	class Compareable;

	/** String **/
	class String : public Compareable{
		typedef Compareable Super;
		SUPPORT_NEWABLE
	private:
		String();
		virtual ~String();
	public:
		virtual String* toString();
		virtual int64_t getHashCode();
		virtual bool equals(Object* str);
	public:
		virtual int64_t compare(Compareable* obj);
	public:
		static String* NewString();
		static String* NewString(const char* str);
		static String* NewString(const char* str, const int64_t len);
		static String* New();
		static String* New(const char* str);
		static String* New(const char* str, const int64_t len);
	public:
		static int64_t Length(String* str);
	public:
		static String* FormatV(const char* fmt, va_list vl);
		static String* FormatV(String* fmt, va_list vl);
		static String* Format(const char* fmt, ...);
		static String* Format(String* fmt, ...);
		static String* Repeat(const char ch, const int64_t cnt);
	public:
		const char* c_str();
		int64_t length();
		int64_t size();
		bool empty();
		char get(const int64_t idx);
		char at(const int64_t idx);
		bool hasPrefix(const char* prefix);
		bool hasPrefix(String* prefix);
		bool hasSubfix(const char* subfix);
		bool hasSubfix(String* subfix);
		int64_t indexOf(const char* str);
		int64_t indexOf(String* subfix);
		int64_t lastIndexOf(const char* str);
		int64_t lastIndexOf(String* subfix);
		bool is(const char* str);
		bool is(String* str);
	public:
		String* append(const char* str);
		String* append(String* str);
		String* lower();
		String* upper();
		String* subString(const int64_t start, const int64_t count);
		String* replace(const char* from, const char* to, const bool bGlobal);
		String* replace(String* from, String* to, const bool bGlobal);
		String* ltrim();
		String* rtrim();
		String* trim();
	public:
		Array* split(const char* sep);
		Array* split(String* sep);
	public:
		static int64_t CalcHashCode(const char* str, int64_t len);
		static String* EncodeQuotableString(String* str);
	private:
		char* m_data;
		int64_t m_length;
	};
}
#endif
