#include "xlualib.h"

static int LUA_C_utf82ws(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const xutf8 u((p_utf8)s, l);

  const wstring ws(utf82ws(u));
  lua_pushlstring(ls, (const char*)ws.c_str(), ws.size() * sizeof(wchar_t));
  return 1;
  }

static int LUA_C_ws2utf8(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const wstring ws((const wchar_t*)s, l / sizeof(wchar_t));

  const xutf8 u(ws2utf8(ws));
  lua_pushlstring(ls, (const char*)u.c_str(), u.size());
  return 1;
  }

static int LUA_C_utf82s(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const xutf8 u((p_utf8)s, l);

  const string ss(ws2s(utf82ws(u)));

  lua_pushlstring(ls, (const char*)ss.c_str(), ss.size());
  return 1;
  }

static int LUA_C_s2utf8(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const string ss(s, l);

  const xutf8 u(ws2utf8(s2ws(ss)));

  lua_pushlstring(ls, (const char*)u.c_str(), u.size());
  return 1;
  }

void register_utf8(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));


  lua_register(ls, "utf82ws", LUA_C_utf82ws);
  lua_register(ls, "ws2utf8", LUA_C_ws2utf8);
  lua_register(ls, "utf82s", LUA_C_utf82s);
  lua_register(ls, "s2utf8", LUA_C_s2utf8);


  lua_pop(ls, lua_gettop(ls));
  }