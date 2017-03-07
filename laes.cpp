#include <string>

#include "xlualib.h"

#include <openssl/aes.h>

#pragma comment(lib, "libeay32")

using std::string;

void pkcs7padding(string& data, const int block_size, const bool padding)
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

static const string aes_data(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  return string(s, l);
  }

static AES_KEY aes_key(lua_State* ls, const bool enc)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 2, &l);
  string key(s, l);

  while(key.size() < AES_BLOCK_SIZE) key.push_back('\0');

  AES_KEY ks;
  int ret;
  if(enc == AES_ENCRYPT)
    {
    ret = AES_set_encrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &ks);
    }
  else
    {
    ret = AES_set_decrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &ks);
    }

  if(ret < 0)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "ÉèÖÃAes KeyÊ§°Ü : " << ret
                   ).c_str());
    lua_error(ls);
    }

  return ks;
  }

static int aes_ecb(lua_State* ls, const int enc, const bool padding)
  {
  string data(aes_data(ls));
  AES_KEY ks(aes_key(ls, enc));

  const unsigned int block_size = luaL_optinteger(ls, 3, AES_BLOCK_SIZE);

  if(padding && enc == AES_ENCRYPT)
    {
    pkcs7padding(data, block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % block_size));
    }
 
  const auto lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += AES_BLOCK_SIZE)
    {
    AES_ecb_encrypt((const unsigned char*)&lp[i], (unsigned char*)&lp[i], &ks, enc);
    }

  if(padding && enc == AES_DECRYPT)
    {
    pkcs7padding(data, block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static void aes_ivec(lua_State* ls, const int ivec_arg, unsigned char* ivec)
  {
  size_t l = 0;
  const auto s = luaL_optlstring(ls, ivec_arg, "\x00\x00\x00\x00\x00\x00\x00\x00", &l);
  string iv(s, l);

  while(iv.size() < AES_BLOCK_SIZE) iv.push_back('\0');
  memcpy(ivec, iv.c_str(), iv.size());
  }

static int aes_cbc(lua_State* ls, const int enc, const bool padding)
  {
  string data(aes_data(ls));
  AES_KEY ks(aes_key(ls, enc));

  unsigned char ivec[AES_BLOCK_SIZE];
  aes_ivec(ls, 3, ivec);

  const unsigned int block_size = luaL_optinteger(ls, 4, AES_BLOCK_SIZE);
  if(padding && enc == AES_ENCRYPT)
    {
    pkcs7padding(data, block_size, true);
    }
  else
    {
    data.resize(data.size() - (data.size() % block_size));
    }

  AES_cbc_encrypt((const unsigned char*)data.c_str(), (unsigned char*)data.c_str(), data.size(), &ks, ivec, enc);

  if(padding && enc == AES_DECRYPT)
    {
    pkcs7padding(data, block_size, false);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static int LUA_C_aes_ecb_pkcs7padding_encrypt(lua_State* ls)
  {
  return aes_ecb(ls, AES_ENCRYPT, true);
  }

static int LUA_C_aes_ecb_pkcs7padding_decrypt(lua_State* ls)
  {
  return aes_ecb(ls, AES_DECRYPT, true);
  }

static int LUA_C_aes_cbc_pkcs7padding_encrypt(lua_State* ls)
  {
  return aes_cbc(ls, AES_ENCRYPT, true);
  }

static int LUA_C_aes_cbc_pkcs7padding_decrypt(lua_State* ls)
  {
  return aes_cbc(ls, AES_DECRYPT, true);
  }


static int LUA_C_aes_ecb_encrypt(lua_State* ls)
  {
  return aes_ecb(ls, AES_ENCRYPT, false);
  }

static int LUA_C_aes_ecb_decrypt(lua_State* ls)
  {
  return aes_ecb(ls, AES_DECRYPT, false);
  }

static int LUA_C_aes_cbc_encrypt(lua_State* ls)
  {
  return aes_cbc(ls, AES_ENCRYPT, false);
  }

static int LUA_C_aes_cbc_decrypt(lua_State* ls)
  {
  return aes_cbc(ls, AES_DECRYPT, false);
  }

//////////////////////////////////////////////////////////////////////////
ADD_XLUALIB_REGISTER(aes)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "aes_ecb_pkcs7padding_encrypt", LUA_C_aes_ecb_pkcs7padding_encrypt);
  lua_register(ls, "aes_ecb_pkcs7padding_decrypt", LUA_C_aes_ecb_pkcs7padding_decrypt);
  lua_register(ls, "aes_cbc_pkcs7padding_encrypt", LUA_C_aes_cbc_pkcs7padding_encrypt);
  lua_register(ls, "aes_cbc_pkcs7padding_decrypt", LUA_C_aes_cbc_pkcs7padding_decrypt);

  lua_register(ls, "aes_ecb_encrypt", LUA_C_aes_ecb_encrypt);
  lua_register(ls, "aes_ecb_decrypt", LUA_C_aes_ecb_decrypt);
  lua_register(ls, "aes_cbc_encrypt", LUA_C_aes_cbc_encrypt);
  lua_register(ls, "aes_cbc_decrypt", LUA_C_aes_cbc_decrypt);

  lua_pop(ls, lua_gettop(ls));
  }