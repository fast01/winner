/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef HPP_CORE_COMPAREABLE_H__
#define HPP_CORE_COMPAREABLE_H__
namespace core{
	/** Compareable **/
	class Compareable : public Object{
	public:
		virtual int64_t compare(Compareable* obj) =0;
		static int64_t Compare(Compareable* obj1, Compareable* obj2);
	};
}
#endif
