/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BIT_SET_H__
#define H_CORE_BIT_SET_H__

namespace core{
	class BitSet: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
		DECLARE_SUPPORT_SCRIPT
	protected:
		BitSet();
		virtual ~BitSet();
	public:
		static BitSet* New();
	public:
		virtual void init();
		virtual void finalize();
	public:
		uint64_t* data();
		uint64_t getBitsHolderCount();
		void set(const uint64_t* src, const uint64_t bits_holder_count);
		void setUint64(const uint64_t val);
		uint64_t asUint64();
		void clear();
		uint64_t count();
	public:
		void fromString(String* src);
		virtual String* toString();
	public:
		bool test(const uint64_t idx);
		void set(const uint64_t idx);
		void flip(const uint64_t idx);
		void clear(const uint64_t idx);
		void clearAll();
		uint64_t fetch(const uint64_t idx, const uint64_t cnt);
		void replace(const uint64_t idx, const uint64_t cnt, const uint64_t val);
	private:
		void _ensure_holder_count(const uint64_t count);
	private:
		uint64_t* m_bits;
		uint64_t m_bits_holder_count;
	};
}

#endif
