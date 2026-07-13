#include "pch.h"
#include "luabase.h"
//
//#include "lua.hpp"
//
//#include "tolua.h"
//#include "tolua_cmd/luaglue_cmd.h"
//#include "lua_cmd.h"
//
//// luaの標準ライブラリテーブル
//static const luaL_Reg loadedlibs[] =
//{
//	{ "_G", luaopen_base },
//	{ LUA_LOADLIBNAME, luaopen_package },
//	{ LUA_COLIBNAME, luaopen_coroutine },
//	{ LUA_TABLIBNAME, luaopen_table },
//	{ LUA_IOLIBNAME, luaopen_io },
//	//  {LUA_OSLIBNAME, luaopen_os},
//	{ LUA_STRLIBNAME, luaopen_string },
//	{ LUA_MATHLIBNAME, luaopen_math },
//	{ LUA_UTF8LIBNAME, luaopen_utf8 },
//	{ LUA_DBLIBNAME, luaopen_debug },
//#if defined(LUA_COMPAT_BITLIB)
//	{ LUA_BITLIBNAME, luaopen_bit32 },
//#endif
//	{ NULL, NULL }
//};
//
//static void _luaL_openlibs(lua_State* L)
//{
//	const luaL_Reg* lib;
//	/* "require" functions from 'loadedlibs' and set results to global table */
//	for(lib = loadedlibs; lib->func; lib++)
//	{
//		luaL_requiref(L, lib->name, lib->func, 1);
//		lua_pop(L, 1);  /* remove lib */
//	}
//}
//
//static lua_State* _luaL_RegistCoroutine(lua_State* L, const char* funcName)
//{
//	// コルーチンの生成とfunctionの登録
//	lua_State* co = lua_newthread(L);
//	lua_getglobal(co, funcName);
//	return co;
//}
//
//static void _luaL_ErrMsg(lua_State* L, int errcode)
//{
//	std::string luaErrMsg = lua_tostring(L, lua_gettop(L));
//	MessageBox(NULL, luaErrMsg.c_str(), "lua error", MB_OK);
//}