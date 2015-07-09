/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_TEMPLATE__
#define H_TEMPLATE__

namespace core{
	/** remove all decorator **/
	template<typename T>
	struct remove_all_decorator{
		typedef typename std::remove_cv< typename std::remove_pointer< typename std::remove_reference< T >::type >::type >::type type;
	};

	/** new **/
	template<typename T> 
	inline T* SafeNew(){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T();
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1> 
	inline T* SafeNew(A1 a1){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2> 
	inline T* SafeNew(A1 a1, A2 a2){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7, a8);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9> 
	inline T* SafeNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7, a8, a9);
		o->init();
		ObjectPool::Instance()->push(o);
		return o;
	}

	/** simple new **/
	template<typename T> 
	inline T* New(){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T();
		o->init();
		return o;
	}
	template<typename T, typename A1> 
	inline T* New(A1 a1){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2> 
	inline T* New(A1 a1, A2 a2){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3> 
	inline T* New(A1 a1, A2 a2, A3 a3){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7, a8);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9> 
	inline T* New(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9){
		T* o =reinterpret_cast< T* >(ALLOCATE(sizeof(T)));
		o =new(o) T(a1, a2, a3, a4, a5, a6, a7, a8, a9);
		o->init();
		return o;
	}

	/** raw new and init **/
	template<typename T> 
	inline T* CppNew(){
		T* o =new T();
		o->init();
		return o;
	}
	template<typename T, typename A1> 
	inline T* CppNew(A1 a1){
		T* o =new T(a1);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2> 
	inline T* CppNew(A1 a1, A2 a2){
		T* o =new T(a1, a2);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3){
		T* o =new T(a1, a2, a3);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4){
		T* o =new T(a1, a2, a3, a4);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5){
		T* o =new T(a1, a2, a3, a4, a5);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6){
		T* o =new T(a1, a2, a3, a4, a5, a6);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7){
		T* o =new T(a1, a2, a3, a4, a5, a6, a7);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8){
		T* o =new T(a1, a2, a3, a4, a5, a6, a7, a8);
		o->init();
		return o;
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9> 
	inline T* CppNew(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9){
		T* o =new T(a1, a2, a3, a4, a5, a6, a7, a8, a9);
		o->init();
		return o;
	}
	inline void CppDelete(Object* o){
		if(o){
			o->finalize();
			delete o;
		}
	}

	/** compare **/
	template<typename T>
	int64_t CompareFunc(Object* a, Object* b){
		return Compareable::Compare(dynamic_cast< Compareable* >(dynamic_cast< T* >(a)), dynamic_cast< Compareable* >(dynamic_cast< T* >(b)));
	}

	/** MakeFieldInfo **/
	template< typename CLASS_T, typename FIELD_T >
	inline FieldInfoBase* MakeFieldInfo(FIELD_T CLASS_T::* p, String* name, String* attribute){
		return SafeNew< FieldInfo<CLASS_T, FIELD_T> >(p, name, attribute);
	}

	/** PerformanceTest **/
	template< int64_t N, typename T >
	void PerformanceTest(const char* name, T fn){
		const int64_t begin_clocks =clock();
		for(int64_t i=0; i<N; ++i){
			fn();
		}
		const int64_t end_clocks =clock();
		const float64_t elapse =static_cast<float64_t>(end_clocks - begin_clocks) / static_cast<float64_t>(CLOCKS_PER_SEC);
		if(elapse > 0){
			const float64_t tps =static_cast<float64_t>(N) / elapse;
			DEBUG("<%s> Performance Test, Times =%lld, Elapse =%f, TPS =%G", name, (long long)N, elapse, tps);
		}
		else{
			DEBUG("<%s> Performance Test, Times =%lld, Elapse =%f, TPS =NaN", name, (long long)N, elapse);
		}
	}

}
#endif
