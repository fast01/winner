/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_OBJECT__
#define H_CORE_OBJECT__

namespace core{
	/** predeclare **/
	class String;
	class Bytes;
	class ClassInfo;

	/** Object **/
	class Object{
		DECLARE_SUPPORT_SCRIPT
	private:
		Object(const Object&);
		Object& operator =(const Object&);
	public:
		virtual ~Object();
	protected:
		Object();
	public:
		virtual void init();
		virtual Object* clone();
		virtual void finalize();
	public:
		const char* typeName();
		void release();
		void retain();
		int64_t getRefCount();
		void memoryOrderSync();
	public:
		virtual int64_t getHashCode();
	public:
		virtual bool equals(Object* obj);
		static bool Equals(Object* obj1, Object* obj2);
	public:
		virtual String* toString();
	public:
		virtual bool fromBytes(Bytes* bytes);
		virtual Bytes* toBytes(Bytes* bytes);
	public:
		virtual ClassInfo* getClassInfo();
		String* getClassName();
	public:
		static bool RegisterToLua(lua_State* L);
	public:
		static int GC_FOR_LUA(lua_State* L);
	private:
		std::atomic<long long> m_iRefCount;
	};

	/** null **/
	extern Object* null;
}
#endif
