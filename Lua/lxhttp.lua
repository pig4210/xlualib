--[=======[
-------- -------- -------- --------
             xhttp操作
-------- -------- -------- --------
]=======]

--[=======[
●
    int response_code, table response_headers, string response_body
              xhttp                     (
                                        string url
                                        [,
                                        table options
                                        ]
                                        );
                                                                          [-1|2, +3, v]
        --进行一次http访问
        --options表可以设置如下参数(注意小写名称)：
          {
          int       connect_time_out;             --连接超时，毫秒计，默认20000
          int       time_out;                     --访问超时，毫秒计，默认10000
          string    proxy;                        --代理
                                                    此项存在且不为空时，设置代理
          string    data;                         --post数据
                                                    此项存在时，http访问为post。否则默认为get
          table     header;                       --访问头
          }
        --访问头、响应头表以[键名] = 值形式组表

        ex:
          local c, h, b = xhttp("http://www.hj032.cn");
          for k, v in pairs(h) do
            xlog("key:" .. k, "value:" .. v);
          end

          local c, h, b = xhttp("http://www.xxxx.xx",
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
]=======]