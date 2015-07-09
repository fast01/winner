/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_BITOP_HPP__
#define H_CORE_BITOP_HPP__

/*
 * Function Lshift, Rshift: n < 64, otherwise the result is undefied
 * Function Fetch: pos + count <= 64 and count > 0, otherwise the result is undefied
 * Function Replace: pos + count <= 64 and count > 0, otherwise the result is undefied. only the lower count bits of replace_val will affect the result.
 *
*/

namespace core{
	/** BitOp **/
	class BitOp{
	public:
		static uint64_t And(const uint64_t a, const uint64_t b){
			return a & b;
		}
		static uint64_t Or(const uint64_t a, const uint64_t b){
			return a | b;
		}
		static uint64_t Xor(const uint64_t a, const uint64_t b){
			return a ^ b;
		}
		static uint64_t Negate(const uint64_t a){
			return ~a;
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static uint64_t Lshift(const uint64_t val, const uint64_t n){
			return val << n;
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static uint64_t Rshift(const uint64_t val, const uint64_t n){
			return val >> n;
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static bool Test(const uint64_t val, const uint64_t n){
			return (val & (static_cast<uint64_t>(1) << n)) != 0;
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static uint64_t Set(const uint64_t val, const uint64_t n){
			return val | (static_cast<uint64_t>(1) << n);
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static uint64_t Clear(const uint64_t val, const uint64_t n){
			return val & (~(static_cast<uint64_t>(1) << n));
		}
		/*
		   n must in [0, 63]
		   if n invalid then result is undefiend
		*/
		static uint64_t Flip(const uint64_t val, const uint64_t n){
			if(Test(val, n)){
				return Clear(val, n);
			}
			else{
				return Set(val, n);
			}
		}
		/*
		   count must in [0, 64]
		   if count invalid then result is undefiend
		*/
		static uint64_t ClearHigh(const uint64_t val, const uint64_t count){
			if(count >= 64){
				return 0;
			}
			else{
				return (val << count) >> count;
			}
		}
		/*
		   count must in [0, 64]
		   if count invalid then result is undefiend
		*/
		static uint64_t ClearLow(const uint64_t val, const uint64_t count){
			if(count >= 64){
				return 0;
			}
			else{
				return (val >> count) << count;
			}
		}
		/*
		   pos must in [0, 63], count must in [1, 64], and pos + count must <= 64
		   if pos invalid or count invalid then result is undefiend
		   if pos + count >= 64 invalid then result is undefiend
		*/
		static uint64_t Fetch(const uint64_t val, const uint64_t pos, const uint64_t count){
			return ClearHigh(val>>pos, 64-count);
		}
		/*
		   pos must in [0, 63], count must in [1, 64], and pos + count must <= 64
		   if pos invalid or count invalid then result is undefiend
		   if pos + count >= 64 invalid then result is undefiend
		*/
		static uint64_t Replace(const uint64_t val, const uint64_t pos, const uint64_t count, const uint64_t replace_val){
			// calc low bits
			const register uint64_t l =ClearHigh(val, 64-pos);
			// calc high bits
			const register uint64_t h =ClearLow(val, pos+count);
			// calc middle bits
			const register uint64_t m =ClearHigh(replace_val, 64-count) << pos;
			return h | m | l;
		}
	};
}

#endif

