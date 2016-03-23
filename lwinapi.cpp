#include "xlualib.h"

static int LUA_C_sleep(lua_State* ls)
  {
  const size_t ms = (size_t)luaL_optinteger(ls, 1, 0);
  Sleep(ms);
  return 0;
  }

static int LUA_C_gettick(lua_State* ls)
  {
  lua_pushinteger(ls, GetTickCount());
  return 1;
  }

void register_winapi(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "sleep", LUA_C_sleep);
  lua_register(ls, "gettick", LUA_C_gettick);

  lua_pop(ls, lua_gettop(ls));
  }