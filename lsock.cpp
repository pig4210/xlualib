#include <string>
#include <stdexcept>

#include "xlualib.h"

using std::string;
using std::runtime_error;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
struct SOCKET_ST
  {
  SOCKET sock;
  sockaddr_in addr;
  USHORT bind_port;
  };

//////////////////////////////////////////////////////////////////////////
static const char* gk_udp_register = "UDP*";
static const char* gk_udp_type = "UDP";

static int LUA_C_udp_close(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  if(st->sock != INVALID_SOCKET)
    {
    shutdown(st->sock, SD_BOTH);
    closesocket(st->sock);
    st->sock = INVALID_SOCKET;
    }
  return 0;
  }

static int UDP_ERROR(lua_State* ls, SOCKET_ST* st, const string& errmsg)
  {
  if(st->sock != INVALID_SOCKET)
    {
    shutdown(st->sock, SD_BOTH);
    closesocket(st->sock);
    st->sock = INVALID_SOCKET;
    }
  lua_pushstring(ls, errmsg.c_str());
  return lua_error(ls);
  }

static int LUA_C_udp_type(lua_State* ls)
  {
  luaL_checkudata(ls, 1, gk_udp_register);
  lua_pushstring(ls, gk_udp_type);
  return 1;
  }

static int LUA_C_udp_getsockname(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  int addr_len = sizeof(addr);
  getsockname(st->sock, (sockaddr*)&addr, &addr_len);

  const auto port = (st->bind_port != 0) ? htons(st->bind_port) : addr.sin_port;

//   if(addr.sin_addr.S_un.S_addr == 0)
//     {
//     char name[100];
//     if(0 == gethostname(name, sizeof(name)))
//       {
//       hostent* p = gethostbyname(name);
//       if(p != nullptr)
//         {
//         memcpy(&addr.sin_addr.S_un.S_addr, p->h_addr_list[0], sizeof(addr.sin_addr.S_un.S_addr));
//         }
//       }
//     }

  xmsg msg;
  msg
    << (int)addr.sin_addr.s_net << '.'
    << (int)addr.sin_addr.s_host << '.'
    << (int)addr.sin_addr.s_lh << '.'
    << (int)addr.sin_addr.s_impno;
  lua_pushstring(ls, msg.c_str());

  msg.clear();
  msg << (int)htons(port);
  lua_pushstring(ls, msg.c_str());

  lua_pushinteger(ls, htonl(addr.sin_addr.S_un.S_addr));
  lua_pushinteger(ls, htons(port));
  return 4;
  }

static int LUA_C_udp_getpeername(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);

  auto& addr = st->addr;

  xmsg msg;
  msg
    << (int)addr.sin_addr.s_net << '.'
    << (int)addr.sin_addr.s_host << '.'
    << (int)addr.sin_addr.s_lh << '.'
    << (int)addr.sin_addr.s_impno;
  lua_pushstring(ls, msg.c_str());

  msg.clear();
  msg << (int)htons(addr.sin_port);
  lua_pushstring(ls, msg.c_str());

  lua_pushinteger(ls, htonl(addr.sin_addr.S_un.S_addr));
  lua_pushinteger(ls, htons(addr.sin_port));
  return 4;
  }

static int LUA_C_udp_tostring(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  xmsg msg;
  msg << gk_udp_register;
  if(st->bind_port != 0)
    {
    msg << "{server}";
    }
  else
    {
    msg << "{client}";
    }
  msg << ":" << (void*)st->sock;

  msg << "    ";

  LUA_C_udp_getsockname(ls);

  auto ip = lua_tostring(ls, -4);
  auto port = lua_tostring(ls, -3);

  msg << ip << ':' << port << " >> " << IpString(st->addr);

  lua_pushstring(ls, msg.c_str());
  return 1;
  }

static int LUA_C_udp_settimeout(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  const int timeout = luaL_checkinteger(ls, 2);

  if(setsockopt(st->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)))
    {
    return UDP_ERROR(ls, st, "设置UDP超时出错");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_udp_broadcast(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  const BOOL b = lua_toboolean(ls, 2);

  if(setsockopt(st->sock, SOL_SOCKET, SO_BROADCAST, (char*)&b, sizeof(b)))
    {
    return UDP_ERROR(ls, st, "设置UDP广播出错");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_udp_send(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  size_t len = 0;
  const char* buf = luaL_checklstring(ls, 2, &len);

  if(lua_type(ls, 3) == LUA_TNUMBER)
    {
    sockaddr_in addr;
    addr.sin_addr.s_addr = lua_tointeger(ls, 3);
    xmsg msg;
    msg
      << (int)addr.sin_addr.s_net << '.'
      << (int)addr.sin_addr.s_host << '.'
      << (int)addr.sin_addr.s_lh << '.'
      << (int)addr.sin_addr.s_impno;
    lua_pushstring(ls, msg.c_str());
    lua_replace(ls, 3);
    }

  auto ip = luaL_optstring(ls, 3, "0.0.0.0");
  auto port = luaL_optstring(ls, 4, "0");

  try
    {
    auto addrto = AddrInfo(ip, port);

    if(addrto.sin_addr.S_un.S_addr == 0 && addrto.sin_port == 0)
      {
      addrto = st->addr;
      }

    while(len > 0)
      {
      int sendlen = ::sendto(st->sock, buf, len, 0,
        (const sockaddr *)&addrto, sizeof(addrto));

      if(sendlen <= 0)
        {
        return UDP_ERROR(ls, st, xmsg() << "UDP发送失败:" << WSAGetLastError());
        }
      len -= sendlen;
      buf += sendlen;
      }
    }
  catch(const runtime_error& err)
    {
    return UDP_ERROR(ls, st, xmsg() << "UDP发送错误:" << err.what());
    }
  catch(...)
    {
    return UDP_ERROR(ls, st, "UDP发送异常");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_udp_recv(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_udp_register);
  char buf[0x800];
  const int len = luaL_optinteger(ls, 2, sizeof(buf));

  char* lp = buf;
  if(len > sizeof(buf))
    {
    lp = new char[len];
    }

  sockaddr_in addr;
  int addrlen = sizeof(addr);
  memset(&addr, 0, sizeof(addr));

  int recvlen = ::recvfrom(st->sock, lp, len, 0,
                       (sockaddr*)&addr, &addrlen);

  if(recvlen == 0)
    {
    if(lp != buf) delete[] lp;
    return UDP_ERROR(ls, st, "UDP接收失败，SOCKET已关闭");
    }
  if(recvlen == SOCKET_ERROR)
    {
    const int codes = WSAGetLastError();
    switch(codes)
      {
      case WSAETIMEDOUT:
        if(lp != buf) delete[] lp;
        lua_pushnil(ls);
        lua_pushstring(ls, "timeout");
        return 2;
      case WSAEMSGSIZE:
        if(lp != buf) delete[] lp;
        lua_pushnil(ls);
        lua_pushstring(ls, "msgsize");
        return 2;
      default:
        {
        if(lp != buf) delete[] lp;
        return UDP_ERROR(ls, st, xmsg() << "UDP接收失败:" << codes);
        }
      }
    }
  lua_pushlstring(ls, lp, recvlen);
  if(lp != buf) delete[] lp;

  xmsg msg;
  msg
    << (int)addr.sin_addr.s_net << '.'
    << (int)addr.sin_addr.s_host << '.'
    << (int)addr.sin_addr.s_lh << '.'
    << (int)addr.sin_addr.s_impno;
  lua_pushstring(ls, msg.c_str());

  msg.clear();
  msg << (int)htons(addr.sin_port);
  lua_pushstring(ls, msg.c_str());

  lua_pushinteger(ls, htonl(addr.sin_addr.S_un.S_addr));
  lua_pushinteger(ls, htons(addr.sin_port));

  return 5;
  }


static const luaL_Reg gkUDPLib[] =
  {
    { "__gc",           LUA_C_udp_close },
    { "__tostring",     LUA_C_udp_tostring },
    { "close",          LUA_C_udp_close },
    { "getsockname",    LUA_C_udp_getsockname },
    { "getpeername",    LUA_C_udp_getpeername },
    { "type",           LUA_C_udp_type },
    { "settimeout",     LUA_C_udp_settimeout },
    { "broadcast",      LUA_C_udp_broadcast },
    { "send",           LUA_C_udp_send },
    { "recv",           LUA_C_udp_recv },
    { nullptr, nullptr }
  };

static int LUA_C_udp_new(lua_State* ls)
  {
  xWSA wsa;
  SOCKET sock = socket(PF_INET, SOCK_DGRAM, 0);
  if(sock == INVALID_SOCKET)
    {
    xmsg msg;
    msg << "建立udp socket失败:" << WSAGetLastError();
    lua_pushstring(ls, msg.c_str());
    return lua_error(ls);
    }
  if(lua_type(ls, 1) == LUA_TNUMBER)
    {
    sockaddr_in addr;
    addr.sin_addr.s_addr = lua_tointeger(ls, 1);
    xmsg msg;
    msg
      << (int)addr.sin_addr.s_net << '.'
      << (int)addr.sin_addr.s_host << '.'
      << (int)addr.sin_addr.s_lh << '.'
      << (int)addr.sin_addr.s_impno;
    lua_pushstring(ls, msg.c_str());
    lua_replace(ls, 1);
    }
  auto ip = luaL_optstring(ls, 1, "0.0.0.0");
  auto port = luaL_optstring(ls, 2, "0");

  auto bind_port = luaL_optinteger(ls, 3, 0);

  try
    {
    auto addr = AddrInfo(ip, port);
    bind_port = (bind_port != 0) ? bind_port :
      ((addr.sin_addr.S_un.S_addr == INADDR_ANY) ? addr.sin_port : 0);

    if(bind_port != 0)
      {
      auto s = AddrInfo(INADDR_ANY, bind_port);
      if(bind(sock, (sockaddr*)&s, sizeof(s)))
        {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        xmsg msg;
        msg << "udp socket绑定[" << (intptr_t)htons(bind_port)
          << "]失败:" << (intptr_t)WSAGetLastError();
        lua_pushstring(ls, msg.c_str());
        return lua_error(ls);
        }
      }

    luaL_getmetatable(ls, gk_udp_register);
    if(lua_type(ls, -1) == LUA_TNIL)
      {
      lua_pop(ls, 1);
      luaL_newmetatable(ls, gk_udp_register);
      lua_pushvalue(ls, -1);
      lua_setfield(ls, -2, "__index");
      luaL_setfuncs(ls, gkUDPLib, 0);
      lua_pop(ls, 1);
      }
    else
      {
      lua_pop(ls, 1);
      }

    SOCKET_ST* st = (SOCKET_ST*)lua_newuserdata(ls, sizeof(*st));
    st->sock = sock;
    st->addr = addr;
    st->bind_port = bind_port;

    luaL_setmetatable(ls, gk_udp_register);
    return 1;
    }
  catch(const runtime_error& err)
    {
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    xmsg msg;
    msg << "建立UDP错误:" << err.what();
    lua_pushstring(ls, msg.c_str());
    return lua_error(ls);
    }
  catch(...)
    {
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    lua_pushstring(ls, "建立UDP异常");
    return lua_error(ls);
    }
  }
//////////////////////////////////////////////////////////////////////////
static const char* gk_tcp_register = "TCP*";
static const char* gk_tcp_type = "TCP";

static int LUA_C_tcp_close(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);
  if(st->sock != INVALID_SOCKET)
    {
    shutdown(st->sock, SD_BOTH);
    closesocket(st->sock);
    st->sock = INVALID_SOCKET;
    }
  return 0;
  }

static int TCP_ERROR(lua_State* ls, SOCKET_ST* st, const string& errmsg)
  {
  if(st->sock != INVALID_SOCKET)
    {
    shutdown(st->sock, SD_BOTH);
    closesocket(st->sock);
    st->sock = INVALID_SOCKET;
    }
  lua_pushstring(ls, errmsg.c_str());
  return lua_error(ls);
  }

static int LUA_C_tcp_type(lua_State* ls)
  {
  luaL_checkudata(ls, 1, gk_tcp_register);
  lua_pushstring(ls, gk_tcp_type);
  return 1;
  }

static int LUA_C_tcp_getsockname(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  int addr_len = sizeof(addr);
  getsockname(st->sock, (sockaddr*)&addr, &addr_len);

  const auto port = (st->bind_port != 0) ? htons(st->bind_port) : addr.sin_port;

  xmsg msg;
  msg
    << (int)addr.sin_addr.s_net << '.'
    << (int)addr.sin_addr.s_host << '.'
    << (int)addr.sin_addr.s_lh << '.'
    << (int)addr.sin_addr.s_impno;
  lua_pushstring(ls, msg.c_str());

  msg.clear();
  msg << (int)htons(port);
  lua_pushstring(ls, msg.c_str());

  lua_pushinteger(ls, htonl(addr.sin_addr.S_un.S_addr));
  lua_pushinteger(ls, htons(port));
  return 4;
  }

static int LUA_C_tcp_getpeername(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);

  auto& addr = st->addr;

  xmsg msg;
  msg
    << (int)addr.sin_addr.s_net << '.'
    << (int)addr.sin_addr.s_host << '.'
    << (int)addr.sin_addr.s_lh << '.'
    << (int)addr.sin_addr.s_impno;
  lua_pushstring(ls, msg.c_str());

  msg.clear();
  msg << (int)htons(addr.sin_port);
  lua_pushstring(ls, msg.c_str());

  lua_pushinteger(ls, htonl(addr.sin_addr.S_un.S_addr));
  lua_pushinteger(ls, htons(addr.sin_port));
  return 4;
  }

static int LUA_C_tcp_tostring(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);
  xmsg msg;
  msg << gk_tcp_register;
  if(st->bind_port != 0)
    {
    msg << "{server}";
    }
  else
    {
    msg << "{client}";
    }
  msg << ":" << (void*)st->sock;

  msg << "    ";

  LUA_C_tcp_getsockname(ls);

  auto ip = lua_tostring(ls, -4);
  auto port = lua_tostring(ls, -3);

  msg << ip << ':' << port << " >> " << IpString(st->addr);

  lua_pushstring(ls, msg.c_str());
  return 1;
  }

static int LUA_C_tcp_settimeout(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);
  if(st->bind_port != 0)
    return TCP_ERROR(ls, st, "TCP Server不支持此功能");

  const int timeout = luaL_checkinteger(ls, 2);

  if(setsockopt(st->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)))
    {
    return TCP_ERROR(ls, st, "设置TCP超时出错");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_tcp_broadcast(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);
  if(st->bind_port != 0)
    return TCP_ERROR(ls, st, "TCP Server无法支持广播");

  const BOOL b = lua_toboolean(ls, 2);

  if(setsockopt(st->sock, SOL_SOCKET, SO_BROADCAST, (char*)&b, sizeof(b)))
    {
    return TCP_ERROR(ls, st, "设置TCP广播出错");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_tcp_send(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);

  if(st->bind_port != 0)
    return TCP_ERROR(ls, st, "TCP Server无法发送数据");

  size_t len = 0;
  const char* buf = luaL_checklstring(ls, 2, &len);

  try
    {
    while(len > 0)
      {
      int sendlen = ::send(st->sock, buf, len, 0);

      if(sendlen <= 0)
        {
        return TCP_ERROR(ls, st, xmsg() << "TCP发送失败:" << WSAGetLastError());
        }
      len -= sendlen;
      buf += sendlen;
      }
    }
  catch(const runtime_error& err)
    {
    return TCP_ERROR(ls, st, xmsg() << "TCP发送错误:" << err.what());
    }
  catch(...)
    {
    return TCP_ERROR(ls, st, "TCP发送异常");
    }
  lua_settop(ls, 1);
  return 1;
  }

static int LUA_C_tcp_recv(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);
  if(st->bind_port != 0)
    return TCP_ERROR(ls, st, "TCP Server无法接收数据");

  char buf[0x800];
  const int len = luaL_optinteger(ls, 2, sizeof(buf));

  char* lp = buf;
  if(len > sizeof(buf))
    {
    lp = new char[len];
    }

  int recvlen = ::recv(st->sock, lp, len, 0);

  if(recvlen == 0)
    {
    if(lp != buf) delete[] lp;
    return TCP_ERROR(ls, st, "TCP接收失败，SOCKET已关闭");
    }
  if(recvlen == SOCKET_ERROR)
    {
    const int codes = WSAGetLastError();
    switch(codes)
      {
      case WSAETIMEDOUT:
        if(lp != buf) delete[] lp;
        lua_pushnil(ls);
        lua_pushstring(ls, "timeout");
        return 2;
      default:
        {
        if(lp != buf) delete[] lp;
        return TCP_ERROR(ls, st, xmsg() << "TCP接收失败:" << codes);
        }
      }
    }
  lua_pushlstring(ls, lp, recvlen);
  if(lp != buf) delete[] lp;

  return 1;
  }

static int LUA_C_tcp_accept(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);

  if(st->bind_port == 0)
    return TCP_ERROR(ls, st, "TCP Client不能接受连接");

  const size_t timeout = luaL_optinteger(ls, 2, -1);

  timeval t;
  t.tv_sec = timeout / 1000;
  t.tv_usec = timeout % 1000;

  const timeval* lpt = (timeout == -1) ? nullptr : &t;

  fd_set s;
  s.fd_count = 1;
  s.fd_array[0] = st->sock;

  switch(select(1, &s, nullptr, nullptr, lpt))
    {
    case SOCKET_ERROR:
      return TCP_ERROR(ls, st, "TCP监听连接失败");
    case 0:
      lua_pushnil(ls);
      lua_pushstring(ls, "timeout");
      return 2;
    default:
      break;
    }

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  int namelen = sizeof(addr);
  const SOCKET hS = accept(st->sock, (sockaddr*)&addr, &namelen);
  if(hS == INVALID_SOCKET)
    return TCP_ERROR(ls, st, xmsg() << "TCP接受连接失败:" << WSAGetLastError());

  st = (SOCKET_ST*)lua_newuserdata(ls, sizeof(*st));
  st->sock = hS;
  st->addr = addr;
  st->bind_port = 0;

  luaL_setmetatable(ls, gk_tcp_register);
  return 1;
  }

static int LUA_C_tcp_check(lua_State* ls)
  {
  SOCKET_ST* st = (SOCKET_ST*)luaL_checkudata(ls, 1, gk_tcp_register);

  timeval t;
  t.tv_sec = 0;
  t.tv_usec = 1;

  fd_set fd;
  FD_ZERO(&fd);
  FD_SET(st->sock, &fd);
  if(0 < select(0, nullptr, &fd, nullptr, &t))
    {
    u_long ul = 0;
    ioctlsocket(st->sock, FIONBIO, &ul);
    lua_pushboolean(ls, true);
    return 1;
    }
  lua_pushboolean(ls, false);
  return 1;
  }

static const luaL_Reg gkTCPLib[] =
  {
    { "__gc",         LUA_C_tcp_close },
    { "__tostring",   LUA_C_tcp_tostring },
    { "close",        LUA_C_tcp_close },
    { "getsockname",  LUA_C_tcp_getsockname },
    { "getpeername",  LUA_C_tcp_getpeername },
    { "type",         LUA_C_tcp_type },
    { "settimeout",   LUA_C_tcp_settimeout },
    { "broadcast",    LUA_C_tcp_broadcast },
    { "send",         LUA_C_tcp_send },
    { "recv",         LUA_C_tcp_recv },
    { "accept",       LUA_C_tcp_accept },
    { "check",        LUA_C_tcp_check },
    { nullptr, nullptr }
  };

static int LUA_C_tcp_new(lua_State* ls)
  {
  xWSA wsa;
  SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == INVALID_SOCKET)
    {
    xmsg msg;
    msg << "建立tcp socket失败:" << WSAGetLastError();
    lua_pushstring(ls, msg.c_str());
    return lua_error(ls);
    }
  if(lua_type(ls, 1) == LUA_TNUMBER)
    {
    sockaddr_in addr;
    addr.sin_addr.s_addr = lua_tointeger(ls, 1);
    xmsg msg;
    msg
      << (int)addr.sin_addr.s_net << '.'
      << (int)addr.sin_addr.s_host << '.'
      << (int)addr.sin_addr.s_lh << '.'
      << (int)addr.sin_addr.s_impno;
    lua_pushstring(ls, msg.c_str());
    lua_replace(ls, 1);
    }
  auto ip = luaL_optstring(ls, 1, "0.0.0.0");
  auto port = luaL_optstring(ls, 2, "0");

  const auto bind_port = luaL_optinteger(ls, 3, 0);
  const bool NonBlockConnect = lua_toboolean(ls, 4);

  try
    {
    auto addr = AddrInfo(ip, port);

    if(bind_port != 0)
      {
      auto s = AddrInfo(INADDR_ANY, bind_port);
      if(bind(sock, (sockaddr*)&s, sizeof(s)))
        {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        xmsg msg;
        msg << "tcp socket绑定[" << (intptr_t)htons(addr.sin_port)
          << "]失败:" << (intptr_t)WSAGetLastError();
        lua_pushstring(ls, msg.c_str());
        return lua_error(ls);
        }
      if(listen(sock, 0))
        {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        xmsg msg;
        msg << "tcp socket监听[" << (intptr_t)htons(addr.sin_port)
          << "]失败:" << (intptr_t)WSAGetLastError();
        lua_pushstring(ls, msg.c_str());
        return lua_error(ls);
        }
      }
    else
      {
      if(addr.sin_addr.S_un.S_addr == INADDR_ANY || addr.sin_port == 0)
        {
        lua_pushstring(ls, "tcp socket 请明确指定连接的地址、端口");
        return lua_error(ls);
        }
      //如果指定了非阻塞连接，就设置SOCK的非阻塞
      if(NonBlockConnect)
        {
        u_long ul = 1;
        ioctlsocket(sock, FIONBIO, &ul);
        connect(sock, (sockaddr*)&addr, sizeof(addr));
        }
      else
        {
        if(connect(sock, (sockaddr*)&addr, sizeof(addr)))
          {
          shutdown(sock, SD_BOTH);
          closesocket(sock);
          xmsg msg;
          msg << "tcp socket连接[" << IpString(addr)
            << "]失败:" << (intptr_t)WSAGetLastError();
          lua_pushstring(ls, msg.c_str());
          return lua_error(ls);
          }
        }
      }

    luaL_getmetatable(ls, gk_tcp_register);
    if(lua_type(ls, -1) == LUA_TNIL)
      {
      lua_pop(ls, 1);
      luaL_newmetatable(ls, gk_tcp_register);
      lua_pushvalue(ls, -1);
      lua_setfield(ls, -2, "__index");
      luaL_setfuncs(ls, gkTCPLib, 0);
      lua_pop(ls, 1);
      }
    else
      {
      lua_pop(ls, 1);
      }

    SOCKET_ST* st = (SOCKET_ST*)lua_newuserdata(ls, sizeof(*st));
    st->sock = sock;
    st->addr = addr;
    st->bind_port = bind_port;

    luaL_setmetatable(ls, gk_tcp_register);
    return 1;
    }
  catch(const runtime_error& err)
    {
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    xmsg msg;
    msg << "建立TCP错误:" << err.what();
    lua_pushstring(ls, msg.c_str());
    return lua_error(ls);
    }
  catch(...)
    {
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    lua_pushstring(ls, "建立TCP异常");
    return lua_error(ls);
    }
  }
//////////////////////////////////////////////////////////////////////////

void register_sock(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "udp_new", LUA_C_udp_new);
  lua_register(ls, "tcp_new", LUA_C_tcp_new);

  lua_pop(ls, lua_gettop(ls));
  }