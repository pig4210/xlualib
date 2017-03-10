#include <string>
#include "xlualib.h"

#define ZLIB_WINAPI
#include <zlib.h>
#pragma comment(lib, "zlib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")

using std::string;

static int LUA_C_zlib_compress(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  string cp;
  cp.reserve(compressBound(l));
  while(true)
    {
    size_t size = cp.capacity();
    const intptr_t rets = compress(
      (Bytef*)cp.end()._Ptr,
      (uLongf*)&size,
      (const Bytef*)s,
      (uLongf)l);
    switch(rets)
      {
      case Z_OK:
        lua_pushlstring(ls, cp.end()._Ptr, size);
        return 1;
      case Z_BUF_ERROR:
        cp.reserve(cp.capacity() + 0x10);
        continue;
      default:
        break;
      }
    lua_pushstring(ls, (xmsg() << "zlib压缩失败 : " << rets).c_str());
    return lua_error(ls);
    }
  }

static int LUA_C_zlib_uncompress(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  string cp;
  cp.reserve(l * 2);
  while(true)
    {
    size_t size = cp.capacity();
    const intptr_t rets = uncompress(
      (Bytef*)cp.end()._Ptr,
      (uLongf*)&size,
      (const Bytef*)s,
      (uLongf)l);
    switch(rets)
      {
      case Z_OK:
        lua_pushlstring(ls, cp.end()._Ptr, size);
        return 1;
      case Z_BUF_ERROR:
        cp.reserve(cp.capacity() + 0x10);
        continue;
      default:
        break;
      }
    lua_pushstring(ls, (xmsg() << "zlib解压失败 : " << rets).c_str());
    return lua_error(ls);
    }
  }

static int LUA_C_gzip_compress(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }

  string cp;
  cp.reserve(compressBound(l));
  while(true)
    {
    z_stream c_stream;
    c_stream.zalloc = nullptr;
    c_stream.zfree = nullptr;
    c_stream.opaque = nullptr;
    intptr_t rets = deflateInit2(&c_stream,
                                 Z_DEFAULT_COMPRESSION,
                                 Z_DEFLATED,
                                 MAX_WBITS + 16,
                                 8,
                                 Z_DEFAULT_STRATEGY);
    if(rets != Z_OK)
      {
      lua_pushstring(ls, (xmsg() << "gzip压缩初始化失败 : " << rets).c_str());
      return lua_error(ls);
      }
    c_stream.next_in = (Bytef*)s;
    c_stream.avail_in = l;
    c_stream.next_out = (Bytef*)cp.c_str();
    c_stream.avail_out = cp.capacity();

    rets = deflate(&c_stream, Z_FINISH);
    const intptr_t rend = deflateEnd(&c_stream);
    switch(rets)
      {
      case Z_STREAM_END:
        if(rend == Z_OK)
          {
          lua_pushlstring(ls, cp.end()._Ptr, c_stream.total_out);
          return 1;
          }
        break;
      case Z_OK:
      case Z_BUF_ERROR:
        cp.reserve(cp.capacity() + 0x10);
        continue;
      default:
        break;
      }
    lua_pushstring(ls, (xmsg() << "压缩失败 : " << rets << ' ' << rend).c_str());
    return lua_error(ls);
    }
  }

static int LUA_C_gzip_uncompress(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }

  string str(s, l);

  string cp;
  cp.reserve(l * 2);
  bool head_add = false;
  static char dummy_head[2] =
    {
    0x8 + 0x7 * 0x10,
    (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
  while(true)
    {
    z_stream d_stream = { 0 };
    d_stream.zalloc = nullptr;
    d_stream.zfree = nullptr;
    d_stream.opaque = nullptr;
    intptr_t rets = inflateInit2(&d_stream, MAX_WBITS + 16);
    if(rets != Z_OK)
      {
      lua_pushstring(ls, (xmsg() << "gzip解压缩初始化失败 : " << rets).c_str());
      return lua_error(ls);
      }
    d_stream.next_in = (Bytef*)str.c_str();
    d_stream.avail_in = str.size();
    d_stream.next_out = (Bytef*)cp.c_str();
    d_stream.avail_out = cp.capacity();

    rets = inflate(&d_stream, Z_FINISH);
    const intptr_t rend = inflateEnd(&d_stream);
    if((rets == Z_DATA_ERROR) && !head_add)
      {
      str.insert(str.begin(), &dummy_head[0], &dummy_head[sizeof(dummy_head)]);
      head_add = true;
      continue;
      }
    switch(rets)
      {
      case Z_STREAM_END:
        if(rend == Z_OK)
          {
          lua_pushlstring(ls, cp.end()._Ptr, d_stream.total_out);
          return 1;
          }
        break;
      case Z_OK:
      case Z_BUF_ERROR:
        cp.reserve(cp.capacity() + 0x10);
        continue;
      default:
        break;
      }
    lua_pushstring(ls, (xmsg() << "解压缩失败 : " << rets).c_str());
    return lua_error(ls);
    }
  }

void register_zlib(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "zlib_compress", LUA_C_zlib_compress);
  lua_register(ls, "zlib_uncompress", LUA_C_zlib_uncompress);

  lua_register(ls, "gzip_compress", LUA_C_gzip_compress);
  lua_register(ls, "gzip_uncompress", LUA_C_gzip_uncompress);

  lua_pop(ls, lua_gettop(ls));
  }