#include "xlualib.h"

static int LUA_C_readmem(lua_State* ls)
  {
  const char* lp = (const char*)luaL_checkinteger(ls, 1);
  const intptr_t len = luaL_checkinteger(ls, 2);
  if(lp == nullptr || len <= 0)
    {
    lua_pushlstring(ls, "", 0);
    lua_pushstring(ls, "error arg");
    return 2;
    }
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
  lua_pushstring(ls, "exception");
  return 2;
  }

static int LUA_C_writemem(lua_State* ls)
  {
  void* lp = (void*)luaL_checkinteger(ls, 1);
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 2, &l);
  if(lp == nullptr || l <= 0)
    {
    lua_pushboolean(ls, false);
    lua_pushstring(ls, "error arg");
    return 2;
    }
  try
    {
    const bool b = Hookit(lp, s, l);
    if(!b)
      {
      lua_pushboolean(ls, false);
      xmsg msg;
      msg << "error msg:" << (intptr_t)GetLastHookErr();
      lua_pushstring(ls, msg.c_str());
      return 2;
      }
    lua_pushboolean(ls, true);
    return 1;
    }
  catch(...)
    {
    ;
    }
  lua_pushboolean(ls, false);
  lua_pushstring(ls, "exception");
  return 2;
  }

static int LUA_C_newmem(lua_State* ls)
  {
  size_t size = luaL_checkinteger(ls, 1);
  if(size == 0)
    {
    lua_pushinteger(ls, 0);
    return 1;
    }
  try
    {
    unsigned char* lp = new unsigned char[size];
    lua_pushinteger(ls, (size_t)lp);
    return 1;
    }
  catch(...)
    {
    ;
    }
  lua_pushinteger(ls, 0);
  return 1;
  }

static int LUA_C_deletemem(lua_State* ls)
  {
  unsigned char* lp = (unsigned char*)luaL_checkinteger(ls, 1);
  if(lp == nullptr)
    {
    lua_pushboolean(ls, false);
    return 1;
    }
  try
    {
    delete[] lp;
    lua_pushboolean(ls, true);
    return 1;
    }
  catch(...)
    {
    ;
    }
  lua_pushboolean(ls, false);
  return 1;
  }

void register_mkmem(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "readmem", LUA_C_readmem);
  lua_register(ls, "writemem", LUA_C_writemem);
  lua_register(ls, "newmem", LUA_C_newmem);
  lua_register(ls, "deletemem", LUA_C_deletemem);

  lua_pop(ls, lua_gettop(ls));
  }