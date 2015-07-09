/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_NUMBER__
#define H_CORE_NUMBER__

/** declare **/
#define DECLARE_NUMBER_CLASS(cls_name, type) \
	class cls_name : public Compareable{	\
	public:	\
		cls_name();	\
		cls_name(type v);	\
		virtual ~cls_name();	\
	public:	\
	virtual Object* clone();	\
		virtual bool equals(Object* obj);	\
		virtual int64_t getHashCode();	\
		virtual String* toString();	\
	public:	\
		virtual int64_t compare(Compareable* obj);	\
	public:	\
		void setValue(type v);	\
		type getValue();	\
	public:	\
		static int64_t CalcHashCode(const type n);	\
	private:	\
		type m_value;	\
	};
	
/** define **/
#define DEFINE_NUMBER_CLASS(cls_name, type)	\
	cls_name::cls_name():m_value(){}	\
	cls_name::cls_name(type v):m_value(v){}	\
	cls_name::~cls_name(){}	\
	bool cls_name::equals(Object* obj){ if(const cls_name* o =dynamic_cast<cls_name*>(obj)){ return m_value==o->m_value; } else { return false; } }	\
	int64_t cls_name::getHashCode(){ return CalcHashCode(m_value); }	\
	String* cls_name::toString(){	\
		return ToString<type>(m_value);	\
	}	\
	Object* cls_name::clone(){ return SafeNew<cls_name, type>(m_value); }	\
	int64_t cls_name::compare(Compareable* obj){ if(const cls_name* o =dynamic_cast<cls_name*>(obj)){ if(m_value > o->m_value){ return 1; } else if(m_value == o->m_value){ return 0; } else { return -1; } } else { return 1; } }	\
	void cls_name::setValue(type v){m_value =v;}	\
	type cls_name::getValue(){ return m_value;}	\
	int64_t cls_name::CalcHashCode(const type n){ const int64_t c = static_cast<int64_t>(n / 2); return ((c<0) ? (-c) : c); }	\

namespace core{
	//** number **//
	DECLARE_NUMBER_CLASS(Float32, float32_t)
	DECLARE_NUMBER_CLASS(Float64, float64_t)
	DECLARE_NUMBER_CLASS(Int8, int8_t)
	DECLARE_NUMBER_CLASS(Int16, int16_t)
	DECLARE_NUMBER_CLASS(Int32, int32_t)
	DECLARE_NUMBER_CLASS(Int64, int64_t)
	DECLARE_NUMBER_CLASS(Uint8, uint8_t)
	DECLARE_NUMBER_CLASS(Uint16, uint16_t)
	DECLARE_NUMBER_CLASS(Uint32, uint32_t)
	DECLARE_NUMBER_CLASS(Uint64, uint64_t)
}
#endif
