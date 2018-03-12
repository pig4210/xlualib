#include <string>

#include "xlualib.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

#pragma comment(lib, "libcrypto")

using std::string;
//////////////////////////////////////////////////////////////////////////
static const char* gk_rsakey = "RsaKey*";

struct RSAKEY_ST
  {
  RSA*    key;        //因为key资源需要回收，所以不能做成lightuserdata
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
  lua_pushfstring(ls, "%s:%p", gk_rsakey, (void*)st->key);
  return 1;
  }

static int LUA_C_rsa_open_public_key(lua_State* ls)
  {
  const char* filename = luaL_checkstring(ls, 1);

  FILE* keyfile = nullptr;
  errno_t en = fopen_s(&keyfile, filename, "r");
  if(0 != en)
    {
    return luaL_error(ls, "打开RSA Public Key[%s]失败 : %d", filename, en);
    }

  RSA* k = PEM_read_RSA_PUBKEY(keyfile, nullptr, nullptr, nullptr);

  if(k == nullptr)
    {
    const auto en = errno;
    fclose(keyfile);
    return luaL_error(ls, "读取RSA Public Key失败 : ", en);
    }

  fclose(keyfile);

  RSAKEY_ST* st = (RSAKEY_ST*)lua_newuserdata(ls, sizeof(*st));
  st->key = k;

  luaL_setmetatable(ls, gk_rsakey);
  return 1;
  }

static int LUA_C_rsa_set_public_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    return luaL_error(ls, "Rsa Public Key不允许为空");
    }

  auto bio = BIO_new_mem_buf(s, l);
  if(bio == nullptr)
    {
    return luaL_error(ls, "建立Rsa Public Key缓存失败");
    }

  RSA* k = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
  BIO_free_all(bio);
  if(k == nullptr)
    {
    return luaL_error(ls, "读取Rsa Public Key缓存失败");
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
    return luaL_error(ls, "打开Rsa Private Key[%s]失败 : %d", filename, en);
    }

  RSA* k = PEM_read_RSAPrivateKey(keyfile, nullptr, nullptr, nullptr);

  if(k == nullptr)
    {
    const auto en = errno;
    fclose(keyfile);
    return luaL_error(ls, "读取Rsa Private Key失败 : %d", en);
    }

  fclose(keyfile);

  RSAKEY_ST* st = (RSAKEY_ST*)lua_newuserdata(ls, sizeof(*st));
  st->key = k;

  luaL_setmetatable(ls, gk_rsakey);
  return 1;
  }

static int LUA_C_rsa_set_private_key(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    return luaL_error(ls, "Rsa Private Key不允许为空");
    }


  auto bio = BIO_new_mem_buf(s, l);
  if(bio == nullptr)
    {
    return luaL_error(ls, "建立Rsa Private Key缓存失败");
    }

  RSA* k = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
  BIO_free_all(bio);
  if(k == nullptr)
    {
    return luaL_error(ls, "读取Rsa Private Key缓存失败");
    }

  RSAKEY_ST* st = (RSAKEY_ST*)lua_newuserdata(ls, sizeof(*st));
  st->key = k;

  luaL_setmetatable(ls, gk_rsakey);
  return 1;
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
      lua_pushstring(ls, "");
      return 1;
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
      lua_pushstring(ls, "");
      return 1;
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
      lua_pushstring(ls, "");
      return 1;
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
      lua_pushstring(ls, "");
      return 1;
      }
    res.append(buf.c_str(), len);

    size += msglen;
    }
  lua_pushlstring(ls, res.c_str(), res.size());
  return 1;
  }

//////////////////////////////////////////////////////////////////////////
static int LUA_C_base64_encode(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }

  bool newline = false;

  do
    {
    BIO* b64 = BIO_new(BIO_f_base64());
    if(!newline)  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO * bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, s, l);
    BIO_flush(b64);
    BUF_MEM * bptr = nullptr;
    BIO_get_mem_ptr(b64, &bptr);

    newline = !newline;

    if(bptr->length != 0)
      {
      lua_pushlstring(ls, bptr->data, bptr->length);
      BIO_free_all(b64);
      return 1;
      }
    BIO_free_all(b64);
    } while(newline);

  lua_pushstring(ls, "");
  return 1;
  }

static int LUA_C_base64_decode(lua_State* ls)
  {
  size_t l = 0;
  const auto s = luaL_checklstring(ls, 1, &l);
  if(l == 0)
    {
    lua_pushstring(ls, "");
    return 1;
    }

  bool newline = false;

  do
    {
    BIO* b64 = BIO_new(BIO_f_base64());
    if(!newline)  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    const size_t maxlen = (l * 6 + 7) / 8;

    unsigned char* buf = new unsigned char[maxlen];

    BIO* bmem = BIO_new_mem_buf((void*)s, l);
    b64 = BIO_push(b64, bmem);

    intptr_t readlen = BIO_read(b64, (void*)buf, maxlen);

    newline = !newline;

    if(readlen > 0)
      {
      lua_pushlstring(ls, (const char*)buf, readlen);
      BIO_free_all(b64);
      delete[] buf;
      return 1;
      }
    BIO_free_all(b64);
    delete[] buf;
    } while(newline);

  lua_pushstring(ls, "");
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
  else
    {
    return luaL_error(ls, "HMAC无法识别的算法标识 : %s", algo);
    }

  unsigned char output[EVP_MAX_MD_SIZE];
  unsigned int output_length = 0;

  HMAC_CTX* ctx = HMAC_CTX_new();
  HMAC_Init_ex(ctx, k, kl, engine, NULL);
  HMAC_Update(ctx, (const unsigned char*)s, l);

  HMAC_Final(ctx, output, &output_length);
  HMAC_CTX_free(ctx);

  lua_pushlstring(ls, (const char*)output, output_length);
  return 1;
  }
//////////////////////////////////////////////////////////////////////////
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
    case 224:
      SHA224(s, l, sha);
      len = SHA224_DIGEST_LENGTH;
      break;
    case 384:
      SHA384(s, l, sha);
      len = SHA384_DIGEST_LENGTH;
      break;
    default:
      return luaL_error(ls, "SHA无法识别的算法标识 : %d", algo);
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

  lua_register(ls, "base64_encode", LUA_C_base64_encode);
  lua_register(ls, "base64_decode", LUA_C_base64_decode);

  lua_register(ls, "hmac", LUA_C_hmac);

  lua_register(ls, "sha", LUA_C_sha);

  lua_pop(ls, lua_gettop(ls));
  }