#include "xlualib.h"

//////////////////////////////////////////////////////////////////////////
static int LUA_C_xrand(lua_State* ls)
  {
  lua_pushinteger(ls, xrand((TULONG)luaL_optinteger(ls, 1, 0)));
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_strxor(lua_State* ls)
  {
  size_t l = 0;
  const unsigned char* s = (const unsigned char*)luaL_checklstring(ls, 1, &l);
  const unsigned char x = luaL_checkinteger(ls, 2);

  string ss((const char*)s, l);
  for(auto& ch : ss)
    {
    ch ^= x;
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
//////////////////////////////////////////////////////////////////////////
static int LUA_C_TeanEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(TeanEncrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeanDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(TeanDecrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeaEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(
    TeaEncrypt(
    s,
    l,
    TEAN_KEY(luaL_checkstring(ls, 2)),
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

  const auto res(
    TeaDecrypt(
    s,
    l,
    TEAN_KEY(luaL_checkstring(ls, 2)),
    luaL_checkinteger(ls, 3),
    luaL_checkinteger(ls, 4)
    ));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_TeanEncipher(lua_State* ls)
  {
  const auto res(
    TeanEncipher(luaL_checkstring(ls, 1), TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.Data, sizeof(res.Data));
  return 1;
  }

static int LUA_C_TeanDecipher(lua_State* ls)
  {
  const auto res(
    TeanDecipher(luaL_checkstring(ls, 1), TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.Data, sizeof(res.Data));
  return 1;
  }

static int LUA_C_XTeanEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(XTeanEncrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XTeanDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(XTeanDecrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XxTeaEncrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(XxTeaEncrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_XxTeaDecrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const auto res(XxTeaDecrypt(s, l, TEAN_KEY(luaL_checkstring(ls, 2))));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_aes_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t ll = 0;
  const auto key = luaL_checklstring(ls, 2, &ll);

  const auto res(AesEncrypt(s, l, AesKey(key, ll)));

  lua_pushlstring(ls, (const char*)res.c_str(), res.size());
  return 1;
  }

static int LUA_C_aes_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t ll = 0;
  const auto key = luaL_checklstring(ls, 2, &ll);

  const auto res(AesDecrypt(s, l, AesKey(key, ll)));

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


  lua_pop(ls, lua_gettop(ls));
  }