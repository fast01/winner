/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_CONTAINER__
#define H_CORE_CONTAINER__

namespace core{
	/** predeclare Iterator **/
	class Iterator;

	/** Container **/
	class Container : public Object{
		typedef Object Super;
	public:
		virtual int64_t size() =0;
		virtual bool empty() =0;
		virtual Iterator* iterator() =0;
	};

	/** Iterator **/
	class Iterator: public Object{
	public:
		virtual bool next() =0;
		virtual Object* getValue() =0;
		virtual Container* getContainer() =0;
	};
}
#endif
