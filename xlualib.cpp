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


#include <map>

using std::map;

//! 做成函数即保证初始化，也保证内部访问
static map<string, xlualib_register_routine>& routines()
  {
  static map<string, xlualib_register_routine> xlualib_register_routines;
  return xlualib_register_routines;
  }

bool Add_XLUALIB_REGISTER_ROUTINE(const char* const name, xlualib_register_routine func)
  {
  routines()[name] = func;
  return name != nullptr;
  }

extern "C"
#ifndef XLUALIB_INSIDE
__declspec(dllexport)
#endif
int luaopen_xlualib(lua_State* ls)
  {
  for(const auto& r : routines())
    {
    r.second(ls);
    }

  load_my_lua(ls);
  return 0;
  }