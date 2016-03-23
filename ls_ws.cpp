#include "xlualib.h"

static int LUA_C_ws2s(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const wstring ws((const wchar_t*)s, l / sizeof(wchar_t));

  const string ss(ws2s(ws));

  lua_pushlstring(ls, ss.c_str(), ss.size());
  return 1;
  }

static int LUA_C_s2ws(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const wstring ws(s2ws(string(s, l)));

  lua_pushlstring(ls, (const char*)ws.c_str(), ws.size() * sizeof(wchar_t));
  return 1;
  }

void register_s_ws(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "ws2s", LUA_C_ws2s);
  lua_register(ls, "s2ws", LUA_C_s2ws);

  lua_pop(ls, lua_gettop(ls));
  }