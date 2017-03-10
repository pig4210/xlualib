#include "xlualib.h"

static int LUA_C_PE(lua_State* ls)
  {
  const IMAGE_DOS_HEADER* dos_head = nullptr;
  bool ispe = false;
  const IMAGE_NT_HEADERS* pe_head = nullptr;
  void* entry = nullptr;
  xblk image;
  xblk code;
  HMODULE hmod = nullptr;
  if(lua_gettop(ls) == 0 ||
     lua_type(ls, 1) == LUA_TNUMBER)
    {
    hmod = (HMODULE)luaL_optinteger(ls, 1, 0);
    if(hmod == nullptr)
      {
      hmod = GetModuleHandle(nullptr);
      }
    }
  else
    {
    auto name = luaL_checkstring(ls, 1);
    hmod = GetModuleHandleA(name);
    }
  try
    {
    if(hmod != nullptr)
      {
      pe pp(hmod);
      dos_head = pp.GetDosHead();
      ispe = pp.IsPE();
      pe_head = pp.GetPeHead();
      entry = pp.EntryPoint();
      image = pp.GetImage();
      code = pp.GetCode();
      }
    }
  catch(...)
    {
    }
  lua_newtable(ls);

  lua_pushstring(ls, "hmod");
  lua_pushinteger(ls, (size_t)hmod);
  lua_settable(ls, -3);

  lua_pushstring(ls, "dos_head");
  lua_pushinteger(ls, (size_t)dos_head);
  lua_settable(ls, -3);

  lua_pushstring(ls, "ispe");
  lua_pushboolean(ls, ispe);
  lua_settable(ls, -3);

  lua_pushstring(ls, "pe_head");
  lua_pushinteger(ls, (size_t)pe_head);
  lua_settable(ls, -3);

  lua_pushstring(ls, "entry");
  lua_pushinteger(ls, (size_t)entry);
  lua_settable(ls, -3);

  lua_pushstring(ls, "image");
  lua_newtable(ls);
  lua_pushstring(ls, "start");
  lua_pushinteger(ls, (size_t)image.start());
  lua_settable(ls, -3);
  lua_pushstring(ls, "end");
  lua_pushinteger(ls, (size_t)image.end());
  lua_settable(ls, -3);
  lua_pushstring(ls, "size");
  lua_pushinteger(ls, (size_t)image.size());
  lua_settable(ls, -3);
  lua_settable(ls, -3);

  lua_pushstring(ls, "code");
  lua_newtable(ls);
  lua_pushstring(ls, "start");
  lua_pushinteger(ls, (size_t)code.start());
  lua_settable(ls, -3);
  lua_pushstring(ls, "end");
  lua_pushinteger(ls, (size_t)code.end());
  lua_settable(ls, -3);
  lua_pushstring(ls, "size");
  lua_pushinteger(ls, (size_t)code.size());
  lua_settable(ls, -3);
  lua_settable(ls, -3);

  return 1;
  }

void register_pe(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "PE", LUA_C_PE);

  lua_pop(ls, lua_gettop(ls));
  }