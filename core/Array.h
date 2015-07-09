/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_ARRAY__
#define H_CORE_ARRAY__

namespace core{
	/** Array **/
	class Array : public Container{
		friend class ArrayIterator;
		SUPPORT_NEWABLE
		typedef Container Super;
	public:
		typedef bool (*PFN_OBJECT_LESS)(Object*, Object*);
	private:
		class LessObject{
		public:
			LessObject(const PFN_OBJECT_LESS fn) : m_fn(fn){}
			bool operator ()(Object* a, Object* b)const{
				return m_fn(a, b);
			}
		private:
			const PFN_OBJECT_LESS m_fn;
		};
	protected:
		Array();
		virtual ~Array();
	public:
		virtual Object* clone();
		virtual void finalize();
	public:
		static int64_t Length(Array* arr);
	public:
		virtual int64_t size();
		virtual bool empty();
		virtual Iterator* iterator();
	public:
		Object* at(const int64_t idx);
		Object* get(const int64_t idx);
		int64_t capacity();
		Object* front();
		Object* back();
		int64_t firstIndexOf(Object* o);
		int64_t lastIndexOf(Object* o);
	public:
		void push_front(Object* o);
		void pop_front();
		void push_back(Object* o);
		void pop_back();
	public:
		void set(const int64_t pos, Object* o);
		void remove(const int64_t pos);
		void insert(const int64_t pos, Object* o);
		void clear();
		void join(Array* arr);
		int64_t sort(const PFN_OBJECT_LESS fn);
		void removeEmpty();
		void removeEquals(Object* o);
		void removeIf(bool (*pfn)(Object*));
		void forEach(void (*pfn)(const int64_t, Object*, void*), void* userdata);
	public:
		void reserve(const int64_t capacity);
		void resize(const int64_t size);
		void optimize(int64_t gap);
	private:
		Object** m_pObjArray;
		int64_t m_size;
		int64_t m_capacity;
	};

	/** ArrayIterator **/
	class ArrayIterator: public Iterator{
		typedef Iterator Super;
	public:
		ArrayIterator();
		ArrayIterator(Array* l);
	public:
		virtual void finalize();
	public:
		virtual bool next();
		virtual Object* getValue();
		virtual Container* getContainer();
	public:
		void reset();
		void reverse();
		void remove();
		Array* getArray();
		int64_t getCursor();
		void bind(Array* arr);
	private:
		Array* m_array;
		int64_t m_cursor;
	};
}
#endif
