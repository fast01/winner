/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_LIST__
#define H_CORE_LIST__

namespace core{
	/** List **/
	class List : public Container{
		SUPPORT_NEWABLE
		typedef Container Super;
	public:
		typedef struct tagLIST_NODE{
			struct tagLIST_NODE* prev;
			struct tagLIST_NODE* next;
			Object* data;
		}LIST_NODE, *PLIST_NODE;
	protected:
		List();
		virtual ~List();
	public:
		virtual Object* clone();
		virtual void finalize();
	public:
		virtual int64_t size();
		virtual bool empty();
		virtual Iterator* iterator();
	public:
		PLIST_NODE front();
		PLIST_NODE back();
	public:
		PLIST_NODE push_front(Object* o);
		void pop_front();
		PLIST_NODE push_back(Object* o);
		void pop_back();
	public:
		void remove(PLIST_NODE node);
		PLIST_NODE insertBefore(PLIST_NODE node, Object* o);
		PLIST_NODE insertAfter(PLIST_NODE node, Object* o);
		void clear();
		void join(List* arr);
		void removeEmpty();
		void removeIf(bool (*pfn)(Object*));
		void removeEquals(Object* o);
		void forEach(void (*pfn)(Object*, void* userdata), void *userdata);
	private:
		PLIST_NODE m_head;
		PLIST_NODE m_tail;
		int64_t m_size;
	};
}
#define LIST_NODE_2_OBJECT(node) ((node)->data)
#endif
