/*
   Copyright (C) 2014-2015 别怀山(foolbie). See Copyright Notice in core.h
*/
#ifndef H_CORE_SHA_H__
#define H_CORE_SHA_H__

namespace core{
	/** Sha **/
	class Sha: public Object{
		SUPPORT_NEWABLE
		typedef Object Super;
	public:
		enum{
			TYPE_SHA1,
			TYPE_SHA224,
			TYPE_SHA256,
			TYPE_SHA384,
			TYPE_SHA512,
			TYPE_MIN =TYPE_SHA1,
			TYPE_MAX =TYPE_SHA512
		};
		enum{
			SHA1_BUFFER_LENGTH =(SHA_DIGEST_LENGTH) * 2,
			SHA224_BUFFER_LENGTH =(SHA224_DIGEST_LENGTH) * 2,
			SHA256_BUFFER_LENGTH =(SHA256_DIGEST_LENGTH) * 2,
			SHA384_BUFFER_LENGTH =(SHA384_DIGEST_LENGTH) * 2,
			SHA512_BUFFER_LENGTH =(SHA512_DIGEST_LENGTH) * 2,
		};
		static_assert(
			SHA1_BUFFER_LENGTH < SHA224_BUFFER_LENGTH &&		
			SHA224_BUFFER_LENGTH < SHA256_BUFFER_LENGTH &&		
			SHA256_BUFFER_LENGTH < SHA384_BUFFER_LENGTH &&		
			SHA384_BUFFER_LENGTH < SHA512_BUFFER_LENGTH,
			"wtf"
		);
	public:
		static bool Sum(const int64_t type, const void* bytes, const int64_t len, char* pbuffer);
		static bool RegisterToLua(lua_State* L);
	private:
		static int _Sum(lua_State* L);
	};
}

#endif
