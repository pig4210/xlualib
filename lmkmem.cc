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

static int LUA_C_hook(lua_State* ls)
  {
  if(lua_gettop(ls) == 0)
    {
    if(!HookClear())
      {
      lua_pushstring(ls, (xmsg() << "UnHook失败 : " << (intptr_t)GetLastHookErr()).c_str());
      return lua_error(ls);
      }
    return 0;
    }

  if(lua_gettop(ls) == 1)
    {
    HookNode* node = (HookNode*)luaL_checkinteger(ls, 1);
    if(!UnHook(node, false))
      {
      lua_pushstring(ls, (xmsg() << "UnHook失败 : " << (intptr_t)GetLastHookErr()).c_str());
      return lua_error(ls);
      }
    return 0;
    }

  void* hookmem = (void*)luaL_checkinteger(ls, 1);
  HookNode* node = nullptr;

  if(lua_isnumber(ls, 2))
    {
    const size_t hooksize = luaL_checkinteger(ls, 2);
    const char* data_descibe = luaL_checkstring(ls, 3);
    const char* len_descibe = luaL_checkstring(ls, 4);
    const bool docodeend = lua_toboolean(ls, 5);
    node = Hook2Log(hookmem, hooksize, data_descibe, len_descibe, docodeend);
    }
  else
    {
    const char* data_descibe = luaL_checkstring(ls, 2);
    const char* len_descibe = luaL_checkstring(ls, 3);
    const bool calltable_offset = lua_toboolean(ls, 4);
    const bool docallend = lua_toboolean(ls, 5);
    node = Hook2Log(hookmem, data_descibe, len_descibe, calltable_offset, docallend);
    }
  if(node == nullptr)
    {
    lua_pushstring(ls, (xmsg() << "Hook失败 : " << (intptr_t)GetLastHookErr()).c_str());
    return lua_error(ls);
    }
  lua_pushinteger(ls, (size_t)node);
  return 1;
  }

void register_mkmem(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "readmem", LUA_C_readmem);
  lua_register(ls, "writemem", LUA_C_writemem);
  lua_register(ls, "newmem", LUA_C_newmem);
  lua_register(ls, "deletemem", LUA_C_deletemem);

  lua_register(ls, "hook", LUA_C_hook);

  lua_pop(ls, lua_gettop(ls));
  }