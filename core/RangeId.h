/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_RANGE_ID_H__
#define H_CORE_RANGE_ID_H__

namespace core{
	/** RangeId **/
	class RangeId: public Object{
		typedef Object Super;
		SUPPORT_NEWABLE
	public:
		enum{
			ALG_DEFAULT =0,
			// consistent algrithm
			ALG_USE_FROM_WHO =0,
			ALG_USE_WHO =1,
			ALG_USE_FROM =2,
			// non-consistent algrithm
			ALG_RANDOM =3,
			ALG_ROUND_ROBIN =4,
		};
	private:
		RangeId();
		virtual ~RangeId();
	public:
		int64_t getAlgrithm();
		int64_t getBeginId();
		int64_t getEndId();
		int64_t getId(const int64_t from, const int64_t who);
		bool isConsistent();
	public:
		void setRange(const int64_t beg, const int64_t end);
		void setAlgrithm(const int64_t type);
	private:
		int64_t m_begin_id;
		int64_t m_end_id;
		int64_t m_algrithm_type;
		int64_t m_round_robin_id;
	};
}

#endif
