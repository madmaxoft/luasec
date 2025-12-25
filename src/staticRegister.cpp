// Registers the entire LuaSec library

#include <string.h>
extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include "ssl.h"
}





// The Lua sources stored in separate cpp files:
extern "C" extern const char LuaSrc_https[];
extern "C" extern const char LuaSrc_ssl[];





void requirestring(lua_State * L, const char * aModuleName, const char * aCode)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
	lua_getfield(L, -1, aModuleName);  /* LOADED[aModuleName] */
	if (!lua_toboolean(L, -1))  /* package not already loaded? */
	{
		lua_pop(L, 1);  /* remove field */
		luaL_loadbuffer(L, aCode, strlen(aCode), aModuleName);
		lua_call(L, 0, 1);
		lua_pushvalue(L, -1);  /* make copy of module (call result) */
		lua_setfield(L, -3, aModuleName);  /* LOADED[aModuleName] = module */
	}
	lua_remove(L, -2);  /* remove LOADED table */
}





extern "C" void luaopen_luasec(lua_State * L)
{
	luaL_requiref(L, "ssl.core", &luaopen_ssl_core, false);
	requirestring(L, "https", LuaSrc_https);
	requirestring(L, "ssl",   LuaSrc_ssl);
}
