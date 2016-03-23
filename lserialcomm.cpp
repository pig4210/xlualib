#include "xlualib.h"

static int LUA_C_SerialCommGets(lua_State* ls)
  {
  HKEY hKey;
  DWORD rets = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                            TEXT("Hardware\\DeviceMap\\SerialComm"),
                            NULL, KEY_READ, &hKey);
  if(ERROR_SUCCESS != rets)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "查询串口失败 : "
                   << (intptr_t)rets << " : " << (intptr_t)GetLastError()
                   ).c_str());
    return lua_error(ls);
    }

  lua_newtable(ls);

  TCHAR port_name[MAX_PATH];
  TCHAR com_name[MAX_PATH];
  for(size_t index = 0; true; ++index)
    {
    DWORD dwLong = _countof(port_name);
    DWORD dwSize = _countof(com_name);
    rets = RegEnumValue(hKey, index, port_name, &dwLong, nullptr, nullptr,
                        (LPBYTE)com_name, &dwSize);
    if(ERROR_NO_MORE_ITEMS == rets) break;
    if(ERROR_SUCCESS != rets)
      {
      CloseHandle(hKey);
      lua_pushstring(ls,
                     (
                     xmsg() << "枚举串口失败 : "
                     << (intptr_t)rets << " : " << (intptr_t)GetLastError()
                     ).c_str());
      return lua_error(ls);
      }
    string cn(ws2s(com_name));
    lua_pushlstring(ls, cn.c_str(), cn.size());
    lua_rawseti(ls, -2, index + 1);
    }
  CloseHandle(hKey);
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static const char* gkSerialCommRegister = "SerialComm*";
static const char* gkSerialCommType = "SerialComm";

struct SerialCommST
  {
  HANDLE h;
  char   name[MAX_PATH];
  };

static SerialCommST* SerialCommCheck(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);

  if(sc->h == nullptr || sc->h == INVALID_HANDLE_VALUE)
    {
    lua_pushstring(ls, "串行通讯口:未打开/已关闭/无效，不能操作");
    lua_error(ls);
    return nullptr;
    }
  return sc;
  }

static int LUA_C_SerialCommHandle(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);
  lua_pushinteger(ls, (size_t)sc->h);
  return 1;
  }

static int LUA_C_SerialCommIsOpen(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);
  lua_pushboolean(ls, sc->h != nullptr && sc->h != INVALID_HANDLE_VALUE);
  return 1;
  }

static int LUA_C_SerialCommName(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);
  lua_pushstring(ls, sc->name);
  return 1;
  }

static int LUA_C_SerialCommType(lua_State* ls)
  {
  luaL_checkudata(ls, 1, gkSerialCommRegister);
  lua_pushstring(ls, gkSerialCommType);
  return 1;
  }

static string SerialCommToString(SerialCommST* sc)
  {
  xmsg msg;
  msg << gkSerialCommType << " [" << sc->name << "] (";
  if(sc->h == nullptr)
    {
    msg << "Closed & N/A";
    }
  else if(sc->h == INVALID_HANDLE_VALUE)
    {
    msg << "Invalid";
    }
  else
    {
    msg << sc->h;
    }
  msg << ")";
  return msg;
  }

static int LUA_C_SerialCommToString(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);
  lua_pushstring(ls, SerialCommToString(sc).c_str());
  return 1;
  }

static int LUA_C_SerialCommFree(lua_State* ls)
  {
  SerialCommST* sc = (SerialCommST*)luaL_checkudata(ls, 1, gkSerialCommRegister);

  if(sc->h != nullptr && sc->h != INVALID_HANDLE_VALUE)
    {
    CloseHandle(sc->h);
    sc->h = nullptr;
    }
  return 0;
  }

static int SerialCommError(lua_State* ls, SerialCommST* sc, const string& errmsg)
  {
  const string scs(SerialCommToString(sc));

  if(sc->h != nullptr && sc->h != INVALID_HANDLE_VALUE)
    {
    CloseHandle(sc->h);
    sc->h = nullptr;
    }

  lua_pushstring(ls, (scs + errmsg).c_str());
  return lua_error(ls);
  }

static DCB SerialCommState(lua_State* ls, SerialCommST* sc)
  {
  DCB dcb;
  memset(&dcb, 0, sizeof(dcb));
  dcb.DCBlength = sizeof(dcb);

  if(FALSE == GetCommState(sc->h, &dcb))
    {
    SerialCommError(ls, sc, xmsg() << "获取状态失败 : " << (intptr_t)GetLastError());
    }

  return dcb;
  }

static void SerialCommState(lua_State* ls, SerialCommST* sc, DCB& dcb)
  {
  if(FALSE == SetCommState(sc->h, &dcb))
    {
    SerialCommError(ls, sc, xmsg() << "设置状态失败 : " << (intptr_t)GetLastError());
    }
  }

static int LUA_C_SerialCommBaudRate(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);
  DCB dcb = SerialCommState(ls, sc);
  const auto old = dcb.BaudRate;
  if(lua_gettop(ls) >= 2)
    {
    dcb.BaudRate = luaL_checkinteger(ls, 2);
    SerialCommState(ls, sc, dcb);
    }
  lua_pushinteger(ls, old);
  return 1;
  }
static int LUA_C_SerialCommByteSize(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);
  DCB dcb = SerialCommState(ls, sc);
  const auto old = dcb.ByteSize;
  if(lua_gettop(ls) >= 2)
    {
    dcb.ByteSize = luaL_checkinteger(ls, 2);
    SerialCommState(ls, sc, dcb);
    }
  lua_pushinteger(ls, old);
  return 1;
  }
static int LUA_C_SerialCommStopBits(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);
  DCB dcb = SerialCommState(ls, sc);
  const auto old = dcb.StopBits;
  if(lua_gettop(ls) >= 2)
    {
    dcb.StopBits = luaL_checkinteger(ls, 2);
    SerialCommState(ls, sc, dcb);
    }
  lua_pushinteger(ls, old);
  return 1;
  }
static int LUA_C_SerialCommParity(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);
  DCB dcb = SerialCommState(ls, sc);
  const auto old = dcb.Parity;
  if(lua_gettop(ls) >= 2)
    {
    dcb.Parity = luaL_checkinteger(ls, 2);
    SerialCommState(ls, sc, dcb);
    }
  lua_pushinteger(ls, old);
  return 1;
  }

static int LUA_C_SerialCommWrite(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);

  size_t l = 0;
  const auto lp = luaL_checklstring(ls, 2, &l);
  DWORD size = 0;
  while(size < l)
    {
    DWORD s = 0;
    if(FALSE == WriteFile(sc->h, &lp[size], l - size, &s, nullptr))
      {
      return SerialCommError(ls, sc, xmsg() << "写入数据失败 : " << (intptr_t)GetLastError());
      }
    size += s;
    }

  DWORD   dwErrors;
  COMSTAT comStat;
  do
    {
    if(FALSE == ClearCommError(sc->h, &dwErrors, &comStat))
      {
      return SerialCommError(ls, sc, xmsg() << "等待写入缓冲失败 : " << (intptr_t)GetLastError());
      }
    } while(comStat.cbOutQue > 0);

  lua_pushinteger(ls, l);
  return 1;
  }

static int LUA_C_SerialCommRead(lua_State* ls)
  {
  auto sc = SerialCommCheck(ls);

  DWORD   dwErrors;
  COMSTAT comStat;
  if(FALSE == ClearCommError(sc->h, &dwErrors, &comStat))
    {
    return SerialCommError(ls, sc, xmsg() << "查询读取数据失败 : " << (intptr_t)GetLastError());
    }
  if(comStat.cbInQue <= 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  string buf;
  buf.resize(comStat.cbInQue);
  DWORD size = 0;
  if(FALSE == ReadFile(sc->h, (void*)buf.c_str(), comStat.cbInQue, &size, nullptr))
    {
    return SerialCommError(ls, sc, xmsg() << "读取数据失败 : " << (intptr_t)GetLastError());
    }
  lua_pushlstring(ls, buf.c_str(), size);
  return 1;
  }


static const luaL_Reg gkSerialCommLib[] =
  {
    { "handle",         LUA_C_SerialCommHandle },
    { "isopen",         LUA_C_SerialCommIsOpen },
    { "name",           LUA_C_SerialCommName },
    { "type",           LUA_C_SerialCommType },
    { "__tostring",     LUA_C_SerialCommToString },
    { "__gc",           LUA_C_SerialCommFree },
    { "close",          LUA_C_SerialCommFree },
    { "baudrate",       LUA_C_SerialCommBaudRate },
    { "bytesize",       LUA_C_SerialCommByteSize },
    { "stopbits",       LUA_C_SerialCommStopBits },
    { "parity",         LUA_C_SerialCommParity },
    { "write",          LUA_C_SerialCommWrite },
    { "read",           LUA_C_SerialCommRead },
    { nullptr, nullptr }
  };

static int LUA_C_SerialCommOpen(lua_State* ls)
  {
  const string names(luaL_checkstring(ls, 1));

  const string name = (0 != memcmp(names.c_str(), "\\\\.\\", 4)) ? ("\\\\.\\" + names) : names;

  HANDLE h = CreateFileA(name.c_str(),
                         GENERIC_READ | GENERIC_WRITE,
                         FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                         nullptr,
                         OPEN_EXISTING,
                         0,
                         nullptr);
  if(h == INVALID_HANDLE_VALUE)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "串行通讯口[" << names << "]打开失败 : "
                   << (intptr_t)GetLastError()
                   ).c_str());
    return lua_error(ls);
    }

  DCB dcb;
  memset(&dcb, 0, sizeof(dcb));
  dcb.DCBlength = sizeof(dcb);
  if(FALSE == GetCommState(h, &dcb))
    {
    CloseHandle(h);
    lua_pushstring(ls,
                   (
                   xmsg() << "串行通讯口[" << names << "]打开并获取状态失败 : "
                   << (intptr_t)GetLastError()
                   ).c_str());
    return lua_error(ls);
    }

  dcb.fBinary = TRUE;
  dcb.fOutxCtsFlow = FALSE;
  dcb.fOutxDsrFlow = FALSE;
  dcb.fDtrControl = DTR_CONTROL_DISABLE;
  dcb.fDsrSensitivity = FALSE;
  dcb.fTXContinueOnXoff = FALSE;
  dcb.fOutX = FALSE;
  dcb.fInX = FALSE;
  dcb.fErrorChar = FALSE;
  dcb.fNull = FALSE;
  dcb.fRtsControl = RTS_CONTROL_DISABLE;
  dcb.fAbortOnError = FALSE;
  if(FALSE == SetCommState(h, &dcb))
    {
    CloseHandle(h);
    lua_pushstring(ls,
                   (
                   xmsg() << "串行通讯口[" << names << "]打开并设置状态失败 : "
                   << (intptr_t)GetLastError()
                   ).c_str());
    return lua_error(ls);
    }

  if(FALSE == PurgeComm(h, PURGE_TXCLEAR | PURGE_RXCLEAR))
    {
    CloseHandle(h);
    lua_pushstring(ls,
                   (
                   xmsg() << "串行通讯口[" << names << "]打开并配置状态失败 : "
                   << (intptr_t)GetLastError()
                   ).c_str());
    return lua_error(ls);
    }

  luaL_getmetatable(ls, gkSerialCommRegister);
  if(lua_type(ls, -1) == LUA_TNIL)
    {
    lua_pop(ls, 1);
    luaL_newmetatable(ls, gkSerialCommRegister);
    lua_pushvalue(ls, -1);
    lua_setfield(ls, -2, "__index");
    luaL_setfuncs(ls, gkSerialCommLib, 0);
    lua_pop(ls, 1);
    }
  else
    {
    lua_pop(ls, 1);
    }

  SerialCommST* ch = (SerialCommST*)lua_newuserdata(ls, sizeof(*ch));
  ch->h = h;
  memcpy(ch->name, names.c_str(), names.size() + 1);
  luaL_setmetatable(ls, gkSerialCommRegister);

  return 1;
  }

void register_serialcomm(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_newtable(ls);

  lua_pushstring(ls, "gets");
  lua_pushcfunction(ls, LUA_C_SerialCommGets);
  lua_settable(ls, -3);

  lua_pushstring(ls, "raw_open");
  lua_pushcfunction(ls, LUA_C_SerialCommOpen);
  lua_settable(ls, -3);

  lua_setglobal(ls, "serialcomm");

  lua_pop(ls, lua_gettop(ls));
  }