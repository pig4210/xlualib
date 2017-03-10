#include <string>

#include "xlualib.h"

using std::string;

static void load_my_lua(lua_State* ls)
  {
  HMODULE hmod = nullptr;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)load_my_lua, &hmod);
  char name[MAX_PATH];
  GetModuleFileNameA(hmod, name, sizeof(name));

  HRSRC hrsrc = FindResource(hmod, MAKEINTRESOURCE(101), TEXT("BIN"));
  if(hrsrc == nullptr)
    {
    //xerr << "FindResource" << (intptr_t)GetLastError();
    return;
    }
  HGLOBAL hglobal = LoadResource(hmod, hrsrc);
  if(hrsrc == nullptr)
    {
    //xerr << "LoadResource" << (intptr_t)GetLastError();
    return;
    }
  void* src = LockResource(hglobal);
  if(src == nullptr)
    {
    //xerr << "LockResource" << (intptr_t)GetLastError();
    return;
    }
  size_t len = SizeofResource(hmod, hrsrc);
  if(len == 0)
    {
    //xerr << "SizeofResource" << (intptr_t)GetLastError();
    return;
    }

  lua_pop(ls, lua_gettop(ls));

  const string tmp((const char*)src, len);//设计用于避免资源不可写

  if(LUA_OK != luaL_loadbufferx(ls, tmp.c_str(), tmp.size(), name, nullptr) ||
     LUA_OK != lua_pcall(ls, 0, 0, 0))
    {
    //xerr << "do my lua" << lua_tostring(ls, -1);
    }

  lua_pop(ls, lua_gettop(ls));
  }

//本来使用全局静态map机制自动注册，但引起了诡异情况，遂恢复

extern void register_aes(lua_State* ls);
extern void register_algorithm(lua_State* ls);
extern void register_blowfish(lua_State* ls);
extern void register_des(lua_State* ls);
extern void register_hex_str(lua_State* ls);
extern void register_mkmem(lua_State* ls);
extern void register_openssl(lua_State* ls);
extern void register_pe(lua_State* ls);
extern void register_serialcomm(lua_State* ls);
extern void register_sock(lua_State* ls);
extern void register_winapi(lua_State* ls);
extern void register_xhttp(lua_State* ls);
extern void register_xlog(lua_State* ls);
extern void register_zlib(lua_State* ls);

extern "C"
#ifndef XLUALIB_INSIDE
__declspec(dllexport)
#endif
int luaopen_xlualib(lua_State* ls)
  {
  register_aes(ls);
  register_algorithm(ls);
  register_blowfish(ls);
  register_des(ls);
  register_hex_str(ls);
  register_mkmem(ls);
  register_openssl(ls);
  register_pe(ls);
  register_serialcomm(ls);
  register_sock(ls);
  register_winapi(ls);
  register_xhttp(ls);
  register_xlog(ls);
  register_zlib(ls);

  load_my_lua(ls);
  return 0;
  }