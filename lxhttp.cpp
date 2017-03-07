#include "xlualib.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#pragma comment(lib, "curl")

#include <map>
#include <vector>
#include <regex>

using std::string;
using std::map;
using std::regex;
using std::smatch;
using std::vector;

//////////////////////////////////////////////////////////////////////////
static size_t xhttp_header_callback(void* ptr, size_t size, size_t nmemb, void* stream)
  {
  const size_t len = size * nmemb;
  map<string, string>& mp = *(map<string, string>*)stream;

  string header((const char*)ptr, len);
  smatch sm;
  regex re("([^:]+):\\s*(.*?)\\r\\n");
  if(regex_match(header, sm, re))
    {
    mp[sm[1]] = sm[2];
    }

  return size * nmemb;
  }

static size_t xhttp_body_callback(void* ptr, size_t size, size_t nmemb, void* stream)
  {
  const size_t len = size * nmemb;
  string& str = *(string*)stream;
  str.append((const char*)ptr, len);
  return size * nmemb;
  }

static int LUA_C_xhttp(lua_State* ls)
  {
  //取URL
  const char* url = luaL_checkstring(ls, 1);

  //初始化CURL
  CURL* curl = curl_easy_init();
  if(curl == nullptr)
    {
    lua_pushstring(ls, "xhttp - curl_easy_init失败");
    return lua_error(ls);
    }

  //初始化header组并预先删除Expect项
  struct curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Expect:");

  vector<string> header;

  size_t connect_time_out = 20000;    //默认连接超时
  size_t time_out = 10000;            //默认超时
  bool post = false;                  //当配置data项存在时，为post请求
  string data;                        //post数据
  string proxy;

  const int top = lua_gettop(ls);

  //如果提供配置则读取之
  if(!lua_isnoneornil(ls, 2))
    {
    lua_settop(ls, top);

    //配置必须为表
    luaL_checktype(ls, 2, LUA_TTABLE);

    //配置可能存在的连接超时
    lua_pushstring(ls, "connect_time_out");
    lua_gettable(ls, 2);
    connect_time_out = luaL_optnumber(ls, -1, connect_time_out);
    lua_settop(ls, top);

    //配置可能存在的超时
    lua_pushstring(ls, "time_out");
    lua_gettable(ls, 2);
    time_out = luaL_optnumber(ls, -1, time_out);
    lua_settop(ls, top);

    //配置可能存在的代理
    lua_pushstring(ls, "proxy");
    lua_gettable(ls, 2);
    proxy.assign(luaL_optstring(ls, -1, ""));
    lua_settop(ls, top);

    //data项存在时，获取之
    lua_pushstring(ls, "data");
    lua_gettable(ls, 2);
    if(!lua_isnil(ls, -1))
      {
      size_t l = 0;
      const char* buf = luaL_checklstring(ls, -1, &l);
      post = true;
      data.append(buf, l);
      }
    lua_settop(ls, top);

    //读取可能存在的header，header必须是表
    lua_pushstring(ls, "header");
    lua_gettable(ls, 2);
    if(!lua_isnil(ls, -1))
      {
      luaL_checktype(ls, -1, LUA_TTABLE);
      lua_pushnil(ls);                    //第一个键
      while(lua_next(ls, -2) != 0)
        {
        lua_pushvalue(ls, -2);            //复制键到栈顶，此时栈：键、值、键
        lua_insert(ls, -2);               //把键放在值前，此时栈：键、键、值
        lua_pushstring(ls, ": ");         //此时栈：键、键、值、": "
        lua_insert(ls, -2);               //位置调整，此时栈：键、键、": "、值
        lua_concat(ls, 3);                //组织，出栈，此时栈：键、"xxxxx"
        const string ss(lua_tostring(ls, -1));
        header.push_back(ss);
        lua_pop(ls, 1);                   //保留键，此时栈：键
        }
      }
    lua_settop(ls, top);
    }

  string repbody;
  map<string, string> repheader;

  //URL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  //超时
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connect_time_out);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, time_out);
  //代理
  if(!proxy.empty())
    {
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    }
  //响应头
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, xhttp_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &repheader);
  //响应体
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, xhttp_body_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &repbody);
  //压入请求头
  for(const auto& ss : header)
    {
    headers = curl_slist_append(headers, ss.c_str());
    }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  //如果是post请求，则设置post
  if(post)
    {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }
  //错误细节展示
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

  //运行之
  CURLcode res = curl_easy_perform(curl);
  //立即删除请求头
  curl_slist_free_all(headers);

  if(res != CURLE_OK)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "xhttp访问出错[" << res << "] : " << curl_easy_strerror(res)
                   ).c_str());
    curl_easy_cleanup(curl);
    return lua_error(ls);
    }

  long code = 0;
  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
  if(res != CURLE_OK)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "xhttp返回出错[" << res << "] : " << curl_easy_strerror(res)
                   ).c_str());
    curl_easy_cleanup(curl);
    return lua_error(ls);
    }

  curl_easy_cleanup(curl);

  //返回值入栈
  lua_pushnumber(ls, code);
  lua_newtable(ls);
  for(const auto& v : repheader)
    {
    lua_pushlstring(ls, v.first.c_str(), v.first.size());
    lua_pushlstring(ls, v.second.c_str(), v.second.size());
    lua_settable(ls, -3);
    }
  lua_pushlstring(ls, repbody.c_str(), repbody.size());

  return 3;
  }

//////////////////////////////////////////////////////////////////////////
ADD_XLUALIB_REGISTER(xhttp)
  {
  lua_pop(ls, lua_gettop(ls));

  lua_register(ls, "xhttp", LUA_C_xhttp);

  lua_pop(ls, lua_gettop(ls));
  }