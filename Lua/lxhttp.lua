--[=======[
---- ---- ---- ----

## xhttp

- 访问错误时，抛出错误

```
/*
options表可以设置如下参数(注意小写名称)：
  {
  ["connect_time_out"]  = number;   // 连接超时，毫秒计，默认20000，即20s
  ["time_out"]          = string;   // 访问超时，毫秒计，默认10000，即10s
  ["proxy"]             = string;   // 代理地址，如："127.0.0.1:8888"
                                    // 此项存在且不为空时，设置代理
  ["data"]              = string;   // post数据
                                    // 此项存在时，http访问为post。否则默认为get
  ["verbose"]           = bool;     // 细节展示，默认false不展示
  ["header"]            = table;    // http head。以  [键名] = 值  形式组表
  }

示例代码：

  local c, h, b = xhttp("http://www.qq.com");
  for k, v in pairs( h ) do
    xlog( "key:" .. k, "value:" .. v );
  end

  local c, h, b = xhttp("http://www.qq.com",
                        {
                        connect_time_out = 10000,
                        time_out = 500,
                        proxy = "127.0.0.1:8080",
                        data = "post data",
                        header =
                          {
                          xxx = "xxxx";
                          }
                        }
                       );
*/
number response_code, table response_headers, string response_body
          xhttp                     ( string url, table options = {} );
```
]=======]