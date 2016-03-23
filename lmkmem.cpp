#include "xlualib.h"

static int LUA_C_readmem(lua_State* ls)
  {
  const char* lp = (const char*)luaL_checkinteger(ls, 1);
  const intptr_t len = luaL_checkinteger(ls, 2);
  if(lp == nullptr || len <= 0) return 0;
  try
    {
    lua_pushlstring(ls, lp, len);
    return 1;
    }
  catch(...)
    {
    ;
    }
  lua_pushlstring(ls, "", 0);
  return 1;
  }

void register_mkmem(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "readmem", LUA_C_readmem);

  lua_pop(ls, lua_gettop(ls));
  }