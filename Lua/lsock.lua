--[=======[
---- ---- ---- ----

## UDP

- UDP操作失败将抛出异常，同时!!释放对象!!

```
Private:  UDP;
```

- bind_port不为0时，无视ip、port，直接绑定本地端口
- 当ip == "0.0.0.0"且port != "0"时，视为bind_port = port，也绑定指定端口
- 当ip != "0.0.0.0"时，将与指定IP通讯
- 当不绑定本地端口时，默认接收延时5s
- ip为数值时，视为大端序，即0x0100007F表示"127.0.0.1"
```
UDP       udp_new         (
                          string|number   ip    = "0.0.0.0",
                          string|number   port  = "0",
                          string|number   bind_port = "0"
                          );
```

- 获取本地/对端SOCK信息
- 返回IP数值大端序，即0x0100007F表示"127.0.0.1"
- 注意：对象未发送任何数据前，返回的数据可能不正确
```
string ip, string port, number ip, number port
          UDP:getsockname ( );

string ip, string port, number ip, number port
          UDP:getpeername ( );
```

```
stirng    UDP:type        ( );  // 返回对象类型，即"UDP"
void      UDP:close       ( );
void      UDP:__gc        ( );
```

- 返回如示：UDP{server/client}:########    local_ip:port >> remote_ip:port
```
string    UDP:__tostring  ( );
```

- 接收延时，毫秒计（默认取消延时）
```
UDP       UDP:settimeout  ( number timeout = -1 );
```

```
UDP       UDP:broadcast   ( bool set = false );
```

- 当不提供ip、port时，默认连接初始化时指定的IP
```
UDP       UDP:send        (
                          string          data,
                          string|number   ip    = "0.0.0.0",
                          string|number   port  = "0"
                          );
```

- 当不提供size时，默认提供0x800的接收缓冲区
- 超时返回nil, "timeout"
- 接收缓冲区不足返回nil, "msgsize"
- 目标不可达返回nil, "unreachable"
```
string data, string ip, string port, number ip, number port
          UDP:recv        ( number size = 0x800 );
```
]=======]


--[=======[
---- ---- ---- ----

## TCP

- TCP操作失败将抛出异常，同时!!释放对象!!

```
Private:  TCP;
```

- bind_port不为0时，无视ip、port，直接绑定本地端口
- 当ip == "0.0.0.0"且port != "0"时，视为bind_port = port，也绑定指定端口
- 当ip != "0.0.0.0"时，将与指定IP通讯
- 当不绑定本地端口时，默认接收延时5s
- ip为数值时，视为大端序，即0x0100007F表示"127.0.0.1"
- NonBlockConnect设置连接无阻塞模式（默认连接阻塞）
```
TCP       tcp_new         (
                          string|number ip    = "0.0.0.0"
                          string|number port  = "0",
                          string|number bind_port = "0",
                          bool          NonBlockConnect = false
                          );
```

- 获取本地/对端SOCK信息
- 返回IP数值大端序，即0x0100007F表示"127.0.0.1"
```
string ip, string port, number ip, number port
          TCP:getsockname ( );

string ip, string port, number ip, number port
          TCP:getpeername ( );
```

```
stirng    TCP:type        ( );  // 返回对象类型，即"TCP"
void      TCP:close       ( );
void      TCP:__gc        ( );
```

- 返回如示：UDP{server/client}:########    local_ip:port >> remote_ip:port
```
string    TCP:__tostring  ( );
```

- 以下函数，Server不支持
- 接收延时，毫秒计（默认取消延时）
```
TCP       TCP:settimeout  ( int timeout = -1 );
TCP       TCP:broadcast   ( bool set = false );
TCP       TCP:send        ( string data );
```

- 当不提供size时，默认提供0x800的接收缓冲区
- 超时返回nil, "timeout"
```
string    TCP:recv        ( number size = 0x800 );
```

- 检测NonBlockConnect的TCP是否连接成功（成功后，自动设置阻塞）
```
bool      TCP:check       ( );
```

- 以下函数，Client不支持

- 当不提供timeout时，默认超时值-1，即阻塞直到连接发生
- 当提供timeout(毫秒计)时，阻塞指定时间，直到连接发生或超时返回
- 连接发生时，返回新连接的TCP对象
- 超时返回nil, "timeout"
```
TCP       TCP:accept      ( number timeout == -1 );
```
]=======]
--[=======[
---- ---- ---- ----

## transip

```
string    transip         ( number ip, bool bigendian = false ); // 显示示例：127.0.0.1
string    transip         ( string ip, bool bigendian = false ); // 显示示例：0x7F000001
```
---- ---- ---- ----

## dns

- 返回第一个table是ip字符串列表
- 返回第二个table是ip值列表，大端序
- 失败则抛出错误

```
table, table
          dns             ( string host );
```
]=======]

