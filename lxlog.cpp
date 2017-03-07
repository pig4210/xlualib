#include "xlualib.h"

static int LUA_C_xlog(lua_State* ls)
  {
  xmsg msg;

  const int argc = lua_gettop(ls);
  if(argc < 1)  return 0;

  lua_getglobal(ls, "tostring");
  for(int i = 1; i <= argc; ++i)
    {
    lua_pushvalue(ls, -1);
    lua_pushvalue(ls, i);
    lua_call(ls, 1, 1);
    if(i > 1) msg << "    ";
    size_t l = 0;
    const auto s = luaL_checklstring(ls, -1, &l);
    msg.append(s, l);
    lua_pop(ls, 1);
    }

  if(msg.empty()) return 0;

  xlog() << msg;
  return 0;
  }

ADD_XLUALIB_REGISTER(xlog)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_getglobal(ls, "xlog");
  if(lua_type(ls, -1) == LUA_TNIL)
    {
    lua_register(ls, "xlog", LUA_C_xlog);
    }

  lua_register(ls, "dbgview", LUA_C_xlog);

  lua_pop(ls, lua_gettop(ls));
  }