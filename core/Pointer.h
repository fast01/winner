/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_POINTER__
#define H_CORE_POINTER__
namespace core{
	/** Pointer **/
	class Pointer : public Object{
		typedef Object Super;
	public:
		Pointer();
		Pointer(void* v);
		virtual ~Pointer();
	public:
		virtual Object* clone();
		virtual bool equals(const Object* obj);
		virtual int64_t getHashCode();
		virtual String* toString();
	public:
		void* getValue();
		void setValue(void* v);
	private:
		void* m_value;
	};
}
#endif
