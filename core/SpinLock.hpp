/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_SPIN_LOCK_H__
#define H_CORE_SPIN_LOCK_H__

namespace core{
	/** SpinLock **/
	class SpinLock{
	public:
		SpinLock(){
			m_flag.clear();
		}
		~SpinLock(){
		}
	public:
		void lock(){
			while(m_flag.test_and_set(std::memory_order_acquire));
		}
		bool trylock(){
			return false == (m_flag.test_and_set(std::memory_order_acquire));
		}
		void unlock(){
			m_flag.clear(std::memory_order_release);
		}
	private:
		std::atomic_flag m_flag;
	};
}

#endif
