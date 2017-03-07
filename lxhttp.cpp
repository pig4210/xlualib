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
  //ȡURL
  const char* url = luaL_checkstring(ls, 1);

  //��ʼ��CURL
  CURL* curl = curl_easy_init();
  if(curl == nullptr)
    {
    lua_pushstring(ls, "xhttp - curl_easy_initʧ��");
    return lua_error(ls);
    }

  //��ʼ��header�鲢Ԥ��ɾ��Expect��
  struct curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Expect:");

  vector<string> header;

  size_t connect_time_out = 20000;    //Ĭ�����ӳ�ʱ
  size_t time_out = 10000;            //Ĭ�ϳ�ʱ
  bool post = false;                  //������data�����ʱ��Ϊpost����
  string data;                        //post����
  string proxy;

  const int top = lua_gettop(ls);

  //����ṩ�������ȡ֮
  if(!lua_isnoneornil(ls, 2))
    {
    lua_settop(ls, top);

    //���ñ���Ϊ��
    luaL_checktype(ls, 2, LUA_TTABLE);

    //���ÿ��ܴ��ڵ����ӳ�ʱ
    lua_pushstring(ls, "connect_time_out");
    lua_gettable(ls, 2);
    connect_time_out = luaL_optnumber(ls, -1, connect_time_out);
    lua_settop(ls, top);

    //���ÿ��ܴ��ڵĳ�ʱ
    lua_pushstring(ls, "time_out");
    lua_gettable(ls, 2);
    time_out = luaL_optnumber(ls, -1, time_out);
    lua_settop(ls, top);

    //���ÿ��ܴ��ڵĴ���
    lua_pushstring(ls, "proxy");
    lua_gettable(ls, 2);
    proxy.assign(luaL_optstring(ls, -1, ""));
    lua_settop(ls, top);

    //data�����ʱ����ȡ֮
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

    //��ȡ���ܴ��ڵ�header��header�����Ǳ�
    lua_pushstring(ls, "header");
    lua_gettable(ls, 2);
    if(!lua_isnil(ls, -1))
      {
      luaL_checktype(ls, -1, LUA_TTABLE);
      lua_pushnil(ls);                    //��һ����
      while(lua_next(ls, -2) != 0)
        {
        lua_pushvalue(ls, -2);            //���Ƽ���ջ������ʱջ������ֵ����
        lua_insert(ls, -2);               //�Ѽ�����ֵǰ����ʱջ����������ֵ
        lua_pushstring(ls, ": ");         //��ʱջ����������ֵ��": "
        lua_insert(ls, -2);               //λ�õ�������ʱջ����������": "��ֵ
        lua_concat(ls, 3);                //��֯����ջ����ʱջ������"xxxxx"
        const string ss(lua_tostring(ls, -1));
        header.push_back(ss);
        lua_pop(ls, 1);                   //����������ʱջ����
        }
      }
    lua_settop(ls, top);
    }

  string repbody;
  map<string, string> repheader;

  //URL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  //��ʱ
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connect_time_out);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, time_out);
  //����
  if(!proxy.empty())
    {
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    }
  //��Ӧͷ
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, xhttp_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &repheader);
  //��Ӧ��
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, xhttp_body_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &repbody);
  //ѹ������ͷ
  for(const auto& ss : header)
    {
    headers = curl_slist_append(headers, ss.c_str());
    }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  //�����post����������post
  if(post)
    {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }
  //����ϸ��չʾ
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

  //����֮
  CURLcode res = curl_easy_perform(curl);
  //����ɾ������ͷ
  curl_slist_free_all(headers);

  if(res != CURLE_OK)
    {
    lua_pushstring(ls,
                   (
                   xmsg() << "xhttp���ʳ���[" << res << "] : " << curl_easy_strerror(res)
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
                   xmsg() << "xhttp���س���[" << res << "] : " << curl_easy_strerror(res)
                   ).c_str());
    curl_easy_cleanup(curl);
    return lua_error(ls);
    }

  curl_easy_cleanup(curl);

  //����ֵ��ջ
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