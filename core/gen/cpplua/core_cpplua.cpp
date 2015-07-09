#include "core_cpplua.h"

namespace core{
namespace cpplua{

/** register core::BitOp **/
class CL_BitOp{
	static int And(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::And(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Or(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Or(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Xor(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Xor(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Negate(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Negate(a1);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Lshift(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Lshift(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Rshift(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Rshift(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Test(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		boolean_t ret =core::BitOp::Test(a1, a2);
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int Set(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Set(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Clear(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Clear(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Flip(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Flip(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Fetch(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a3;
		if(!::core::FromLua< uint64_t >(L, 3, a3)){
			ERROR("fail to call %s, arg a3 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Fetch(a1, a2, a3);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int Replace(lua_State* L){
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a3;
		if(!::core::FromLua< uint64_t >(L, 3, a3)){
			ERROR("fail to call %s, arg a3 invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a4;
		if(!::core::FromLua< uint64_t >(L, 4, a4)){
			ERROR("fail to call %s, arg a4 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =core::BitOp::Replace(a1, a2, a3, a4);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.BitOp")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "BitOp");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::BitOp");
		lua_setfield(L, -2, "qualified_name");
		// static
		lua_pushcfunction(L, CL_BitOp::And);
		lua_setfield(L, -2, "And");

		lua_pushcfunction(L, CL_BitOp::Or);
		lua_setfield(L, -2, "Or");

		lua_pushcfunction(L, CL_BitOp::Xor);
		lua_setfield(L, -2, "Xor");

		lua_pushcfunction(L, CL_BitOp::Negate);
		lua_setfield(L, -2, "Negate");

		lua_pushcfunction(L, CL_BitOp::Lshift);
		lua_setfield(L, -2, "Lshift");

		lua_pushcfunction(L, CL_BitOp::Rshift);
		lua_setfield(L, -2, "Rshift");

		lua_pushcfunction(L, CL_BitOp::Test);
		lua_setfield(L, -2, "Test");

		lua_pushcfunction(L, CL_BitOp::Set);
		lua_setfield(L, -2, "Set");

		lua_pushcfunction(L, CL_BitOp::Clear);
		lua_setfield(L, -2, "Clear");

		lua_pushcfunction(L, CL_BitOp::Flip);
		lua_setfield(L, -2, "Flip");

		lua_pushcfunction(L, CL_BitOp::Fetch);
		lua_setfield(L, -2, "Fetch");

		lua_pushcfunction(L, CL_BitOp::Replace);
		lua_setfield(L, -2, "Replace");

		return true;
	}
};
/** register core::BitSet **/
class CL_BitSet{
	static int New(lua_State* L){
		/* get arg */
		/* call */
		BitSet* ret =core::BitSet::New();
		::core::push_object_to_lua< BitSet* >(L, ret);
		return 1;
	}
	static int fromString(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->fromString(a1);
		return 0;
	}
	static int toString(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		::core::String* ret =self->toString();
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int setUint64(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->setUint64(a1);
		return 0;
	}
	static int asUint64(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint64_t ret =self->asUint64();
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int count(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint64_t ret =self->count();
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int test(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		boolean_t ret =self->test(a1);
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int set(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->set(a1);
		return 0;
	}
	static int flip(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->flip(a1);
		return 0;
	}
	static int clear(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->clear(a1);
		return 0;
	}
	static int clearAll(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		self->clearAll();
		return 0;
	}
	static int fetch(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 3, a2)){
			ERROR("fail to call a2, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint64_t ret =self->fetch(a1, a2);
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int replace(lua_State* L){
		/* get self */
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a2;
		if(!::core::FromLua< uint64_t >(L, 3, a2)){
			ERROR("fail to call a2, arg %s invalid", __FUNCTION__);
			return 0;
		}
		uint64_t a3;
		if(!::core::FromLua< uint64_t >(L, 4, a3)){
			ERROR("fail to call a3, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->replace(a1, a2, a3);
		return 0;
	}
public:
	static int GC(lua_State*L){
		core::BitSet* self =0;
		if(!get_object_from_lua< core::BitSet >(L, 1, self)){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		RELEASE_POINTER(self);
		return 0;
	}
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.BitSet")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "BitSet");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::BitSet");
		lua_setfield(L, -2, "qualified_name");
		// metatable
		lua_getfield(L, -1, "metatable");
		if(lua_istable(L, -1) == 0){
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "metatable");
		}
		lua_pushcfunction(L, CL_BitSet::fromString);
		lua_setfield(L, -2, "fromString");

		lua_pushcfunction(L, CL_BitSet::toString);
		lua_setfield(L, -2, "toString");

		lua_pushcfunction(L, CL_BitSet::setUint64);
		lua_setfield(L, -2, "setUint64");

		lua_pushcfunction(L, CL_BitSet::asUint64);
		lua_setfield(L, -2, "asUint64");

		lua_pushcfunction(L, CL_BitSet::count);
		lua_setfield(L, -2, "count");

		lua_pushcfunction(L, CL_BitSet::test);
		lua_setfield(L, -2, "test");

		lua_pushcfunction(L, CL_BitSet::set);
		lua_setfield(L, -2, "set");

		lua_pushcfunction(L, CL_BitSet::flip);
		lua_setfield(L, -2, "flip");

		lua_pushcfunction(L, CL_BitSet::clear);
		lua_setfield(L, -2, "clear");

		lua_pushcfunction(L, CL_BitSet::clearAll);
		lua_setfield(L, -2, "clearAll");

		lua_pushcfunction(L, CL_BitSet::fetch);
		lua_setfield(L, -2, "fetch");

		lua_pushcfunction(L, CL_BitSet::replace);
		lua_setfield(L, -2, "replace");

		lua_pushcfunction(L, CL_BitSet::GC);
		lua_setfield(L, -2, "__gc");

		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_setglobal(L, "mt_*core::BitSet*_tm");
		// static
		lua_pushcfunction(L, CL_BitSet::New);
		lua_setfield(L, -2, "New");

		return true;
	}
};
/** register core::Bytes **/
class CL_Bytes{
	static int New(lua_State* L){
		/* get arg */
		/* call */
		core::Bytes* ret =core::Bytes::New();
		::core::push_object_to_lua< core::Bytes* >(L, ret);
		return 1;
	}
	static int good(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		boolean_t ret =self->good();
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int bad(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		boolean_t ret =self->bad();
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int rewind(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		self->rewind();
		return 0;
	}
	static int getReadCursor(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int64_t ret =self->getReadCursor();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int getWriteCursor(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int64_t ret =self->getWriteCursor();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int setReadCursor(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->setReadCursor(a1);
		return 0;
	}
	static int setWriteCursor(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->setWriteCursor(a1);
		return 0;
	}
	static int skip(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->skip(a1);
		return 0;
	}
	static int size(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int64_t ret =self->size();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int encodeBoolean(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		boolean_t a1;
		if(!::core::FromLua< boolean_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeBoolean(a1);
		return 0;
	}
	static int decodeBoolean(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		boolean_t ret =self->decodeBoolean();
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int encodeInt64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeInt64(a1);
		return 0;
	}
	static int decodeInt64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int64_t ret =self->decodeInt64();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int encodeUint64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeUint64(a1);
		return 0;
	}
	static int decodeUint64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint64_t ret =self->decodeUint64();
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int encodeFloat32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		float32_t a1;
		if(!::core::FromLua< float32_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeFloat32(a1);
		return 0;
	}
	static int decodeFloat32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		float32_t ret =self->decodeFloat32();
		::core::ToLua< float32_t >(L, ret);
		return 1;
	}
	static int encodeFloat64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		float64_t a1;
		if(!::core::FromLua< float64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeFloat64(a1);
		return 0;
	}
	static int decodeFloat64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		float64_t ret =self->decodeFloat64();
		::core::ToLua< float64_t >(L, ret);
		return 1;
	}
	static int encodeString(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->encodeString(a1);
		return 0;
	}
	static int decodeString(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		::core::String* ret =self->decodeString();
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int readInt8(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int8_t ret =self->readInt8();
		::core::ToLua< int8_t >(L, ret);
		return 1;
	}
	static int readInt16(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int16_t ret =self->readInt16();
		::core::ToLua< int16_t >(L, ret);
		return 1;
	}
	static int readInt32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int32_t ret =self->readInt32();
		::core::ToLua< int32_t >(L, ret);
		return 1;
	}
	static int readInt64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		int64_t ret =self->readInt64();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int readUint8(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint8_t ret =self->readUint8();
		::core::ToLua< uint8_t >(L, ret);
		return 1;
	}
	static int readUint16(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint16_t ret =self->readUint16();
		::core::ToLua< uint16_t >(L, ret);
		return 1;
	}
	static int readUint32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint32_t ret =self->readUint32();
		::core::ToLua< uint32_t >(L, ret);
		return 1;
	}
	static int readUint64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		uint64_t ret =self->readUint64();
		::core::ToLua< uint64_t >(L, ret);
		return 1;
	}
	static int readFloat32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		float32_t ret =self->readFloat32();
		::core::ToLua< float32_t >(L, ret);
		return 1;
	}
	static int readFloat64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		/* call */
		float64_t ret =self->readFloat64();
		::core::ToLua< float64_t >(L, ret);
		return 1;
	}
	static int writeInt8(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int8_t a1;
		if(!::core::FromLua< int8_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeInt8(a1);
		return 0;
	}
	static int writeInt16(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int16_t a1;
		if(!::core::FromLua< int16_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeInt16(a1);
		return 0;
	}
	static int writeInt32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int32_t a1;
		if(!::core::FromLua< int32_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeInt32(a1);
		return 0;
	}
	static int writeInt64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeInt64(a1);
		return 0;
	}
	static int writeUint8(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint8_t a1;
		if(!::core::FromLua< uint8_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeUint8(a1);
		return 0;
	}
	static int writeUint16(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint16_t a1;
		if(!::core::FromLua< uint16_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeUint16(a1);
		return 0;
	}
	static int writeUint32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint32_t a1;
		if(!::core::FromLua< uint32_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeUint32(a1);
		return 0;
	}
	static int writeUint64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		uint64_t a1;
		if(!::core::FromLua< uint64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeUint64(a1);
		return 0;
	}
	static int writeFloat32(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		float32_t a1;
		if(!::core::FromLua< float32_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeFloat32(a1);
		return 0;
	}
	static int writeFloat64(lua_State* L){
		/* get self */
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		float64_t a1;
		if(!::core::FromLua< float64_t >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		self->writeFloat64(a1);
		return 0;
	}
public:
	static int GC(lua_State*L){
		core::Bytes* self =0;
		if(!get_object_from_lua< core::Bytes >(L, 1, self)){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		RELEASE_POINTER(self);
		return 0;
	}
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.Bytes")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "Bytes");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::Bytes");
		lua_setfield(L, -2, "qualified_name");
		// metatable
		lua_getfield(L, -1, "metatable");
		if(lua_istable(L, -1) == 0){
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "metatable");
		}
		lua_pushcfunction(L, CL_Bytes::good);
		lua_setfield(L, -2, "good");

		lua_pushcfunction(L, CL_Bytes::bad);
		lua_setfield(L, -2, "bad");

		lua_pushcfunction(L, CL_Bytes::rewind);
		lua_setfield(L, -2, "rewind");

		lua_pushcfunction(L, CL_Bytes::getReadCursor);
		lua_setfield(L, -2, "getReadCursor");

		lua_pushcfunction(L, CL_Bytes::getWriteCursor);
		lua_setfield(L, -2, "getWriteCursor");

		lua_pushcfunction(L, CL_Bytes::setReadCursor);
		lua_setfield(L, -2, "setReadCursor");

		lua_pushcfunction(L, CL_Bytes::setWriteCursor);
		lua_setfield(L, -2, "setWriteCursor");

		lua_pushcfunction(L, CL_Bytes::skip);
		lua_setfield(L, -2, "skip");

		lua_pushcfunction(L, CL_Bytes::size);
		lua_setfield(L, -2, "size");

		lua_pushcfunction(L, CL_Bytes::encodeBoolean);
		lua_setfield(L, -2, "encodeBoolean");

		lua_pushcfunction(L, CL_Bytes::decodeBoolean);
		lua_setfield(L, -2, "decodeBoolean");

		lua_pushcfunction(L, CL_Bytes::encodeInt64);
		lua_setfield(L, -2, "encodeInt64");

		lua_pushcfunction(L, CL_Bytes::decodeInt64);
		lua_setfield(L, -2, "decodeInt64");

		lua_pushcfunction(L, CL_Bytes::encodeUint64);
		lua_setfield(L, -2, "encodeUint64");

		lua_pushcfunction(L, CL_Bytes::decodeUint64);
		lua_setfield(L, -2, "decodeUint64");

		lua_pushcfunction(L, CL_Bytes::encodeFloat32);
		lua_setfield(L, -2, "encodeFloat32");

		lua_pushcfunction(L, CL_Bytes::decodeFloat32);
		lua_setfield(L, -2, "decodeFloat32");

		lua_pushcfunction(L, CL_Bytes::encodeFloat64);
		lua_setfield(L, -2, "encodeFloat64");

		lua_pushcfunction(L, CL_Bytes::decodeFloat64);
		lua_setfield(L, -2, "decodeFloat64");

		lua_pushcfunction(L, CL_Bytes::encodeString);
		lua_setfield(L, -2, "encodeString");

		lua_pushcfunction(L, CL_Bytes::decodeString);
		lua_setfield(L, -2, "decodeString");

		lua_pushcfunction(L, CL_Bytes::readInt8);
		lua_setfield(L, -2, "readInt8");

		lua_pushcfunction(L, CL_Bytes::readInt16);
		lua_setfield(L, -2, "readInt16");

		lua_pushcfunction(L, CL_Bytes::readInt32);
		lua_setfield(L, -2, "readInt32");

		lua_pushcfunction(L, CL_Bytes::readInt64);
		lua_setfield(L, -2, "readInt64");

		lua_pushcfunction(L, CL_Bytes::readUint8);
		lua_setfield(L, -2, "readUint8");

		lua_pushcfunction(L, CL_Bytes::readUint16);
		lua_setfield(L, -2, "readUint16");

		lua_pushcfunction(L, CL_Bytes::readUint32);
		lua_setfield(L, -2, "readUint32");

		lua_pushcfunction(L, CL_Bytes::readUint64);
		lua_setfield(L, -2, "readUint64");

		lua_pushcfunction(L, CL_Bytes::readFloat32);
		lua_setfield(L, -2, "readFloat32");

		lua_pushcfunction(L, CL_Bytes::readFloat64);
		lua_setfield(L, -2, "readFloat64");

		lua_pushcfunction(L, CL_Bytes::writeInt8);
		lua_setfield(L, -2, "writeInt8");

		lua_pushcfunction(L, CL_Bytes::writeInt16);
		lua_setfield(L, -2, "writeInt16");

		lua_pushcfunction(L, CL_Bytes::writeInt32);
		lua_setfield(L, -2, "writeInt32");

		lua_pushcfunction(L, CL_Bytes::writeInt64);
		lua_setfield(L, -2, "writeInt64");

		lua_pushcfunction(L, CL_Bytes::writeUint8);
		lua_setfield(L, -2, "writeUint8");

		lua_pushcfunction(L, CL_Bytes::writeUint16);
		lua_setfield(L, -2, "writeUint16");

		lua_pushcfunction(L, CL_Bytes::writeUint32);
		lua_setfield(L, -2, "writeUint32");

		lua_pushcfunction(L, CL_Bytes::writeUint64);
		lua_setfield(L, -2, "writeUint64");

		lua_pushcfunction(L, CL_Bytes::writeFloat32);
		lua_setfield(L, -2, "writeFloat32");

		lua_pushcfunction(L, CL_Bytes::writeFloat64);
		lua_setfield(L, -2, "writeFloat64");

		lua_pushcfunction(L, CL_Bytes::GC);
		lua_setfield(L, -2, "__gc");

		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_setglobal(L, "mt_*core::Bytes*_tm");
		// static
		lua_pushcfunction(L, CL_Bytes::New);
		lua_setfield(L, -2, "New");

		return true;
	}
};
/** register core::Command **/
class CL_Command{
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.Command")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "Command");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::Command");
		lua_setfield(L, -2, "qualified_name");
		// enum
		lua_pushinteger(L, core::Command::STATE_INIT);
		lua_setfield(L, -2, "STATE_INIT");
		lua_pushinteger(L, core::Command::STATE_COMPLETE);
		lua_setfield(L, -2, "STATE_COMPLETE");
		lua_pushinteger(L, core::Command::STATE_ERROR);
		lua_setfield(L, -2, "STATE_ERROR");
		return true;
	}
};
/** register core::DateTime **/
class CL_DateTime{
	static int Now(lua_State* L){
		/* get arg */
		/* call */
		int64_t ret =core::DateTime::Now();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.DateTime")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "DateTime");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::DateTime");
		lua_setfield(L, -2, "qualified_name");
		// static
		lua_pushcfunction(L, CL_DateTime::Now);
		lua_setfield(L, -2, "Now");

		return true;
	}
};
/** register core::Endian **/
class CL_Endian{
	static int Net2HostU16(lua_State* L){
		/* get arg */
		uint16_t a1;
		if(!::core::FromLua< uint16_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Endian::Net2HostU16(a1);
		return 0;
	}
	static int Net2HostU32(lua_State* L){
		/* get arg */
		uint32_t a1;
		if(!::core::FromLua< uint32_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint32_t ret =core::Endian::Net2HostU32(a1);
		::core::ToLua< uint32_t >(L, ret);
		return 1;
	}
	static int Host2NetU16(lua_State* L){
		/* get arg */
		uint16_t a1;
		if(!::core::FromLua< uint16_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint16_t ret =core::Endian::Host2NetU16(a1);
		::core::ToLua< uint16_t >(L, ret);
		return 1;
	}
	static int Host2NetU32(lua_State* L){
		/* get arg */
		uint32_t a1;
		if(!::core::FromLua< uint32_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		uint32_t ret =core::Endian::Host2NetU32(a1);
		::core::ToLua< uint32_t >(L, ret);
		return 1;
	}
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.Endian")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "Endian");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::Endian");
		lua_setfield(L, -2, "qualified_name");
		// static
		lua_pushcfunction(L, CL_Endian::Net2HostU16);
		lua_setfield(L, -2, "Net2HostU16");

		lua_pushcfunction(L, CL_Endian::Net2HostU32);
		lua_setfield(L, -2, "Net2HostU32");

		lua_pushcfunction(L, CL_Endian::Host2NetU16);
		lua_setfield(L, -2, "Host2NetU16");

		lua_pushcfunction(L, CL_Endian::Host2NetU32);
		lua_setfield(L, -2, "Host2NetU32");

		return true;
	}
};
/** register core::ErrorCode **/
class CL_ErrorCode{
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.ErrorCode")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "ErrorCode");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::ErrorCode");
		lua_setfield(L, -2, "qualified_name");
		// enum
		lua_pushinteger(L, core::ErrorCode::OK);
		lua_setfield(L, -2, "OK");
		lua_pushinteger(L, core::ErrorCode::ERROR);
		lua_setfield(L, -2, "ERROR");
		lua_pushinteger(L, core::ErrorCode::SYSTEM_ERROR);
		lua_setfield(L, -2, "SYSTEM_ERROR");
		lua_pushinteger(L, core::ErrorCode::SYSTEM_BUSY);
		lua_setfield(L, -2, "SYSTEM_BUSY");
		lua_pushinteger(L, core::ErrorCode::INVALID_ARG);
		lua_setfield(L, -2, "INVALID_ARG");
		lua_pushinteger(L, core::ErrorCode::UNKNOWN_COMMAND);
		lua_setfield(L, -2, "UNKNOWN_COMMAND");
		lua_pushinteger(L, core::ErrorCode::TIMEOUT);
		lua_setfield(L, -2, "TIMEOUT");
		lua_pushinteger(L, core::ErrorCode::INVALID_BINARY_DATA);
		lua_setfield(L, -2, "INVALID_BINARY_DATA");
		lua_pushinteger(L, core::ErrorCode::NOT_EXIST);
		lua_setfield(L, -2, "NOT_EXIST");
		lua_pushinteger(L, core::ErrorCode::ALREADY_EXIST);
		lua_setfield(L, -2, "ALREADY_EXIST");
		lua_pushinteger(L, core::ErrorCode::NOT_ENOUGH);
		lua_setfield(L, -2, "NOT_ENOUGH");
		lua_pushinteger(L, core::ErrorCode::CANCELLED);
		lua_setfield(L, -2, "CANCELLED");
		return true;
	}
};
/** register core::FileSystem **/
class CL_FileSystem{
	static int CleanPath(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		::core::String* ret =core::FileSystem::CleanPath(a1);
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int ParseDir(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		::core::String* ret =core::FileSystem::ParseDir(a1);
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int ParseExt(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		::core::String* ret =core::FileSystem::ParseExt(a1);
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int ParseName(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		::core::String* ret =core::FileSystem::ParseName(a1);
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int Join(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a2;
		if(!::core::FromLua< ::core::String* >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		::core::String* ret =core::FileSystem::Join(a1, a2);
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.FileSystem")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "FileSystem");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::FileSystem");
		lua_setfield(L, -2, "qualified_name");
		// static
		lua_pushcfunction(L, CL_FileSystem::CleanPath);
		lua_setfield(L, -2, "CleanPath");

		lua_pushcfunction(L, CL_FileSystem::ParseDir);
		lua_setfield(L, -2, "ParseDir");

		lua_pushcfunction(L, CL_FileSystem::ParseExt);
		lua_setfield(L, -2, "ParseExt");

		lua_pushcfunction(L, CL_FileSystem::ParseName);
		lua_setfield(L, -2, "ParseName");

		lua_pushcfunction(L, CL_FileSystem::Join);
		lua_setfield(L, -2, "Join");

		return true;
	}
};
/** register core::Logger **/
class CL_Logger{
	static int Printf(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Printf(a1);
		return 0;
	}
	static int Log(lua_State* L){
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a2;
		if(!::core::FromLua< ::core::String* >(L, 2, a2)){
			ERROR("fail to call %s, arg a2 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Log(a1, a2);
		return 0;
	}
	static int Info(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Info(a1);
		return 0;
	}
	static int Warn(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Warn(a1);
		return 0;
	}
	static int Error(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Error(a1);
		return 0;
	}
	static int Fatal(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Fatal(a1);
		return 0;
	}
	static int Debug(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::Debug(a1);
		return 0;
	}
	static int SetName(lua_State* L){
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::SetName(a1);
		return 0;
	}
	static int GetName(lua_State* L){
		/* get arg */
		/* call */
		::core::String* ret =core::Logger::GetName();
		::core::ToLua< ::core::String* >(L, ret);
		return 1;
	}
	static int SetLevel(lua_State* L){
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::SetLevel(a1);
		return 0;
	}
	static int GetLevel(lua_State* L){
		/* get arg */
		/* call */
		int64_t ret =core::Logger::GetLevel();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int SetFileLifeTime(lua_State* L){
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::SetFileLifeTime(a1);
		return 0;
	}
	static int GetFileLifeTime(lua_State* L){
		/* get arg */
		/* call */
		int64_t ret =core::Logger::GetFileLifeTime();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
	static int SetFlushMode(lua_State* L){
		/* get arg */
		int64_t a1;
		if(!::core::FromLua< int64_t >(L, 1, a1)){
			ERROR("fail to call %s, arg a1 invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		core::Logger::SetFlushMode(a1);
		return 0;
	}
	static int GetFlushMode(lua_State* L){
		/* get arg */
		/* call */
		int64_t ret =core::Logger::GetFlushMode();
		::core::ToLua< int64_t >(L, ret);
		return 1;
	}
public:
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.Logger")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "Logger");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::Logger");
		lua_setfield(L, -2, "qualified_name");
		// enum
		lua_pushinteger(L, core::Logger::DEBUG);
		lua_setfield(L, -2, "DEBUG");
		lua_pushinteger(L, core::Logger::INFO);
		lua_setfield(L, -2, "INFO");
		lua_pushinteger(L, core::Logger::WARN);
		lua_setfield(L, -2, "WARN");
		lua_pushinteger(L, core::Logger::ERROR);
		lua_setfield(L, -2, "ERROR");
		lua_pushinteger(L, core::Logger::FATAL);
		lua_setfield(L, -2, "FATAL");
		lua_pushinteger(L, core::Logger::USER_MIN_LEVEL);
		lua_setfield(L, -2, "USER_MIN_LEVEL");
		lua_pushinteger(L, core::Logger::USER_MAX_LEVEL);
		lua_setfield(L, -2, "USER_MAX_LEVEL");
		lua_pushinteger(L, core::Logger::SHOW_ALL);
		lua_setfield(L, -2, "SHOW_ALL");
		lua_pushinteger(L, core::Logger::SHOW_NONE);
		lua_setfield(L, -2, "SHOW_NONE");
		lua_pushinteger(L, core::Logger::FLUSH_MODE_FLUSH);
		lua_setfield(L, -2, "FLUSH_MODE_FLUSH");
		lua_pushinteger(L, core::Logger::FLUSH_MODE_SYNC);
		lua_setfield(L, -2, "FLUSH_MODE_SYNC");
		lua_pushinteger(L, core::Logger::FLUSH_MODE_DEFAULT);
		lua_setfield(L, -2, "FLUSH_MODE_DEFAULT");
		// static
		lua_pushcfunction(L, CL_Logger::Printf);
		lua_setfield(L, -2, "Printf");

		lua_pushcfunction(L, CL_Logger::Log);
		lua_setfield(L, -2, "Log");

		lua_pushcfunction(L, CL_Logger::Info);
		lua_setfield(L, -2, "Info");

		lua_pushcfunction(L, CL_Logger::Warn);
		lua_setfield(L, -2, "Warn");

		lua_pushcfunction(L, CL_Logger::Error);
		lua_setfield(L, -2, "Error");

		lua_pushcfunction(L, CL_Logger::Fatal);
		lua_setfield(L, -2, "Fatal");

		lua_pushcfunction(L, CL_Logger::Debug);
		lua_setfield(L, -2, "Debug");

		lua_pushcfunction(L, CL_Logger::SetName);
		lua_setfield(L, -2, "SetName");

		lua_pushcfunction(L, CL_Logger::GetName);
		lua_setfield(L, -2, "GetName");

		lua_pushcfunction(L, CL_Logger::SetLevel);
		lua_setfield(L, -2, "SetLevel");

		lua_pushcfunction(L, CL_Logger::GetLevel);
		lua_setfield(L, -2, "GetLevel");

		lua_pushcfunction(L, CL_Logger::SetFileLifeTime);
		lua_setfield(L, -2, "SetFileLifeTime");

		lua_pushcfunction(L, CL_Logger::GetFileLifeTime);
		lua_setfield(L, -2, "GetFileLifeTime");

		lua_pushcfunction(L, CL_Logger::SetFlushMode);
		lua_setfield(L, -2, "SetFlushMode");

		lua_pushcfunction(L, CL_Logger::GetFlushMode);
		lua_setfield(L, -2, "GetFlushMode");

		return true;
	}
};
/** register core::Url **/
class CL_Url{
	static int New(lua_State* L){
		/* get arg */
		/* call */
		Url* ret =core::Url::New();
		::core::push_object_to_lua< Url* >(L, ret);
		return 1;
	}
	static int build(lua_State* L){
		/* get self */
		core::Url* self =0;
		if(!get_object_from_lua< core::Url >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a2;
		if(!::core::FromLua< ::core::String* >(L, 3, a2)){
			ERROR("fail to call a2, arg %s invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a3;
		if(!::core::FromLua< ::core::String* >(L, 4, a3)){
			ERROR("fail to call a3, arg %s invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a4;
		if(!::core::FromLua< ::core::String* >(L, 5, a4)){
			ERROR("fail to call a4, arg %s invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a5;
		if(!::core::FromLua< ::core::String* >(L, 6, a5)){
			ERROR("fail to call a5, arg %s invalid", __FUNCTION__);
			return 0;
		}
		::core::String* a6;
		if(!::core::FromLua< ::core::String* >(L, 7, a6)){
			ERROR("fail to call a6, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		boolean_t ret =self->build(a1, a2, a3, a4, a5, a6);
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
	static int parse(lua_State* L){
		/* get self */
		core::Url* self =0;
		if(!get_object_from_lua< core::Url >(L, 1, self) || !self){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		/* get arg */
		::core::String* a1;
		if(!::core::FromLua< ::core::String* >(L, 2, a1)){
			ERROR("fail to call a1, arg %s invalid", __FUNCTION__);
			return 0;
		}
		/* call */
		boolean_t ret =self->parse(a1);
		::core::ToLua< boolean_t >(L, ret);
		return 1;
	}
public:
	static int GC(lua_State*L){
		core::Url* self =0;
		if(!get_object_from_lua< core::Url >(L, 1, self)){
			ERROR("fail to call %s, self invalid", __FUNCTION__);
			return 0;
		}
		RELEASE_POINTER(self);
		return 0;
	}
public:
	static bool Register(lua_State* L){
		LuaTopHelper tlh(L);
		// prepare global Class table
		if(!ensure_lua_global_table(L, "Core.Url")){ return false; }
		// prepare prototype table
		lua_pushstring(L, "Url");
		lua_setfield(L, -2, "name");
		lua_pushstring(L, "core::Url");
		lua_setfield(L, -2, "qualified_name");
		// metatable
		lua_getfield(L, -1, "metatable");
		if(lua_istable(L, -1) == 0){
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "metatable");
		}
		lua_pushcfunction(L, CL_Url::build);
		lua_setfield(L, -2, "build");

		lua_pushcfunction(L, CL_Url::parse);
		lua_setfield(L, -2, "parse");

		lua_pushcfunction(L, CL_Url::GC);
		lua_setfield(L, -2, "__gc");

		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_setglobal(L, "mt_*core::Url*_tm");
		// static
		lua_pushcfunction(L, CL_Url::New);
		lua_setfield(L, -2, "New");

		return true;
	}
};
bool register_cpplua(lua_State* L){
	CL_BitOp::Register(L);
	CL_BitSet::Register(L);
	CL_Bytes::Register(L);
	CL_Command::Register(L);
	CL_DateTime::Register(L);
	CL_Endian::Register(L);
	CL_ErrorCode::Register(L);
	CL_FileSystem::Register(L);
	CL_Logger::Register(L);
	CL_Url::Register(L);
	return true;
}

}
}
