/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HASH_H__
#define H_CORE_HASH_H__
namespace core{
	/** Hash **/
	class Hash : public Container{
		SUPPORT_NEWABLE
			friend class HashIterator;
		typedef Container Super;
	public:
		typedef bool (*PFN_OBJECT_LESS)(Object*, Object*);
	protected:
		Hash();
		virtual ~Hash();
	public:
		virtual void init();
		virtual Object* clone();
		virtual void finalize();
	public:
		int64_t size();
		bool empty();
		virtual Iterator* iterator();
	public:
		Object* get(const int64_t key);
		Object* get(const char* key);
		Object* get(Object* key);
	public:
		void set(const int64_t key, Object* val);
		void set(const char* key, Object* val);
		void set(Object* key, Object* val);
	public:
		void remove(const int64_t key);
		void remove(const char* key);
		void remove(Object* key);
		void removeIf(bool (*pfn)(Object*, Object*));
		void clear();
	public:
		bool has(const int64_t key);
		bool has(const char* key);
		bool has(Object* key);
	public:
		void forEach(void (*pfn)(Object*, Object*, void*), void* userdata);
		void optimize(int64_t gap);
	private:
		void _try_rehash();
		int64_t _code_to_slot(const int64_t code);
		int64_t _key_to_slot(Object* obj);
	private:
		typedef struct tagNODE{
			Object* key;
			Object* value;
			struct tagNODE* prev;
			struct tagNODE* next;
		}NODE, *PNODE;
		PNODE* m_pSlotArray;
		int64_t m_slot_count;
		int64_t m_item_count;
	private:
		void _release_node(const int64_t slot, PNODE node);
	};

	//** HashIterator **//
	class HashIterator : public Iterator{
		typedef Iterator Super;
	public:
		HashIterator();
		HashIterator(Hash* hash);
		virtual ~HashIterator();
	public:
		virtual void init();
		virtual Object* clone();
		virtual void finalize();
	public:
		virtual bool next();
		virtual Object* getValue();
		virtual Container* getContainer();
	public:
		void bind(Hash* hash);
		void reset();
		void remove();
		Hash* getHash();
		Object* getKey();
	private:
		Hash* m_hash;
		int64_t m_index;
		Hash::PNODE m_cursor;
	};
}
#endif
