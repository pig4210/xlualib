#include "xlualib.h"

static int LUA_C_str2hexs(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  const bool errexit = lua_toboolean(ls, 2);
  const bool errbreak = lua_toboolean(ls, 3);

  const hexstring hs = str2hexs(string(s, l), nullptr, errexit, errbreak);

  lua_pushlstring(ls, (const char*)hs.c_str(), hs.size());
  return 1;
  }

static int LUA_C_hex2show(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const size_t prews = lua_tointeger(ls, 2);

  Hex2showCode code = HC_ASCII;
  const int argc = lua_gettop(ls);
  if(argc >= 3 && lua_type(ls, 3) != LUA_TNIL)
    {
    const unsigned short* a = (const unsigned short*)luaL_checkstring(ls, 3);
    switch(*a)
      {
      case '\0u': case '\0U': code = HC_UNICODE;  break;
      case 0x38: code = HC_UTF8;  break;
      case '\0a': case '\0A':
      default: code = HC_ASCII;    break;
      }
    }

  bool isup = true;
  if(argc >= 4 && lua_type(ls, 4) != LUA_TNIL)
    {
    isup = lua_toboolean(ls, 4);
    }

  lua_pushstring(ls, hex2show(s, l, prews, code, isup).c_str());
  return 1;
  }

static int LUA_C_hex2str(lua_State* ls)
  {
  const int argc = lua_gettop(ls);
  if(argc < 1)  return 0;

  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const bool isup = lua_toboolean(ls, 2);

  lua_pushstring(ls, hex2str(string(s, l), isup).c_str());
  return 1;
  }

void register_hex_str(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));


  lua_register(ls, "str2hexs", LUA_C_str2hexs);
  lua_register(ls, "hex2show", LUA_C_hex2show);
  lua_register(ls, "hex2str", LUA_C_hex2str);


  lua_pop(ls, lua_gettop(ls));
  }