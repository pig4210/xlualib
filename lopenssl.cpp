#include "xlualib.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/des.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

#pragma comment(lib, "libeay32")
//////////////////////////////////////////////////////////////////////////
static const char* gk_rsakey = "RsaKey*";

struct RSAKEY_ST
  {
  RSA*    key;
  };

static int LUA_C_rsa_key_gc(lua_State* ls)
  {
  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 1, gk_rsakey);

  if(st->key != nullptr && st->key != INVALID_HANDLE_VALUE)
    {
    RSA_free(st->key);
    st->key = nullptr;
    }
  return 0;
  }

static int LUA_C_rsa_key_tostring(lua_State* ls)
  {
  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 1, gk_rsakey);

  xmsg msg;
  msg << gk_rsakey << ":" << (void*)st->key;

  lua_pushstring(ls, msg.c_str());
  return 1;
  }

static int rsa_open_public_key(lua_State* ls, FILE* keyfile)
  {
  RSA* k = PEM_read_RSA_PUBKEY(keyfile, nullptr, nullptr, nullptr);

  if(k == nullptr)
    {
    fclose(keyfile);
    lua_pushstring(ls,
                   (
                   xmsg() << "读取Rsa Public Key失败 : " << errno
                   ).c_str());
    return lua_error(ls);
    }

  if(0 != fclose(keyfile))
    {
    RSA_free(k);
    lua_pushstring(ls,
                   (
                   xmsg() << "关闭Rsa Public Key失败 : " << errno
                   ).c_str());
    return lua_error(ls);
    }

  RSAKEY_ST* st = (RSAKEY_ST*)lua_newuserdata(ls, sizeof(*st));
  st->key = k;

  luaL_setmetatable(ls, gk_rsakey);
  return 1;
  }

static int LUA_C_rsa_open_public_key(lua_State* ls)
  {
  const char* filename = luaL_checkstring(ls, 1);

  FILE* keyfile = nullptr;
  errno_t en = fopen_s(&keyfile, filename, "r");
  if(0 != en)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "打开RSA Public Key[" << filename << "]失败 : " << en
                   ).c_str());
    return lua_error(ls);
    }
  return rsa_open_public_key(ls, keyfile);
  }

static int LUA_C_rsa_set_public_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "Rsa Public Key不允许为空");
    return lua_error(ls);
    }

  FILE* keyfile = nullptr;
  errno_t en = tmpfile_s(&keyfile);
  if(0 != en)
    {
    lua_pushstring(ls, (xmsg() << "建立Rsa Public Key文件失败 : " << en).c_str());
    return lua_error(ls);
    }

  if(1 != fwrite(s, l, 1, keyfile))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "写入Rsa Public Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }

  if(0 != fflush(keyfile))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "刷新Rsa Public Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }

  if(0 != fseek(keyfile, 0, SEEK_SET))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "设置Rsa Public Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }
  return rsa_open_public_key(ls, keyfile);
  }

static int rsa_open_private_key(lua_State* ls, FILE* keyfile)
  {
  RSA* k = PEM_read_RSAPrivateKey(keyfile, nullptr, nullptr, nullptr);

  if(k == nullptr)
    {
    fclose(keyfile);
    lua_pushstring(ls,
                   (
                   xmsg() << "读取Rsa Private Key失败 : " << errno
                   ).c_str());
    return lua_error(ls);
    }

  if(0 != fclose(keyfile))
    {
    RSA_free(k);
    lua_pushstring(ls,
                   (
                   xmsg() << "关闭Rsa Private Key失败 : " << errno
                   ).c_str());
    return lua_error(ls);
    }

  RSAKEY_ST* st = (RSAKEY_ST*)lua_newuserdata(ls, sizeof(*st));
  st->key = k;

  luaL_setmetatable(ls, gk_rsakey);
  return 1;
  }

static int LUA_C_rsa_open_private_key(lua_State* ls)
  {
  const char* filename = luaL_checkstring(ls, 1);

  FILE* keyfile = nullptr;
  errno_t en = fopen_s(&keyfile, filename, "r");
  if(0 != en)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "打开Rsa Private Key[" << filename << "]失败 : " << en
                   ).c_str());
    return lua_error(ls);
    }
  return rsa_open_private_key(ls, keyfile);
  }

static int LUA_C_rsa_set_private_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "Rsa Private Key不允许为空");
    return lua_error(ls);
    }

  FILE* keyfile = nullptr;
  errno_t en = tmpfile_s(&keyfile);
  if(0 != en)
    {
    lua_pushstring(ls, (xmsg() << "建立Rsa Private Key文件失败 : " << en).c_str());
    return lua_error(ls);
    }

  if(1 != fwrite(s, l, 1, keyfile))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "写入Rsa Private Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }

  if(0 != fflush(keyfile))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "刷新Rsa Private Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }

  if(0 != fseek(keyfile, 0, SEEK_SET))
    {
    fclose(keyfile);
    lua_pushstring(ls, (xmsg() << "设置Rsa Private Key文件失败 : " << errno).c_str());
    return lua_error(ls);
    }
  return rsa_open_private_key(ls, keyfile);
  }

static int LUA_C_rsa_public_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 2, gk_rsakey);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  RSA* key = st->key;
  const intptr_t digitsize = RSA_size(key);
  const intptr_t chunksize = digitsize - 11;

  const unsigned char* lp = (const unsigned char*)s;
  string buf(digitsize, '\0');
  string res;

  for(intptr_t size = 0; size < (intptr_t)l;)
    {
    intptr_t msglen = l - size;
    if(msglen > chunksize) msglen = chunksize;
    const intptr_t len = RSA_public_encrypt(
      msglen, lp + size, (unsigned char*)buf.c_str(), key, RSA_PKCS1_PADDING);
    if(len <= 0)
      {
      lua_pushstring(ls, "Rsa Public Encrypt失败");
      return lua_error(ls);
      }
    res.append(buf.c_str(), len);

    size += msglen;
    }
  lua_pushlstring(ls, res.c_str(), res.size());
  return 1;
  }

static int LUA_C_rsa_private_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 2, gk_rsakey);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  RSA* key = st->key;
  const intptr_t digitsize = RSA_size(key);
  const intptr_t chunksize = digitsize - 11;

  const unsigned char* lp = (const unsigned char*)s;
  string buf(digitsize, '\0');
  string res;

  for(intptr_t size = 0; size < (intptr_t)l;)
    {
    intptr_t msglen = l - size;
    if(msglen > chunksize) msglen = chunksize;
    const intptr_t len = RSA_private_encrypt(
      msglen, lp + size, (unsigned char*)buf.c_str(), key, RSA_PKCS1_PADDING);
    if(len <= 0)
      {
      lua_pushstring(ls, "Rsa Private Encrypt失败");
      return lua_error(ls);
      }
    res.append(buf.c_str(), len);

    size += msglen;
    }
  lua_pushlstring(ls, res.c_str(), res.size());
  return 1;
  }

static int LUA_C_rsa_public_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 2, gk_rsakey);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  RSA* key = st->key;
  const intptr_t digitsize = RSA_size(key);

  const unsigned char* lp = (const unsigned char*)s;
  string buf(digitsize, '\0');
  string res;

  for(intptr_t size = 0; size < (intptr_t)l;)
    {
    intptr_t msglen = l - size;
    if(msglen > digitsize) msglen = digitsize;
    const intptr_t len = RSA_public_decrypt(
      msglen, lp + size, (unsigned char*)buf.c_str(), key, RSA_PKCS1_PADDING);
    if(len <= 0)
      {
      lua_pushstring(ls, "Rsa Public Decrypt失败");
      return lua_error(ls);
      }
    res.append(buf.c_str(), len);

    size += msglen;
    }
  lua_pushlstring(ls, res.c_str(), res.size());
  return 1;
  }

static int LUA_C_rsa_private_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  RSAKEY_ST* st = (RSAKEY_ST*)luaL_checkudata(ls, 2, gk_rsakey);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }
  RSA* key = st->key;
  const intptr_t digitsize = RSA_size(key);

  const unsigned char* lp = (const unsigned char*)s;
  string buf(digitsize, '\0');
  string res;

  for(intptr_t size = 0; size < (intptr_t)l;)
    {
    intptr_t msglen = l - size;
    if(msglen > digitsize) msglen = digitsize;
    const intptr_t len = RSA_private_decrypt(
      msglen, lp + size, (unsigned char*)buf.c_str(), key, RSA_PKCS1_PADDING);
    if(len <= 0)
      {
      lua_pushstring(ls, "Rsa Private Decrypt失败");
      return lua_error(ls);
      }
    res.append(buf.c_str(), len);

    size += msglen;
    }
  lua_pushlstring(ls, res.c_str(), res.size());
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_des_encrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  string data(s, l);

  DES_key_schedule ks;
  DES_set_key_unchecked((const_DES_cblock*)luaL_checkstring(ls, 2), &ks);

  const char ch = 8 - (data.size() % 8);
  data.append((size_t)ch, ch);

  const char* lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += sizeof(DES_cblock))
    {
    DES_ecb_encrypt((const_DES_cblock*)&lp[i], (DES_cblock*)&lp[i], &ks, DES_ENCRYPT);
    }

  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

static int LUA_C_des_decrypt(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  string data(s, l);

  DES_key_schedule ks;
  DES_set_key_unchecked((const_DES_cblock*)luaL_checkstring(ls, 2), &ks);
  if(data.size() % 8 != 0)
    {
    lua_pushstring(ls, "DES解密串应该是8的倍长");
    return lua_error(ls);
    }

  const char* lp = data.c_str();
  for(size_t i = 0; i < data.size(); i += sizeof(DES_cblock))
    {
    DES_ecb_encrypt((const_DES_cblock*)&lp[i], (DES_cblock*)&lp[i], &ks, DES_DECRYPT);
    }

  const unsigned char ch = *data.rbegin();

  if(ch <= sizeof(DES_cblock))
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
  lua_pushlstring(ls, data.c_str(), data.size());
  return 1;
  }

//////////////////////////////////////////////////////////////////////////
static int LUA_C_base64_encode(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const bool newline = lua_toboolean(ls, 2);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 0;
    }

  BIO* b64 = BIO_new(BIO_f_base64());
  if(!newline)  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO * bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, s, l);
  BIO_flush(b64);
  BUF_MEM * bptr = nullptr;
  BIO_get_mem_ptr(b64, &bptr);

  lua_pushlstring(ls, bptr->data, bptr->length);
  BIO_free_all(b64);

  return 1;
  }

static int LUA_C_base64_decode(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  const bool newline = lua_toboolean(ls, 2);

  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 0;
    }

  BIO* b64 = BIO_new(BIO_f_base64());
  if(!newline)  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

  const size_t maxlen = (l * 6 + 7) / 8;

  unsigned char* buf = new unsigned char[maxlen];

  BIO* bmem = BIO_new_mem_buf((void*)s, l);
  b64 = BIO_push(b64, bmem);

  intptr_t readlen = BIO_read(b64, (void*)buf, maxlen);

  lua_pushlstring(ls, (const char*)buf, readlen);
  delete[] buf;
  BIO_free_all(b64);

  return 1;
  }
//////////////////////////////////////////////////////////////////////////
static int LUA_C_hmac(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);

  size_t kl = 0;
  const auto k = luaL_checklstring(ls, 2, &kl);

  auto algo = luaL_checkstring(ls, 3);

  const EVP_MD* engine = nullptr;

  if(_stricmp(algo, "sha512") == 0)
    {
    engine = EVP_sha512();
    }
  else if(_stricmp("sha256", algo) == 0)
    {
    engine = EVP_sha256();
    }
  else if(_stricmp("sha1", algo) == 0)
    {
    engine = EVP_sha1();
    }
  else if(_stricmp("md5", algo) == 0)
    {
    engine = EVP_md5();
    }
  else if(_stricmp("sha224", algo) == 0)
    {
    engine = EVP_sha224();
    }
  else if(_stricmp("sha384", algo) == 0)
    {
    engine = EVP_sha384();
    }
  else if(_stricmp("sha", algo) == 0)
    {
    engine = EVP_sha();
    }
  else
    {
    lua_pushstring(ls, (xmsg() << "HMAC无法识别的算法标识 : " << algo).c_str());
    return lua_error(ls);
    }

  unsigned char output[EVP_MAX_MD_SIZE];
  unsigned int output_length = 0;

  HMAC_CTX ctx;
  HMAC_CTX_init(&ctx);
  HMAC_Init_ex(&ctx, k, kl, engine, NULL);
  HMAC_Update(&ctx, (const unsigned char*)s, l);

  HMAC_Final(&ctx, output, &output_length);
  HMAC_CTX_cleanup(&ctx);

  lua_pushlstring(ls, (const char*)output, output_length);
  return 1;
  }

static int LUA_C_sha(lua_State* ls)
  {
  size_t l = 0;
  const unsigned char* s = (const unsigned char*)luaL_checklstring(ls, 1, &l);

  auto algo = luaL_optinteger(ls, 2, 256);

  unsigned char sha[SHA512_DIGEST_LENGTH];

  unsigned int len = 0;

  switch(algo)
    {
    case 256:
      SHA256(s, l, sha);
      len = SHA256_DIGEST_LENGTH;
      break;
    case 512:
      SHA512(s, l, sha);
      len = SHA512_DIGEST_LENGTH;
      break;
    case 1:
      SHA1(s, l, sha);
      len = SHA_DIGEST_LENGTH;
      break;
    case 0:
      SHA(s, l, sha);
      len = SHA_DIGEST_LENGTH;
      break;
    case 224:
      SHA224(s, l, sha);
      len = SHA224_DIGEST_LENGTH;
      break;
    case 384:
      SHA384(s, l, sha);
      len = SHA384_DIGEST_LENGTH;
      break;
    default:
      lua_pushstring(ls, (xmsg() << "SHA无法识别的算法标识 : " << algo).c_str());
      return lua_error(ls);
    }

  lua_pushlstring(ls, (const char*)sha, len);
  return 1;
  }



//////////////////////////////////////////////////////////////////////////
void register_openssl(lua_State* ls)
  {
  lua_pop(ls, lua_gettop(ls));

  luaL_newmetatable(ls, gk_rsakey);

  lua_pushvalue(ls, -1);
  lua_setfield(ls, -2, "__index");

  lua_pushcfunction(ls, LUA_C_rsa_key_gc);
  lua_setfield(ls, -2, "__gc");

  lua_pushcfunction(ls, LUA_C_rsa_key_tostring);
  lua_setfield(ls, -2, "__tostring");

  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "rsa_open_public_key", LUA_C_rsa_open_public_key);
  lua_register(ls, "rsa_set_public_key", LUA_C_rsa_set_public_key);

  lua_register(ls, "rsa_open_private_key", LUA_C_rsa_open_private_key);
  lua_register(ls, "rsa_set_private_key", LUA_C_rsa_set_private_key);

  lua_register(ls, "rsa_public_encrypt", LUA_C_rsa_public_encrypt);
  lua_register(ls, "rsa_private_encrypt", LUA_C_rsa_private_encrypt);

  lua_register(ls, "rsa_public_decrypt", LUA_C_rsa_public_decrypt);
  lua_register(ls, "rsa_private_decrypt", LUA_C_rsa_private_decrypt);


  lua_register(ls, "des_encrypt", LUA_C_des_encrypt);
  lua_register(ls, "des_decrypt", LUA_C_des_decrypt);

  lua_register(ls, "base64_encode", LUA_C_base64_encode);
  lua_register(ls, "base64_decode", LUA_C_base64_decode);

  lua_register(ls, "sha", LUA_C_sha);

  lua_register(ls, "hmac", LUA_C_hmac);

  lua_pop(ls, lua_gettop(ls));
  }