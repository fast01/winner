#include "core.h"

namespace core{
	/*** impl CallbackItemRpcInfo ***/
	/** ctor & dtor **/
	CallbackItemRpcInfo::CallbackItemRpcInfo(const int64_t group_id)
		: CallbackRpcInfo(group_id)
		, m_group(0)
		, m_index(-1){
	}
	CallbackItemRpcInfo::~CallbackItemRpcInfo(){
	}

	/** Object **/
	void CallbackItemRpcInfo::init(){
		Super::init();
	}
	void CallbackItemRpcInfo::finalize(){
		CLEAN_POINTER(m_group);
		Super::finalize();
	}
	/** CallbackRpcInfo **/
	int64_t CallbackItemRpcInfo::invoke(Object* param){
		const int64_t ret =m_group->set(m_index, parse(param));
		done();
		return ret;
	}
	/** self **/
	CallbackRpcGroup* CallbackItemRpcInfo::getGroup(){
		return m_group;
	}
	void CallbackItemRpcInfo::setGroup(CallbackRpcGroup* grp){
		ASSIGN_POINTER(m_group, grp);
	}
	int64_t CallbackItemRpcInfo::getIndex(){
		return m_index;
	}
	void CallbackItemRpcInfo::setIndex(const int64_t idx){
		m_index =idx;
	}
}
