#include <string>

#include "xlualib.h"

using std::string;

//////////////////////////////////////////////////////////////////////////
static int LUA_C_xrand(lua_State* ls)
  {
  lua_pushinteger(ls, xrand((size_t)luaL_optinteger(ls, 1, 0)));
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_strxor(lua_State* ls)
  {
  size_t l = 0;
  const unsigned char* s = (const unsigned char*)luaL_checklstring(ls, 1, &l);
  string xors;
  if(LUA_TNUMBER == lua_type(ls, 2))
    {
    auto x = luaL_checkinteger(ls, 2);
    xors.assign((const char*)&x, sizeof(x));
    while(!xors.empty() && *xors.rbegin() == 0x00)
      {
      xors.pop_back();
      }
    }
  else
    {
    size_t l = 0;
    const auto s = luaL_checklstring(ls, 2, &l);
    xors.assign(s, l);
    }
  if(xors.empty())
    {
    lua_pushstring(ls, "未能正确提供xor因子");
    return lua_error(ls);
    }

  string ss;
  for(size_t i = 0; i < l; ++i)
    {
    const unsigned char ch = s[i] ^ xors[i % xors.size()];
    ss.push_back(ch);
    }

  lua_pushlstring(ls, ss.c_str(), ss.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_md5(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto mv(md5(s, l));

  lua_pushlstring(ls, (const char*)&mv.Data, sizeof(mv.Data));
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_crc16(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  lua_pushinteger(ls, crc16(s, l));
  return 1;
  }

static int LUA_C_crc32(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  lua_pushinteger(ls, crc32(s, l));
  return 1;
  }

static int LUA_C_crc64(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  lua_pushinteger(ls, crc64(s, l));
  return 1;
  }

static int LUA_C_crcccitt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  lua_pushinteger(ls, crcccitt(s, l));
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_tovarint(lua_State* ls)
  {
  const auto v = luaL_checkinteger(ls, 1);
  const bool s = lua_toboolean(ls, 2);
  if(s)
    {
    const auto res = tovarint((int64)v);
    lua_pushlstring(ls, (const char*)res.c_str(), res.size());
    }
  else
    {
    const auto res = tovarint((uint64)v);
    lua_pushlstring(ls, (const char*)res.c_str(), res.size());
    }

  return 1;
  }
static int LUA_C_getvarint(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  const bool is = lua_toboolean(ls, 2);
  
  int64 sv = 0;
  uint64 uv = 0;
  if(is)
    {
    const auto res = getvarint(sv, (const unsigned char*)s, l);
    lua_pushinteger(ls, sv);
    lua_pushinteger(ls, res);
    }
  else
    {
    const auto res = getvarint(uv, (const unsigned char*)s, l);
    lua_pushinteger(ls, uv);
    lua_pushinteger(ls, res);
    }
  return 2;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_TeanEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);
  
  const auto res(TeanEncrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeanDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(TeanDecrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeaEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(
    TeaEncrypt(
    s,
    l,
    TEA_KEY(ks, kl),
    luaL_checkinteger(ls, 3),
    luaL_checkinteger(ls, 4)
    ));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeaDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(
    TeaDecrypt(
    s,
    l,
    TEA_KEY(ks, kl),
    luaL_checkinteger(ls, 3),
    luaL_checkinteger(ls, 4)
    ));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeanEncipher(lua_State* ls)
  {
  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(
    TeanEncipher(luaL_checkstring(ls, 1), TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.Data, sizeof(res.Data));
  return 1;
  }

static int LUA_C_TeanDecipher(lua_State* ls)
  {
  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(
    TeanDecipher(luaL_checkstring(ls, 1), TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.Data, sizeof(res.Data));
  return 1;
  }

static int LUA_C_XTeanEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(XTeanEncrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XTeanDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(XTeanDecrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XxTeaEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(XxTeaEncrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XxTeaDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(XxTeaDecrypt(s, l, TEA_KEY(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_aes_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(AesEncrypt(s, l, AesKey(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_aes_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(AesDecrypt(s, l, AesKey(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_des_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(DesEncrypt(s, l, DesKey(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_des_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto ks = luaL_checklstring(ls, 2, &kl);

  const auto res(DesDecrypt(s, l, DesKey(ks, kl)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
void register_algorithm(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "xrand", LUA_C_xrand);

  lua_register(ls, "strxor", LUA_C_strxor);

  lua_register(ls, "md5", LUA_C_md5);

  lua_register(ls, "crc16", LUA_C_crc16);
  lua_register(ls, "crc32", LUA_C_crc32);
  lua_register(ls, "crc64", LUA_C_crc64);
  lua_register(ls, "crcccitt", LUA_C_crcccitt);

  lua_register(ls, "tovarint", LUA_C_tovarint);
  lua_register(ls, "getvarint", LUA_C_getvarint);

  lua_register(ls, "TeanEncrypt", LUA_C_TeanEncrypt);
  lua_register(ls, "TeanDecrypt", LUA_C_TeanDecrypt);
  lua_register(ls, "TeaEncrypt", LUA_C_TeaEncrypt);
  lua_register(ls, "TeaDecrypt", LUA_C_TeaDecrypt);
  lua_register(ls, "TeanEncipher", LUA_C_TeanEncipher);
  lua_register(ls, "TeanDecipher", LUA_C_TeanDecipher);
  lua_register(ls, "XTeanEncrypt", LUA_C_XTeanEncrypt);
  lua_register(ls, "XTeanDecrypt", LUA_C_XTeanDecrypt);
  lua_register(ls, "XxTeaEncrypt", LUA_C_XxTeaEncrypt);
  lua_register(ls, "XxTeaDecrypt", LUA_C_XxTeaDecrypt);

  lua_register(ls, "aes_encrypt", LUA_C_aes_encrypt);
  lua_register(ls, "aes_decrypt", LUA_C_aes_decrypt);

  lua_register(ls, "des_encrypt", LUA_C_des_encrypt);
  lua_register(ls, "des_decrypt", LUA_C_des_decrypt);

  lua_pop(ls, lua_gettop(ls));
  }