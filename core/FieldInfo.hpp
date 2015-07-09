/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_FIELD_INFO_H__
#define H_CORE_FIELD_INFO_H__

namespace core{
	/** FieldInfo **/
	template<typename CLASS_T, typename FIELD_T>
	class FieldInfo: public FieldInfoBase{
		SUPPORT_NEWABLE
		typedef FieldInfoBase Super;
	public:
		typedef CLASS_T class_type;
		typedef FIELD_T field_type;
	private:
		FieldInfo(field_type class_type::* p, String* name, String* attribute)
			: FieldInfoBase(name, field_info_type<field_type>::value, attribute)
			, m_p(p){
			ASSERT(m_p);
		}
	public:
		virtual void finalize(){
			Super::finalize();
		}
	public:
		virtual void load(Object* obj){
			if(class_type* real_obj =dynamic_cast< class_type* >(obj)){
				field_type& value =real_obj->*m_p;
				load_field_value<field_type>(value, m_field_value);
			}
		}
		virtual void store(Object* obj){
			if(class_type* real_obj =dynamic_cast< class_type* >(obj)){
				field_type& value =real_obj->*m_p;
				store_field_value<field_type>(value, m_field_value);
			}
		}
	private:
		field_type class_type::* m_p;
	};
}
#endif
