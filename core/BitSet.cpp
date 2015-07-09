/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#include "core.h"

namespace core{
	/*** script support ***/
	DEFINE_SUPPORT_SCRIPT(BitSet, "core::BitSet")

	/*** impl BitSet ***/
	/** ctor & ctor **/
	BitSet::BitSet()
		: m_bits(0)
		, m_bits_holder_count(0){
	}
	BitSet::~BitSet(){
	}

	/** New **/
	BitSet* BitSet::New(){
		BitSet* obj =SafeNew<BitSet>();
		return obj;
	}

	/** Object **/
	void BitSet::init(){
		Super::init();
	}
	void BitSet::finalize(){
		clear();
		Super::finalize();
	}

	/** basic **/
	uint64_t* BitSet::data(){
		return m_bits;
	}
	uint64_t BitSet::getBitsHolderCount(){
		return m_bits_holder_count;
	}
	void BitSet::set(const uint64_t* bits, const uint64_t bits_holder_count){
		if(bits && bits_holder_count>0){
			_ensure_holder_count(bits_holder_count);
			memset(m_bits, 0, m_bits_holder_count*sizeof(uint64_t));
			memcpy(m_bits, bits, bits_holder_count*sizeof(uint64_t));
		}
	}
	void BitSet::setUint64(const uint64_t val){
		_ensure_holder_count(1);
		memset(m_bits, 0, sizeof(uint64_t)*m_bits_holder_count);
		m_bits[0] =val;
	}
	uint64_t BitSet::asUint64(){
		if(m_bits_holder_count >= 1){
			return m_bits[0];
		}
		else{
			return 0;
		}
	}
	void BitSet::clear(){
		DEALLOCATE(m_bits);
		m_bits =0;
		m_bits_holder_count =0;
	}
	uint64_t BitSet::count(){
		uint64_t cnt =0;
		const uint64_t bits_count =m_bits_holder_count * 64;
		for(uint64_t i=0; i<bits_count; ++i){
			if(test(i)){
				++cnt;
			}
		}
		return cnt;
	}

	/** with string **/
	void BitSet::fromString(String* src){
		if(m_bits_holder_count){
			memset(m_bits, 0, m_bits_holder_count*sizeof(uint64_t));
		}
		const int64_t len =src->size();
		for(int64_t i=0; i<len; ++i){
			if(src->at(i) == '1'){
				set(static_cast<uint64_t>(i));
			}
		}
	}
	String* BitSet::toString(){
		Bytes* bs =SafeNew<Bytes>();
		uint64_t set_cnt =0;
		const uint64_t max_set_cnt =count();
		const uint64_t bits_count =m_bits_holder_count * 64;
		bs->reserve(bits_count);
		for(uint64_t i=0; i<bits_count; ++i){
			if(test(i)){
				bs->write("1", 1);
				++set_cnt;
				if(set_cnt == max_set_cnt){
					break;
				}
			}
			else{
				bs->write("0", 1);
			}
		}
		return String::New(bs->c_str(), bs->size());
	}

	/** bits op **/
	bool BitSet::test(const uint64_t idx){
		const register uint64_t pos =idx / 64;
		if(pos < m_bits_holder_count){
			const register uint64_t offset =idx % 64;
			return m_bits[pos] & (static_cast<uint64_t>(1) << offset);
		}
		else{
			return false;
		}
	}
	void BitSet::set(const uint64_t idx){
		const register uint64_t pos =idx / 64;
		const register uint64_t offset =idx % 64;
		_ensure_holder_count(pos + 1);
		m_bits[pos] |= (static_cast<uint64_t>(1) << offset);
	}
	void BitSet::flip(const uint64_t idx){
		if(test(idx)){
			clear(idx);
		}
		else{
			set(idx);
		}
	}
	void BitSet::clear(const uint64_t idx){
		const register uint64_t pos =idx / 64;
		if(pos < m_bits_holder_count){
			const register uint64_t offset =idx % 64;
			m_bits[pos] &= (~(static_cast<uint64_t>(1) << offset));
		}
	}
	void BitSet::clearAll(){
		if(m_bits_holder_count){
			memset(m_bits, 0, m_bits_holder_count * sizeof(uint64_t));
		}
	}
	uint64_t BitSet::fetch(const uint64_t idx, const uint64_t cnt){
		const register uint64_t pos =idx / 64;
		if(pos < m_bits_holder_count){
			const register uint64_t offset =idx % 64;
			const register uint64_t remain_cnt =64 - offset;
			if(remain_cnt >= cnt){
				return BitOp::Fetch(m_bits[pos], offset, cnt);
			}
			else{
				const register uint64_t head =BitOp::Fetch(m_bits[pos], offset, remain_cnt);
				if(pos+1 < m_bits_holder_count){
					const register uint64_t tail =BitOp::Fetch(m_bits[pos+1], 0, cnt - remain_cnt);
					return head | (tail << remain_cnt);
				}
				else{
					return head;
				}
			}
		}
		else{
			return 0;
		}
	}
	void BitSet::replace(const uint64_t idx, const uint64_t cnt, const uint64_t val){
		const register uint64_t pos =idx / 64;
		const register uint64_t offset =idx % 64;
		const register uint64_t remain_cnt =64 - offset;

		if(remain_cnt >= cnt){
			_ensure_holder_count(pos+1);
			m_bits[pos] =BitOp::Replace(m_bits[pos], offset, cnt, val);
		}
		else{
			_ensure_holder_count(pos+2);
			m_bits[pos] =BitOp::Replace(m_bits[pos], offset, remain_cnt, val);
			m_bits[pos+1] =BitOp::Replace(m_bits[pos+1], 0, cnt-remain_cnt, val >> remain_cnt);
		}
	}

	/** helper **/
	void BitSet::_ensure_holder_count(const uint64_t count){
		if(count > m_bits_holder_count){
			const register uint64_t old_bits_holder_count =m_bits_holder_count;
			m_bits_holder_count =count;
			m_bits =reinterpret_cast< uint64_t* >(REALLOCATE(m_bits, m_bits_holder_count * sizeof(uint64_t)));
			memset(m_bits+old_bits_holder_count, 0, (m_bits_holder_count - old_bits_holder_count) * sizeof(uint64_t));
		}
	}
}
