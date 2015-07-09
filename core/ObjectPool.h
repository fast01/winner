/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_OBJECT_POOL__
#define H_CORE_OBJECT_POOL__

namespace core{
	/** ObjectPool **/
	class ObjectPool : public Object{
		SUPPORT_NEWABLE
	private:
		ObjectPool();
		virtual ~ObjectPool();
	public:
		virtual void init();
		virtual void finalize();
	public:
		void push(Object* obj);
		int64_t getCount();
		void restore(const int64_t count);
	public:
		static void Set(ObjectPool* pool);
		static ObjectPool* Instance();
		static void Release();
	private:
		Array* m_pObjStack;
	};

	//** ObjectPoolHelper **//
	class ObjectPoolHelper{
	public:
		ObjectPoolHelper();
		~ObjectPoolHelper();
	private:
		ObjectPool* m_object_pool;
		int64_t m_iCount;
	};
}
#define OPH() core::ObjectPoolHelper UNIQUE_LOCAL_NAME(ObjectPoolHelper);

#endif
