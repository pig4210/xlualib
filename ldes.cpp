#include "xlualib.h"

#include <openssl/des.h>

#pragma comment(lib, "libeay32")

static void pkcs7padding(string& data, const int block_size, const bool padding)
  {
  if(padding)
    {
    const char ch = block_size - (data.size() % block_size);
    data.append((size_t)ch, ch);
    return;
    }
  if(data.empty())  return;
  const unsigned char ch = *data.rbegin();

  if(ch <= block_size)
    {
    bool ispadding = true;
    for(auto it = data.rbegin(); it != (data.rbegin() + ch); ++it)
      {
      if(*it != ch)
        {
        ispadding = false;
        break;
        }
      }
    if(ispadding) data.resize(data.size() - ch);
    }
  }

static const string des_data(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  return string(s, l);
  }

static DES_key_schedule des_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 2, &l);
  string key(s, l);

  while(key.size() < sizeof(DES_cblock)) key.push_back('\0');

  DES_key_schedule ks;
  DES_set_key_unchecked((const_DES_cblock*)key.c_str(), &ks);
  return ks;
  }

static void des_key3(lua_State* ls, DES_key_schedule ks[3])
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 2, &l);
  string key(s, l);
  //K1¡¢K2²¹Æë
  while(key.size() < 2 * sizeof(DES_cblock)) key.push_back('\0');
  //K3 == K1
  if(key.size() == 2 * sizeof(DES_cblock))
    {
    key += string(key.begin(), key.begin() + sizeof(DES_cblock));
    }
  //K3²¹Æë
  while(key.size() < 3 * sizeof(DES_cblock)) key.push_back('\0');

  const_DES_cblock* lpk = (const_DES_cblock*)key.c_str();

  DES_set_key_unchecked(&lpk[0], &ks[0]);
  DES_set_key_unchecked(&lpk[1], &ks[1]);
  DES_set_key_unchecked(&lpk[2], &ks[2]);
  }

static int des_ecb(lua_State* ls, const int enc, const bool padding)
  {
  string data(des_data(ls));
  DES_key_schedule ks(des_key(ls));

  const unsigned int block_size = luaL_optinteger(ls, 3, sizeof(DES_cblock));

  if(padding && enc == DES_ENCRYPT)
    {
    pkcs7padding(data, block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % block_size));
    }

  const auto lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += sizeof(DES_cblock))
    {
    DES_ecb_encrypt((const_DES_cblock*)&lp[i], (DES_cblock*)&lp[i], &ks, enc);
    }

  if(padding && enc == DES_DECRYPT)
    {
    pkcs7padding(data, block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static int des_ecb3(lua_State* ls, const int enc, const bool padding)
  {
  string data(des_data(ls));
  DES_key_schedule ks[3];
  des_key3(ls, ks);

  const unsigned int block_size = luaL_optinteger(ls, 3, sizeof(DES_cblock));

  if(padding && enc == DES_ENCRYPT)
    {
    pkcs7padding(data, block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % block_size));
    }

  const auto lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += sizeof(DES_cblock))
    {
    DES_ecb3_encrypt((const_DES_cblock*)&lp[i], (DES_cblock*)&lp[i], &ks[0], &ks[1], &ks[2], enc);
    }

  if(padding && enc == DES_DECRYPT)
    {
    pkcs7padding(data, block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static void des_ivec(lua_State* ls, const int ivec_arg, DES_cblock* ivec)
  {
  size_t l = 0;
  const auto s = luaL_optlstring(ls, ivec_arg, "\x00\x00\x00\x00\x00\x00\x00\x00", &l);
  string iv(s, l);

  while(iv.size() < sizeof(*ivec)) iv.push_back('\0');
  memcpy(ivec, iv.c_str(), iv.size());
  }

static int des_cbc(lua_State* ls, const int enc, const bool padding, const bool fresh_ivec)
  {
  string data(des_data(ls));
  DES_key_schedule ks(des_key(ls));

  DES_cblock ivec;
  des_ivec(ls, 3, &ivec);

  const unsigned int block_size = luaL_optinteger(ls, 4, sizeof(DES_cblock));
  if(padding && enc == DES_ENCRYPT)
    {
    pkcs7padding(data, block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % block_size));
    }

  if(fresh_ivec)
    {
    DES_ncbc_encrypt((const unsigned char*)data.c_str(), (unsigned char*)data.c_str(), data.size(), &ks, &ivec, enc);
    }
  else
    {
    DES_cbc_encrypt((const unsigned char*)data.c_str(), (unsigned char*)data.c_str(), data.size(), &ks, &ivec, enc);
    }

  if(padding && enc == DES_DECRYPT)
    {
    pkcs7padding(data, block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static int LUA_C_des_ecb_pkcs7padding_encrypt(lua_State* ls)
  {
  return des_ecb(ls, DES_ENCRYPT, true);
  }

static int LUA_C_des_ecb_pkcs7padding_decrypt(lua_State* ls)
  {
  return des_ecb(ls, DES_DECRYPT, true);
  }

static int LUA_C_des_cbc_pkcs7padding_encrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_ENCRYPT, true, false);
  }

static int LUA_C_des_cbc_pkcs7padding_decrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_DECRYPT, true, false);
  }

static int LUA_C_des_ncbc_pkcs7padding_encrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_ENCRYPT, true, true);
  }

static int LUA_C_des_ncbc_pkcs7padding_decrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_DECRYPT, true, true);
  }

static int LUA_C_des_ecb3_pkcs7padding_encrypt(lua_State* ls)
  {
  return des_ecb3(ls, DES_ENCRYPT, true);
  }

static int LUA_C_des_ecb3_pkcs7padding_decrypt(lua_State* ls)
  {
  return des_ecb3(ls, DES_DECRYPT, true);
  }

static int LUA_C_des_ecb_encrypt(lua_State* ls)
  {
  return des_ecb(ls, DES_ENCRYPT, false);
  }

static int LUA_C_des_ecb_decrypt(lua_State* ls)
  {
  return des_ecb(ls, DES_DECRYPT, false);
  }

static int LUA_C_des_cbc_encrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_ENCRYPT, false, false);
  }

static int LUA_C_des_cbc_decrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_DECRYPT, false, false);
  }

static int LUA_C_des_ncbc_encrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_ENCRYPT, false, true);
  }

static int LUA_C_des_ncbc_decrypt(lua_State* ls)
  {
  return des_cbc(ls, DES_DECRYPT, false, true);
  }

static int LUA_C_des_ecb3_encrypt(lua_State* ls)
  {
  return des_ecb3(ls, DES_ENCRYPT, false);
  }

static int LUA_C_des_ecb3_decrypt(lua_State* ls)
  {
  return des_ecb3(ls, DES_DECRYPT, false);
  }

//////////////////////////////////////////////////////////////////////////
void register_des(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "des_ecb_pkcs7padding_encrypt", LUA_C_des_ecb_pkcs7padding_encrypt);
  lua_register(ls, "des_ecb_pkcs7padding_decrypt", LUA_C_des_ecb_pkcs7padding_decrypt);
  lua_register(ls, "des_cbc_pkcs7padding_encrypt", LUA_C_des_cbc_pkcs7padding_encrypt);
  lua_register(ls, "des_cbc_pkcs7padding_decrypt", LUA_C_des_cbc_pkcs7padding_decrypt);
  lua_register(ls, "des_ncbc_pkcs7padding_encrypt", LUA_C_des_ncbc_pkcs7padding_encrypt);
  lua_register(ls, "des_ncbc_pkcs7padding_decrypt", LUA_C_des_ncbc_pkcs7padding_decrypt);
  lua_register(ls, "des_ecb3_pkcs7padding_encrypt", LUA_C_des_ecb3_pkcs7padding_encrypt);
  lua_register(ls, "des_ecb3_pkcs7padding_decrypt", LUA_C_des_ecb3_pkcs7padding_decrypt);

  lua_register(ls, "des_ecb_encrypt", LUA_C_des_ecb_encrypt);
  lua_register(ls, "des_ecb_decrypt", LUA_C_des_ecb_decrypt);
  lua_register(ls, "des_cbc_encrypt", LUA_C_des_cbc_encrypt);
  lua_register(ls, "des_cbc_decrypt", LUA_C_des_cbc_decrypt);
  lua_register(ls, "des_ncbc_encrypt", LUA_C_des_ncbc_encrypt);
  lua_register(ls, "des_ncbc_decrypt", LUA_C_des_ncbc_decrypt);
  lua_register(ls, "des_ecb3_encrypt", LUA_C_des_ecb3_encrypt);
  lua_register(ls, "des_ecb3_decrypt", LUA_C_des_ecb3_decrypt);

  lua_pop(ls, lua_gettop(ls));
  }