#include <string>

#include "xlualib.h"

using std::string;
using std::wstring;

//////////////////////////////////////////////////////////////////////////
static int LUA_C_hex2bin(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  const bool errexit = lua_toboolean(ls, 2);
  const bool errbreak = lua_toboolean(ls, 3);

  const auto hs = hex2bin(s, l, nullptr, errexit, errbreak);

  lua_pushlstring(ls, (const char*)hs.c_str(), hs.size());
  return 1;
  }

static int LUA_C_bin2hex(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const bool isup = lua_toboolean(ls, 2);

  lua_pushstring(ls, bin2hex(s, l, isup).c_str());
  return 1;
  }

static int LUA_C_showbin(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const size_t flag = luaL_optinteger(ls, 2, 1);

  const size_t prews = flag / 0x10;

  Hex2showCode code = HC_ASCII;

  if((flag & 1) == 1)
    {
    code = HC_ASCII;
    }
  else if((flag & 2) == 2)
    {
    code = HC_UNICODE;
    }
  else if((flag & 8) == 8)
    {
    code = HC_UTF8;
    }

  bool isup = true;

  if((flag & 4) == 4)
    {
    isup = false;
    }

  lua_pushstring(ls, showbin(s, l, code, isup, prews).c_str());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_ws2s(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto ss(ws2s((const charucs2_t*)s, l / sizeof(charucs2_t)));

  lua_pushlstring(ls, ss.c_str(), ss.size());
  return 1;
  }

static int LUA_C_s2ws(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto ws(s2ws(s, l));

  lua_pushlstring(ls, (const char*)ws.c_str(), ws.size() * sizeof(charucs2_t));
  return 1;
  }

//////////////////////////////////////////////////////////////////////////
static int LUA_C_utf82ws(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto ws(utf82ws((pc_utf8)s, l));
  lua_pushlstring(ls, (const char*)ws.c_str(), ws.size() * sizeof(charucs2_t));
  return 1;
  }

static int LUA_C_ws2utf8(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto u(ws2utf8((const charucs2_t*)s, l / sizeof(charucs2_t)));
  lua_pushlstring(ls, (const char*)u.c_str(), u.size());
  return 1;
  }

static int LUA_C_utf82s(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto ss(ws2s(utf82ws((pc_utf8)s, l)));

  lua_pushlstring(ls, (const char*)ss.c_str(), ss.size());
  return 1;
  }

static int LUA_C_s2utf8(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto u(ws2utf8(s2ws(s, l)));

  lua_pushlstring(ls, (const char*)u.c_str(), u.size());
  return 1;
  }

void register_hex_str(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "hex2bin", LUA_C_hex2bin);
  lua_register(ls, "showbin", LUA_C_showbin);
  lua_register(ls, "bin2hex", LUA_C_bin2hex);

  lua_register(ls, "ws2s", LUA_C_ws2s);
  lua_register(ls, "s2ws", LUA_C_s2ws);

  lua_register(ls, "utf82ws", LUA_C_utf82ws);
  lua_register(ls, "ws2utf8", LUA_C_ws2utf8);
  lua_register(ls, "utf82s", LUA_C_utf82s);
  lua_register(ls, "s2utf8", LUA_C_s2utf8);

  lua_pop(ls, lua_gettop(ls));
  }