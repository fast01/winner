/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_POD_ARRAY__
#define H_CORE_POD_ARRAY__

/** declare **/
#define DECLARE_POD_ARRAY_CLASS(cls_name, type) \
class cls_name : public Object{	\
		typedef Object Super; \
		SUPPORT_NEWABLE \
	public:	\
		cls_name();	\
		virtual ~cls_name();	\
	public:	\
		virtual Object* clone();	\
		virtual bool equals(Object* obj);	\
		virtual void finalize();	\
	public: \
		type front();	\
		type back();	\
	public:	\
		void clear();	\
		int64_t size();	\
		int64_t capacity();	\
		bool empty();	\
		type at(const int64_t pos);	\
		type get(const int64_t pos);	\
		type* data();	\
	public:	\
		void set(const int64_t pos, type v);	\
	public:	\
		void push_front(type v);	\
		void pop_front();	\
		void push_back(type v);	\
		void pop_back();	\
	public:	\
		void remove(const int64_t pos);	\
		void remove(const int64_t pos, const int64_t cnt);	\
		void insert(const int64_t pos, type v);	\
		void insert(const int64_t pos, const type* v, const int64_t cnt);	\
	public:	\
		void one();	\
		void zero();	\
	public:	\
		void reserve(int64_t c);	\
		void resize(int64_t s);	\
	private:	\
		type* m_data;	\
		int64_t m_size;	\
		int64_t m_capacity;	\
	};
	
namespace core{
	//** array **//
	DECLARE_POD_ARRAY_CLASS(BooleanArray, bool)
	DECLARE_POD_ARRAY_CLASS(Float32Array, float32_t)
	DECLARE_POD_ARRAY_CLASS(Float64Array, float64_t)
	DECLARE_POD_ARRAY_CLASS(Int8Array, int8_t)
	DECLARE_POD_ARRAY_CLASS(Int16Array, int16_t)
	DECLARE_POD_ARRAY_CLASS(Int32Array, int32_t)
	DECLARE_POD_ARRAY_CLASS(Int64Array, int64_t)
	DECLARE_POD_ARRAY_CLASS(Uint8Array, uint8_t)
	DECLARE_POD_ARRAY_CLASS(Uint16Array, uint16_t)
	DECLARE_POD_ARRAY_CLASS(Uint32Array, uint32_t)
	DECLARE_POD_ARRAY_CLASS(Uint64Array, uint64_t)
	
}
#endif
