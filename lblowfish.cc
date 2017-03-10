#include <string>

#include "xlualib.h"

#include <openssl/blowfish.h>

#pragma comment(lib, "libeay32")

using std::string;

static const int blowfish_block_size = 8;

extern void pkcs7padding(string& data, const int block_size, const bool padding);

static const string blowfish_data(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  return string(s, l);
  }

static BF_KEY blowfish_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 2, &l);

  BF_KEY ks;

  BF_set_key(&ks, l, (const unsigned char*)s);

  return ks;
  }

static int blowfish_ecb(lua_State* ls, const int enc, const bool padding)
  {
  string data(blowfish_data(ls));
  BF_KEY ks(blowfish_key(ls));

  if(padding && enc == BF_ENCRYPT)
    {
    pkcs7padding(data, blowfish_block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % blowfish_block_size));
    }
 
  const auto lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += blowfish_block_size)
    {
    BF_ecb_encrypt((const unsigned char*)&lp[i], (unsigned char*)&lp[i], &ks, enc);
    }

  if(padding && enc == BF_DECRYPT)
    {
    pkcs7padding(data, blowfish_block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static void blowfish_ivec(lua_State* ls, const int ivec_arg, unsigned char* ivec)
  {
  size_t l = 0;
  const auto s = luaL_optlstring(ls, ivec_arg, "\x00\x00\x00\x00\x00\x00\x00\x00", &l);
  string iv(s, l);

  while(iv.size() < blowfish_block_size) iv.push_back('\0');
  memcpy(ivec, iv.c_str(), iv.size());
  }

static int blowfish_cbc(lua_State* ls, const int enc, const bool padding)
  {
  string data(blowfish_data(ls));
  BF_KEY ks(blowfish_key(ls));

  unsigned char ivec[blowfish_block_size];
  blowfish_ivec(ls, 3, ivec);

  if(padding && enc == BF_ENCRYPT)
    {
    pkcs7padding(data, blowfish_block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % blowfish_block_size));
    }

  BF_cbc_encrypt((const unsigned char*)data.c_str(), (unsigned char*)data.c_str(), data.size(), &ks, ivec, enc);

  if(padding && enc == BF_DECRYPT)
    {
    pkcs7padding(data, blowfish_block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static int LUA_C_blowfish_ecb_pkcs7padding_encrypt(lua_State* ls)
  {
  return blowfish_ecb(ls, BF_ENCRYPT, true);
  }

static int LUA_C_blowfish_ecb_pkcs7padding_decrypt(lua_State* ls)
  {
  return blowfish_ecb(ls, BF_DECRYPT, true);
  }

static int LUA_C_blowfish_cbc_pkcs7padding_encrypt(lua_State* ls)
  {
  return blowfish_cbc(ls, BF_ENCRYPT, true);
  }

static int LUA_C_blowfish_cbc_pkcs7padding_decrypt(lua_State* ls)
  {
  return blowfish_cbc(ls, BF_DECRYPT, true);
  }


static int LUA_C_blowfish_ecb_encrypt(lua_State* ls)
  {
  return blowfish_ecb(ls, BF_ENCRYPT, false);
  }

static int LUA_C_blowfish_ecb_decrypt(lua_State* ls)
  {
  return blowfish_ecb(ls, BF_DECRYPT, false);
  }

static int LUA_C_blowfish_cbc_encrypt(lua_State* ls)
  {
  return blowfish_cbc(ls, BF_ENCRYPT, false);
  }

static int LUA_C_blowfish_cbc_decrypt(lua_State* ls)
  {
  return blowfish_cbc(ls, BF_DECRYPT, false);
  }

//////////////////////////////////////////////////////////////////////////
void register_blowfish(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "blowfish_ecb_pkcs7padding_encrypt", LUA_C_blowfish_ecb_pkcs7padding_encrypt);
  lua_register(ls, "blowfish_ecb_pkcs7padding_decrypt", LUA_C_blowfish_ecb_pkcs7padding_decrypt);
  lua_register(ls, "blowfish_cbc_pkcs7padding_encrypt", LUA_C_blowfish_cbc_pkcs7padding_encrypt);
  lua_register(ls, "blowfish_cbc_pkcs7padding_decrypt", LUA_C_blowfish_cbc_pkcs7padding_decrypt);

  lua_register(ls, "blowfish_ecb_encrypt", LUA_C_blowfish_ecb_encrypt);
  lua_register(ls, "blowfish_ecb_decrypt", LUA_C_blowfish_ecb_decrypt);
  lua_register(ls, "blowfish_cbc_encrypt", LUA_C_blowfish_cbc_encrypt);
  lua_register(ls, "blowfish_cbc_decrypt", LUA_C_blowfish_cbc_decrypt);

  lua_pop(ls, lua_gettop(ls));
  }