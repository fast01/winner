/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_MACRO__
#define H_MACRO__

/** inherit **/
#define extends public
#define implements public

/** unused **/
#define UNUSED(var) ((void*)(&var))

/** unique name **/
#define __UNIQUE_LOCAL_NAME(name, ln) name ## ln
#define _UNIQUE_LOCAL_NAME(name, ln) __UNIQUE_LOCAL_NAME(name, ln)
#define UNIQUE_LOCAL_NAME(name) _UNIQUE_LOCAL_NAME(name, __LINE__)

/** assert **/
#define ASSERT assert
#define ENSURE(expr) while(!static_cast<bool>(expr)){ ERROR("eval false:"  #expr); break; }
#define MUST(expr, info) assert(expr)

/** memory **/
#define ALLOCATE(s) malloc(static_cast<size_t>(s))
#define DEALLOCATE(p) free(p)
#define REALLOCATE(p, s) realloc(p, static_cast<size_t>(s))

#if defined(MALLOC) || defined(FREE) || defined(REALLOC)
#pragma message("macros MALLOC FREE REALLOC have been defined already")
#else
#define MALLOC ALLOCATE
#define FREE DEALLOCATE
#define REALLOC REALLOCATE
#endif

/** log **/
#define PRINTF(...) do{ if(auto logger =::core::Logger::Instance()){ logger->printf(__VA_ARGS__); } }while(0)
#define LOG(lv, ...) do{ if(auto logger =::core::Logger::Instance()){ logger->log(lv, __VA_ARGS__); } }while(0)
#define DEBUG(...) do{ if(auto logger =::core::Logger::Instance()){ logger->debug(__VA_ARGS__); } }while(0)
#define INFO(...) do{ if(auto logger =::core::Logger::Instance()){ logger->info(__VA_ARGS__); } }while(0)
#define WARN(...) do{ if(auto logger =::core::Logger::Instance()){ logger->warn(__VA_ARGS__); } }while(0)
#define ERROR(...) do{ if(auto logger =::core::Logger::Instance()){ logger->error(__VA_ARGS__); } }while(0)
#define FATAL(...) do{ if(auto logger =::core::Logger::Instance()){ logger->fatal(__VA_ARGS__); } }while(0)

/** report **/
#define REPORT(...) ::core::LogService::Log(__VA_ARGS__)

/** pointer ctrl **/
#define RETAIN_POINTER(p) do{ if(::core::Object* obj_____ =dynamic_cast<::core::Object*>(p)){ obj_____->retain(); } }while(0)
#define RELEASE_POINTER(p) do{ if(::core::Object* obj_____ =dynamic_cast<::core::Object*>(p)){ obj_____->release(); } }while(0)
#define DELETE_POINTER RELEASE_POINTER
#define CLEAN_POINTER(p) do{ if(p){ p->release(); p =0; } }while(0)
#define ASSIGN_POINTER(dst, src) do{ decltype(src) s=src; if(dst != s){ if(s){ s->retain(); } if(dst){ dst->release(); } dst =s; } }while(0)
#define CAST_POINTER(p, t) (dynamic_cast< t * >(p))
#define IS_A(p, t) (dynamic_cast< t * >(p) != 0)
#define CAN_DYNAMIC_CAST IS_A
#define IS_NULL(p, t) (p == 0)

/** check **/
#define CHECK_LOG(expr, level, info) do{ if(!(expr)) { LOG(level, ::core::String::Format("%s, expr `%s`, file `%s`,func `%s`, lineno `%d`", info, #expr, __FILE__, __FUNCTION__, __LINE__)->c_str()); } }while(0)
#define CHECK_LOG_RETURN(expr, level, info) do{ if(!(expr)) { LOG(level, ::core::String::Format("%s, expr `%s`, file `%s`,func `%s`, lineno `%d`", info, #expr, __FILE__, __FUNCTION__, __LINE__)->c_str()); return; } }while(0)
#define CHECK_LOG_RETURN_VALUE(expr, level, info, value) do{ if(!(expr)) { LOG(level, ::core::String::Format("%s, expr `%s`, file `%s`,func `%s`, lineno `%d`", info, #expr, __FILE__, __FUNCTION__, __LINE__)->c_str()); return (value); } }while(0)
#define CHECK_RETURN(expr) do{ if(!(expr)) return; }while(0)
#define CHECK_RETURN_VALUE(expr, value) do{ if(!(expr)) return (value); }while(0)
#define CHECK_EXIT(expr, code) do{ if(!(expr)){ FATAL("CHECK_EXIT:exit with code `%d`, expr `%s`, file `%s`,func `%s`, lineno `%d`", (int)(code), #expr, __FILE__, __FUNCTION__, __LINE__); exit(code); } }while(0)
#define CHECK_ABORT(expr) do{ if(!(expr)){ FATAL("CHECK_ABORT:abort, expr `%s`, file `%s`,func `%s`, lineno `%d`", #expr, __FILE__, __FUNCTION__, __LINE__); abort(); } }while(0)
#define CHECK_OUT_OF_MEMORY(expr) do{ if(!(expr)){ FATAL("CHECK_EXIT:out of memory, file `%s`,func `%s`, lineno `%d`", __FILE__, __FUNCTION__, __LINE__); abort(); } }while(0)
#define CHECK_OOM CHECK_OUT_OF_MEMORY

/** string **/
#define STR(s) ::core::StringTable::Get(s)
#define CSTR(obj) (dynamic_cast< ::core::String* >(obj)->c_str())

/** class charactistic **/
#define DECLARE_CLASS_INFO \
	private: \
		static ClassInfo* m_class_info____; /* weak ptr */ \
	public: \
		static Object* Creator(); \
		virtual ClassInfo* getClassInfo(); \
		static ClassInfo* GetClassInfo(); \
		static void RegisterClass();

#define DEFINE_CLASS_INFO(cls) \
	ClassInfo* cls::m_class_info____ =0; \
	Object* cls::Creator(){ \
		return SafeNew<cls>(); \
	} \
	ClassInfo* cls::getClassInfo(){ \
		return cls::GetClassInfo(); \
	} \
	ClassInfo* cls::GetClassInfo(){ \
		ASSERT(m_class_info____ && "class info must registered befor you use it"); \
		return m_class_info____; \
	} \
	void cls::RegisterClass(){ \
		if(0 != m_class_info____){ return; } \
		m_class_info____ =SafeNew< ClassInfo >(String::New(#cls), &cls::Creator); \
		ObjectFactory::Instance()->registerClassInfo(m_class_info____); \
	}

#define BEGIN_CLASS_INFO(cls) \
	ClassInfo* cls::m_class_info____ =0; \
	Object* cls::Creator(){ \
		return SafeNew<cls>(); \
	} \
	ClassInfo* cls::getClassInfo(){ \
		return cls::GetClassInfo(); \
	} \
	ClassInfo* cls::GetClassInfo(){ \
		ASSERT(m_class_info____ && "class info must registered befor you use it"); \
		return m_class_info____; \
	} \
	void cls::RegisterClass(){ \
		if(0 != m_class_info____){ return; } \
		m_class_info____ =SafeNew< ClassInfo >(String::New(#cls), &cls::Creator); \
		ObjectFactory::Instance()->registerClassInfo(m_class_info____);

#define END_CLASS_INFO \
	}

#define FIELD_INFO(p, name, attribute) do{ auto fi =::core::MakeFieldInfo(p, STR(name), STR(attribute)); m_class_info____->appendFieldInfo(fi); }while(0);

#define OBJECT_CREATOR(cls) cls::Creator
#define CLASS_NAME(obj) ((obj)->getClassName())
#define CLASS_INFO(obj) ((obj)->getClassInfo())

/** new **/
#define SUPPORT_NEWABLE \
	template<typename _T>	\
	friend _T* ::core::SafeNew();	\
	template<typename _T, typename _A1> \
	friend _T* ::core::SafeNew(_A1 a1); \
	template<typename _T, typename _A1, typename _A2> \
	friend _T* ::core::SafeNew(_A1 a1, _A2 a2); \
	template<typename _T, typename _A1, typename _A2, typename _A3> \
	friend _T* ::core::SafeNew(_A1 a1, _A2 a2, _A3 a3); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4> \
	friend _T* ::core::SafeNew(_A1 a1, _A2 a2, _A3 a3, _A4 a4); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5> \
	friend _T* ::core::SafeNew(_A1 a1, _A2 a2, _A3 a3, _A4 a4, _A5 a5); \
	\
	template<typename _T>	\
	friend _T* ::core::New();	\
	template<typename _T, typename _A1> \
	friend _T* ::core::New(_A1 a1); \
	template<typename _T, typename _A1, typename _A2> \
	friend _T* ::core::New(_A1 a1, _A2 a2); \
	template<typename _T, typename _A1, typename _A2, typename _A3> \
	friend _T* ::core::New(_A1 a1, _A2 a2, _A3 a3); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4> \
	friend _T* ::core::New(_A1 a1, _A2 a2, _A3 a3, _A4 a4); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5> \
	friend _T* ::core::New(_A1 a1, _A2 a2, _A3 a3, _A4 a4, _A5 a5); \
	\
	template<typename _T>	\
	friend _T* ::core::CppNew(); \
	template<typename _T, typename _A1> \
	friend _T* ::core::CppNew(_A1 a1); \
	template<typename _T, typename _A1, typename _A2> \
	friend _T* ::core::CppNew(_A1 a1, _A2 a2); \
	template<typename _T, typename _A1, typename _A2, typename _A3> \
	friend _T* ::core::CppNew(_A1 a1, _A2 a2, _A3 a3); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4> \
	friend _T* ::core::CppNew(_A1 a1, _A2 a2, _A3 a3, _A4 a4); \
	template<typename _T, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5> \
	friend _T* ::core::CppNew(_A1 a1, _A2 a2, _A3 a3, _A4 a4, _A5 a5);

/** thread local singleton **/
#define DECLARE_THREAD_LOCAL_SINGLETON(cls_name)	\
	public:	\
	static void Init();	\
	static cls_name* Instance(); \
	static void Release();

#define DEFINE_THREAD_LOCAL_SINGLETON(cls_name)	\
	static thread_local cls_name* m_instance_ ## cls_name; \
	void cls_name::Init(){	\
		cls_name* ins =New< cls_name >(); \
		ins->retain(); \
		m_instance_ ## cls_name =ins; \
	} \
	cls_name* cls_name::Instance(){	\
		return m_instance_ ## cls_name; \
	} \
	void cls_name::Release(){	\
		cls_name* ins =m_instance_ ## cls_name; \
		RELEASE_POINTER(ins); \
		m_instance_ ## cls_name =0; \
	}

/** process local singleton **/
#define DECLARE_PROCESS_LOCAL_SINGLETON(cls_name)	\
	public:	\
	static cls_name* Instance();

#define DEFINE_PROCESS_LOCAL_SINGLETON(cls_name)	\
	cls_name* cls_name::Instance(){	\
		::core::ProcessLocal* pls =::core::ProcessLocal::Instance();	\
		return pls ? pls->get##cls_name() : 0;	\
	}

/** math **/
#define MAX(a, b) ((a)<(b) ? (b) : (a))
#define MIN(a, b) ((a)>(b) ? (b) : (a))
#define CLAMP(v, a, b) (((v)<=(a)) ? (a) : ((v)>=(b) ? (b) : (v)))
#define SLERP(a, b, f) ((a) + (((b)-(a))*(f)))

/** property **/
#define DECLARE_PROPERTY(type, name) \
	public: \
		type get##name(); \
		void set##name(type v); \
	private: \
		type m_##name;

#define DEFINE_PROPERTY(cls, type, name) \
		type cls::get##name(){ return m_##name; } \
		void cls::set##name(type v){ m_##name =v; }

#define DECLARE_PROPERTY_P(type, name) \
	public: \
		type get##name(); \
		void set##name(type v); \
	private: \
		type m_##name =0;

#define DEFINE_PROPERTY_P(cls, type, name) \
		type cls::get##name(){ return m_##name; } \
		void cls::set##name(type v){ ASSIGN_POINTER(m_##name, v); }

/** property dynamic array **/
#define DECLARE_PROPERTY_DYNAMIC_ARRAY(arr_type, type, name) \
	public: \
		int64_t get##name##Count(); \
		type get##name(const int64_t idx); \
		void set##name(const int64_t idx, type v); \
		void remove##name(const int64_t idx); \
		void append##name(type v); \
		void pop##name(type v); \
		void insert##name(const int64_t idx, type v); \
	private: \
		arr_type m_##name =0;

#define DEFINE_PROPERTY_DYNAMIC_ARRAY(cls, arr_type, item_type, name) \
		int64_t cls::get##name##Count(){ return m_##name ? m_##name->size() : 0; } \
		item_type cls::get##name(const int64_t idx){ typedef item_type t; if(m_##name && idx>=0 && idx<m_##name->size()){ return (item_type)(m_##name->get(idx)); }else{ return t(); }  } \
		void cls::set##name(const int64_t idx, item_type v){ if(m_##name && idx>=0 && idx<m_##name->size()){ m_##name->set(idx, v); } } \
		void cls::remove##name(const int64_t idx){ if(m_##name){ m_##name->remove(idx); } } \
		void cls::append##name(item_type v){ if(m_##name == 0){ m_##name =::core::New< std::remove_pointer<arr_type>::type >(); m_##name->retain(); } m_##name->push_back(v); } \
		void cls::pop##name(item_type v){ if(m_##name){ m_##name->pop_back(); } } \
		void cls::insert##name(const int64_t idx, item_type v){ if(m_##name == 0){ m_##name =::core::New< std::remove_pointer<arr_type>::type >(); m_##name->retain(); } m_##name->insert(idx, v); }

/** property fixed array **/
#define DECLARE_PROPERTY_FIXED_ARRAY(type, name, n) \
	public: \
		int64_t get##name##Count(); \
		type get##name(const int64_t idx); \
		void set##name(const int64_t idx, type v); \
	private: \
		type m_##name[n];

#define DEFINE_PROPERTY_FIXED_ARRAY(cls, type, name, n) \
		int64_t cls::get##name##Count(){ return n; } \
		type cls::get##name(const int64_t idx){ return m_##name[idx]; } \
		void cls::set##name(const int64_t idx, type v){ m_##name[idx] =v; }

#define DEFINE_PROPERTY_FIXED_ARRAY_P(cls, type, name, n) \
		int64_t cls::get##name##Count(){ return n; } \
		type cls::get##name(const int64_t idx){ return m_##name[idx]; } \
		void cls::set##name(const int64_t idx, type v){ ASSIGN_POINTER(m_##name[idx], v); }

/** script **/
#define DECLARE_SUPPORT_LUA_SCRIPT public: virtual const char* getLuaMetatableName();
#define DECLARE_SUPPORT_SCRIPT DECLARE_SUPPORT_LUA_SCRIPT

#define DEFINE_SUPPORT_LUA_SCRIPT(cls, name) const char* cls::getLuaMetatableName(){ return "mt_*" name "*_tm"; }
#define DEFINE_SUPPORT_SCRIPT DEFINE_SUPPORT_LUA_SCRIPT

#define MAKE_LUA_METATABLE_NAME(n) "mt_*" #n "*_tm"

/** tricky **/
#define function []

/** generate id **/
#define GENERATE_ID(id) do{ if(id > 0x7FFFFFFFFFFFFll){ id=1; }else{ ++id;} }while(0)

/** invade list **/
#define LIST_DEL(head, tail, node, name) \
	do{ \
		auto n =(node); \
		if(!n) break; \
		if(n->next_ ## name==0 && n->prev_ ## name==0) break; \
		if(n->prev_ ## name){ \
			n->prev_ ## name->next_ ## name =n->next_ ## name; \
		} \
		else{ \
			head =n->next_ ## name; \
		} \
		if(n->next_ ## name){ \
			n->next_ ## name->prev_ ## name =n->prev_ ## name; \
		} \
		else{ \
			tail =n->prev_ ## name; \
		} \
		if(n->prev_ ## name) n->prev_ ## name =0; \
		if(n->next_ ## name) n->next_ ## name =0; \
	}while(0)

#define LIST_APPEND(head, tail, node, name) \
		do{ \
			auto n =(node); \
			if(!n) break; \
			DEL_FROM_LIST(n); \
			if(tail){ \
				ASSERT(head); \
				tail->next_ ## name =n; \
				n->prev_ ## name =tail; \
				tail =n;\
			} \
			else{ \
				ASSERT(head == 0); \
				head =n; \
				tail =n; \
			} \
		}while(0)

#define LIST_INSERT_AFTER(anchor, node, name) \
		do{ \
			auto a =(anchor); \
			if(!a) break; \
			auto n =(node); \
			if(!n) break; \
			DEL_FROM_LIST(n); \
			n->prev ## name =a; \
			n->next ## name =a->next ## name; \
			if(a->next ## name){ \
				a->next_ ## name->prev ## name=n; \
			} \
			a->next_ ## name =n; \
		}while(0)

/** callback service helper **/
/* command */
#define DECLARE_COMMAND(name) private: static int64_t name(Command* command);

#define IMPL_COMMAND(cls, name, req_t, res_t) \
	int64_t cls::name(Command* command){ \
		ENSURE(command); \
		cls* self =dynamic_cast< cls* >(command->getService()); \
		ENSURE(self); \
		req_t* request =dynamic_cast< req_t* >(command->getRequest()); \
		res_t* respond =dynamic_cast< res_t* >(command->getRespond()); \
		ENSURE(request && respond);

#define END_COMMAND }

/* notify */
#define DECLARE_NOTIFY(name) private: static int64_t name(Command* command);

#define IMPL_NOTIFY(cls, name, req_t) \
	int64_t cls::name(Command* command){ \
		ENSURE(command); \
		cls* self =dynamic_cast< cls* >(command->getService()); \
		ENSURE(self); \
		req_t* request =dynamic_cast< req_t* >(command->getRequest()); \
		ENSURE(request);

#define END_NOTIFY }

#endif
