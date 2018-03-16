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
    
  size_t size = compressBound(l);
  unsigned char* data = new unsigned char[size];
  while(true)
    {
    const intptr_t rets = compress(
      (Bytef*)data,
      (uLongf*)&size,
      (const Bytef*)s,
      (uLongf)l);
    switch(rets)
      {
      case Z_OK:
        lua_pushlstring(ls, (const char*)data, size);
        delete[] data;
        return 1;
      case Z_BUF_ERROR:
        size += 0x100;
        delete[] data;
        data = new unsigned char[size];
        continue;
      default:
        break;
      }
    delete[] data;
    lua_pushstring(ls, "");
    lua_pushinteger(ls, rets);
    return 2;
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

  size_t size = l * 2;
  unsigned char* data = new unsigned char[size];
  while(true)
    {
    const intptr_t rets = uncompress(
      (Bytef*)data,
      (uLongf*)&size,
      (const Bytef*)s,
      (uLongf)l);
    switch(rets)
      {
      case Z_OK:
        lua_pushlstring(ls, (const char*)data, size);
        delete[] data;
        return 1;
      case Z_BUF_ERROR:
        size += 0x100;
        delete[] data;
        data = new unsigned char[size];
        continue;
      default:
        break;
      }
    delete[] data;
    lua_pushstring(ls, "");
    lua_pushinteger(ls, rets);
    return 2;
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

  size_t size = compressBound(l);
  unsigned char* data = new unsigned char[size];
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
      delete[] data;
      lua_pushstring(ls, "");
      lua_pushinteger(ls, rets);
      return 2;
      }
    c_stream.next_in = (Bytef*)s;
    c_stream.avail_in = l;
    c_stream.next_out = (Bytef*)data;
    c_stream.avail_out = size;

    rets = deflate(&c_stream, Z_FINISH);
    const intptr_t rend = deflateEnd(&c_stream);
    switch(rets)
      {
      case Z_STREAM_END:
        if(rend == Z_OK)
          {
          lua_pushlstring(ls, (const char*)data, c_stream.total_out);
          delete[] data;
          return 1;
          }
        break;
      case Z_OK:
      case Z_BUF_ERROR:
        size += 0x100;
        delete[] data;
        data = new unsigned char[size];
        continue;
      default:
        break;
      }
    delete[] data;
    lua_pushstring(ls, "");
    lua_pushinteger(ls, rets);
    lua_pushinteger(ls, rend);
    return 3;
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
  bool raw_deflate = false;
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
    /*
    1.8...15     压缩格式为zlib header包裹的deflate原始数据;
    2.-8...-15    压缩格式为原始的deflate数据；
    3.8...15 + (32)  压缩格式为zlib header或者gzip header包括的deflate原始数据。
    */
    intptr_t rets = inflateInit2(&d_stream, raw_deflate ? -MAX_WBITS : MAX_WBITS + 16);
    if(rets != Z_OK)
      {
      lua_pushstring(ls, "");
      lua_pushinteger(ls, rets);
      return 2;
      }
    d_stream.next_in = (Bytef*)str.c_str();
    d_stream.avail_in = str.size();
    d_stream.next_out = (Bytef*)cp.c_str();
    d_stream.avail_out = cp.capacity();

    rets = inflate(&d_stream, Z_FINISH);
    const intptr_t rend = inflateEnd(&d_stream);
    if(rets == Z_DATA_ERROR)
      {
      if(!head_add)   // 尝试加上头
        {
        str.insert(str.begin(), &dummy_head[0], &dummy_head[sizeof(dummy_head)]);
        head_add = true;
        continue;
        }
      if(!raw_deflate)  //尝试使用deflate
        {
        str.erase(str.begin(), str.begin() + sizeof(dummy_head));
        raw_deflate = true;
        continue;
        }
      }
    switch(rets)
      {
      case Z_STREAM_END:
        if(rend == Z_OK)
          {
          lua_pushlstring(ls, cp.c_str(), d_stream.total_out);
          return 1;
          }
        break;
      case Z_OK:
      case Z_BUF_ERROR:
        cp.reserve(cp.capacity() + 0x100);
        continue;
      default:
        break;
      }
    lua_pushstring(ls, "");
    lua_pushinteger(ls, rets);
    lua_pushinteger(ls, rend);
    return 3;
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