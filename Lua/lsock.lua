--[=======[
-------- -------- -------- --------
             sock操作
-------- -------- -------- --------
]=======]

--[=======[
●
    Private:  UDP;
        --UDP操作失败将抛出异常，同时!!释放对象!!

    UDP       udp_new         (
                              [
                              string|uint         ip    = "0.0.0.0",
                              string|uint         port  = "0",
                              string|uint         bind_port = "0"
                              ]
                              );                                    [-0|1|2|3, +1, c|v]
        --当ip == "0.0.0.0"且port != "0"时，绑定指定端口
        --当ip != "0.0.0.0"时，默认连接指定IP，此时，启用bind_port为绑定端口
        
    string ip, string port, uint ip, uint port
              UDP:getsockname ( );                                          [-0, +1, c]

    string ip, string port, uint ip, uint port
              UDP:getpeername ( );                                          [-0, +1, c]

    stirng    UDP:type        ( );     --返回"UDP"                          [-0, +1, c]
    
    void      UDP:close       ( );                                          [-0, +0, c]

    void      UDP:__gc        ( );                                          [-0, +0, c]

    string    UDP:__tostring  ( );                                          [-0, +1, c]
        --返回UDP{server/client}:########    local_ip:port >> link_ip:port
        
    UDP       UDP:settimeout  ( int timeout );                              [-1, +1, v]
        --接收延时，毫秒计

    UDP       UDP:broadcast   ( bool set );                                 [-1, +1, v]
        --广播设置

    UDP       UDP:send        (
                              string              data
                              [,
                              string|uint         ip    = "0.0.0.0",
                              string|uint         port  = "0"
                              ]
                              );                                      [-1|2|3, +1, c|v]
        --当不提供ip、port时，默认连接初始化时指定的IP
        
    string data, string ip, string port, uint ip, uint port
              UDP:recv        ( [ int size = 0x800 ] );               [-0|1, +2|5, c|v]
        --当不提供size时，默认提供0x800的接收缓冲区
        --超时返回nil, "timeout"
        --接收缓冲区不足返回nil, "msgsize"
]=======]


--[=======[
●
    Private:  TCP;
        --TCP操作失败将抛出异常，同时!!释放对象!!

    TCP       tcp_new         (
                              string|uint         ip    = "0.0.0.0"
                              string|uint         port  = "0",
                              string|uint         bind_port = "0"
                              );                                          [-3, +1, c|v]
        
    string ip, string port, uint ip, uint port
              TCP:getsockname ( );                                          [-0, +1, c]

    string ip, string port, uint ip, uint port
              TCP:getpeername ( );                                          [-0, +1, c]

    stirng    TCP:type        ( );     --返回"TCP"                          [-0, +1, c]
    
    void      TCP:close       ( );                                          [-0, +0, c]

    void      TCP:__gc        ( );                                          [-0, +0, c]

    string    TCP:__tostring  ( );                                          [-0, +1, c]
        --返回TCP{server/client}:########    local_ip:port >> link_ip:port
        
    TCP       TCP:settimeout  ( int timeout );                              [-1, +1, v]
        --接收延时，毫秒计。Server不支持

    TCP       TCP:broadcast   ( bool set );                                 [-1, +1, v]
        --广播设置。Server不支持

    TCP       TCP:send        ( string data );                            [-1, +1, c|v]
        --发送数据。Server不支持
        
    string    TCP:recv        ( [ int size = 0x800 ] );               [-0|1, +1|2, c|v]
        --当不提供size时，默认提供0x800的接收缓冲区
        --超时返回nil, "timeout"。Server不支持
    TCP       TCP:accept      ( [ int timeout == -1 ]);                 [-0|1, +1|2, v]
        --Client不支持
        --当不提供timeout时，默认超时值-1，即阻塞直到连接发生
        --当提供timeout(毫秒计)时，阻塞指定时间，直到连接发生或超时返回
        --连接发生时，返回新连接的TCP对象
        --超时返回nil, "timeout"
]=======]