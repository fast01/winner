#ifndef H_CORE_CALLBACK_ITEM_RPC_INFO_H__
#define H_CORE_CALLBACK_ITEM_RPC_INFO_H__

namespace core{
	/** predeclare **/
	class CallbackRpcGroup;

	/** CallbackItemRpcInfo **/
	class CallbackItemRpcInfo: public CallbackRpcInfo{
		SUPPORT_NEWABLE
		typedef CallbackRpcInfo Super;
	private:
		CallbackItemRpcInfo(const int64_t group_id);
		virtual ~CallbackItemRpcInfo();
	public:
		virtual void init();
		virtual void finalize();
	public:
		virtual int64_t invoke(Object* param);
	public:
		CallbackRpcGroup* getGroup();
		void setGroup(CallbackRpcGroup* grp);

		int64_t getIndex();
		void setIndex(const int64_t idx);
	private:
		CallbackRpcGroup* m_group;
		int64_t m_index;
	};
}

#endif
