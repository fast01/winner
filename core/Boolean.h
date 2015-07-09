/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_BOOLEAN__
#define H_BOOLEAN__
namespace core{
	/** boolean **/
	class Boolean : public Object{
		typedef Object Super;
	public:
		Boolean();
		Boolean(bool v);
		virtual ~Boolean();
	public:
		virtual Object* clone();
		virtual bool equals(Object* obj);
		virtual int64_t getHashCode();
		virtual String* toString();
	public:
		bool getValue();
		void setValue(const bool v);
	private:
		bool m_value;
	};
}
#endif
