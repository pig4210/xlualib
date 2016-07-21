#include "xlualib.h"

static int LUA_C_Sleep(lua_State* ls)
  {
  const size_t ms = (size_t)luaL_optinteger(ls, 1, 0);
  Sleep(ms);
  return 0;
  }

static int LUA_C_GetTickCount(lua_State* ls)
  {
  lua_pushinteger(ls, GetTickCount());
  return 1;
  }

static int LUA_C_GetModuleHandle(lua_State* ls)
  {
  LPCSTR name = (LPCSTR)luaL_optstring(ls, 1, nullptr);
  HMODULE mod = GetModuleHandleA(name);
  lua_pushinteger(ls, (size_t)mod);
  return 1;
  }

static int LUA_C_LoadLibrary(lua_State* ls)
  {
  auto name = luaL_checkstring(ls, 1);
  HMODULE mod = LoadLibraryA(name);
  lua_pushinteger(ls, (size_t)mod);
  if(mod != nullptr)  return 1;
  lua_pushinteger(ls, (size_t)GetLastError());
  return 2;
  }

static int LUA_C_FreeLibrary(lua_State* ls)
  {
  HMODULE mod = (HMODULE)luaL_checkinteger(ls, 1);
  auto b = FreeLibrary(mod);
  if(b)
    {
    lua_pushboolean(ls, true);
    return 1;
    }
  lua_pushboolean(ls, false);
  lua_pushinteger(ls, (size_t)GetLastError());
  return 2;
  }

static int LUA_C_GetProcAddress(lua_State* ls)
  {
  HMODULE mod = (HMODULE)luaL_checkinteger(ls, 1);
  auto name = luaL_checkstring(ls, 2);
  size_t func = (size_t)GetProcAddress(mod, name);
  lua_pushinteger(ls, (size_t)func);
  return 1;
  }


void register_winapi(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "sleep", LUA_C_Sleep);
  lua_register(ls, "gettick", LUA_C_GetTickCount);

  lua_register(ls, "Sleep", LUA_C_Sleep);
  lua_register(ls, "GetTickCount", LUA_C_GetTickCount);
  lua_register(ls, "GetModuleHandle", LUA_C_GetModuleHandle);
  lua_register(ls, "LoadLibrary", LUA_C_LoadLibrary);
  lua_register(ls, "FreeLibrary", LUA_C_FreeLibrary);
  lua_register(ls, "GetProcAddress", LUA_C_GetProcAddress);

  lua_pop(ls, lua_gettop(ls));
  }