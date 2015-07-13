/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_HTML_TEMPLATE_H__
#define H_CORE_HTML_TEMPLATE_H__

namespace core{
	/** HtmlTemplate **/
	class HtmlTemplate :public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	private:
		HtmlTemplate();
		virtual ~HtmlTemplate();
	public:
		virtual void init();
		virtual void finalize();
	public:
		String* render(String* source, Hash* param);
		bool good();
	private:
		int64_t _push_hash(Hash* param);
	private:
		lua_State* m_L;
	};
}
#endif
