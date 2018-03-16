# xlua

---- ---- ---- ----

由于Lua没有强制类型，扩展函数以伪代码形式展示

注意到所有简化函数定义为

```
functionXX            = function( ... ) return functionX                   ( ... ); end 
```
而不定义为

```
functionXX            = functionX;
```
是考虑当`functionX`被修改时，`functionXX`能随之改变。此为记

---- ---- ---- ----

## AES

- AES数据块大小 16 byte 。不对齐部分不处理，忽略之
- 提供的KEY长度不足 16/24/32 byte时，以\x00补足（保证set key不出错）
- 提供的向量长度不足 16 byte时，以\x00补足

### AES/ECB/PKCS7Padding

```
string    aes_ecb_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:aes_ecb_p7_enc     ( ... );

string    aes_ecb_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:aes_ecb_p7_dec     ( ... );
```

### AES/CBC/PKCS7Padding

```
string    aes_cbc_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                                    );
string    string:aes_cbc_p7_enc     ( ... );

string    aes_cbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                                    );
string    string:aes_cbc_p7_dec     ( ... );
```

### AES/ECB/NoPadding

```
string    aes_ecb_encrypt           ( string data, string key );
string    aes_ecb_decrypt           ( string data, string key );
string    string:aes_ecb_enc        ( ... );
string    string:aes_ecb_dec        ( ... );
```

### AES/CBC/NoPadding

```
string    aes_cbc_encrypt           (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                                    );
string    string:aes_cbc_enc        ( ... );

string    aes_cbc_decrypt           (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                                    );
string    string:aes_cbc_dec        ( ... );
```

---- ---- ---- ----

## xrand

```
number    xrand           ( number mod = 0 );
```

---- ---- ---- ----

## strxor

- xor因子可以为字符串原始值如"\x12\x34"
- 当xor因子为数值时，将转换成小端序的字符串原始值，并去除高位00。如0x00001234等价于"\x34\x12"
- 请不要提供0值，因为没有意义，函数会视xor因子为空串
- xor因子为空串时，函数直接返回data

```
string    strxor          ( string data, number|string xor );
string    string:xor      ( ... );
```

---- ---- ---- ----

## MD5

```
string    md5             ( string data );
string    string:md5      ( );
```

---- ---- ---- ----

## CRC

```
number    crc16           ( string data );
number    crc32           ( string data );
number    crc64           ( string data );
number    crcccitt        ( string data );

number    string:crc16    ( );
number    string:crc32    ( );
number    string:crc64    ( );
number    string:crcccitt ( );
```

---- ---- ---- ----

## varint

```
string    tovarint        ( number value, bool signed = false );
```
- 当getvarint返回 usebytes == 0 时，表示操作失败
```
number value, number usebytes
          getvarint       ( string data, bool signed = false );
number value, number usebytes
          string:getvarint( ... );
```

---- ---- ---- ----

## TEA

- 提供的KEY长度不足 16 byte时，以\x00补足

```
string    TeanEncrypt     ( string data, string key );
string    TeanDecrypt     ( string data, string key );
string    TeaEncrypt      (
                          string  data,
                          string  key,
                          number  delta,
                          number  round
                          );
string    TeaDecrypt      (
                          string  data,
                          string  key,
                          number  delta,
                          number  round
                          );
string    TeanEncipher    ( string data, string key );
string    TeanDecipher    ( string data, string key );
string    XTeanEncrypt    ( string data, string key );
string    XTeanDecrypt    ( string data, string key );
string    XxTeaEncrypt    ( string data, string key );
string    XxTeaDecrypt    ( string data, string key );

string    string:tean_enc ( ... );
string    string:tean_dec ( ... );
string    string:tea_enc  ( ... );
string    string:tea_dec  ( ... );
string    string:tean_enr ( ... );
string    string:tean_der ( ... );
string    string:xtean_enc( ... );
string    string:xtean_dec( ... );
string    string:xxtea_enc( ... );
string    string:xxtea_dec( ... );
```

---- ---- ---- ----

## AES RAW

- AES数据块大小 16 byte 。不对齐部分不处理，忽略之
- 提供的KEY长度不足 16 byte时，以\x00补足

```
string    aes_encrypt     ( string data, string key );
string    aes_decrypt     ( string data, string key );

string    string:aes_enc  ( ... );
string    string:aes_dec  ( ... );
```

---- ---- ---- ----

## DES RAW

- DES数据块大小 16 byte 。不对齐部分不处理，忽略之
- 提供的KEY长度不足 8 byte时，以\x00补足

```
string    des_encrypt     ( string data, string key );
string    des_decrypt     ( string data, string key );

string    string:des_enc  ( string key );
string    string:des_dec  ( string key );
```

---- ---- ---- ----

## HEX & BIN

### hex2bin

```
string    hex2bin         (
                          string   hex,
                          bool     errexit       = false,
                          bool     errbreak      = false
                          );
string    string:hex2bin  ( ... );
string    string.bins     ( ... );
```

### bin2hex

```
string    bin2hex         ( string data, bool isup = false );
string    string:bin2hex  ( ... );
```

### showbin

为了简化参数，设计flag

- flag & 1 表示显示编码为ASCII。（编码不冲突，优先顺序如示）
- flag & 2 表示显示编码为Unicode
- flag & 8 表示显示编码为UTF8
- flag & 4 表示显示hex为小写，默认hex大写显示
- flag >= 0x10的部分被当作prews参数，如0x20表示前缀2个空格

```
string    showbin         ( string data, number flag = 1 );
string    string:showbin  ( ... );
string    string:show     ( ... );
```

---- ---- ---- ----

## ASCII & UCS2

```
string    ws2s            ( string s );
string    string:ws2s     ( );

string    s2ws            ( string ws );
string    string:s2ws     ( );
```

---- ---- ---- ----

## UTF8

```
string    utf82ws         ( string utf8 );
string    string:utf82ws  ( );

string    ws2utf8         ( string ws );
string    string:ws2utf8  ( );

string    utf82s          ( string utf8 );
string    string:utf82s   ( );

string    s2utf8          ( string s );
string    string:s2utf8   ( );
```

---- ---- ---- ----

## FILE

### readfile

- 读文件失败时，返回nil, 错误信息

```
string    readfile        ( string filename, string mode = "rb" );
string    string:read     ( ... );
```

### writefile

- 写文件失败时，返回错误信息

```
void      writefile       ( string data, string filename, string mode = "wb" );
void      string:write    ( ... );
```

---- ---- ---- ----

## BLOWFISH

- BLOWFISH数据块大小 8 byte 。不对齐部分不处理，忽略之
- 提供的向量长度不足 8 byte时，以\x00补足
- 注意不补充KEY

### BF/ECB/PKCS7Padding

```
string    blowfish_ecb_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:bf_ecb_p7_enc      ( ... );

string    blowfish_ecb_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:bf_ecb_p7_dec      ( ... );
```

### BF/CBC/PKCS7Padding

```
string    blowfish_cbc_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:bf_cbc_p7_enc      ( ... );

string    blowfish_cbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:bf_cbc_p7_dec      ( ... );
```

### BF/ECB/NoPadding

```
string    blowfish_ecb_encrypt      ( string data, string key );
string    blowfish_ecb_decrypt      ( string data, string key );
string    string:bf_ecb_enc         ( ... );
string    string:bf_ecb_dec         ( ... );
```

### BF/CBC/NoPadding

```
string    blowfish_cbc_encrypt      (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:bf_cbc_enc         ( ... );

string    blowfish_cbc_decrypt      (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:bf_cbc_dec         ( ... );
```

---- ---- ---- ----

## DES

- DES数据块大小 8 byte 。不对齐部分不处理，忽略之
- 提供的KEY长度不足 8 byte时，以\x00补足
- 提供的向量长度不足 8 byte时，以\x00补足

### DES/ECB/PKCS7Padding

```
string    des_ecb_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:des_ecb_p7_enc     ( ... );

string    des_ecb_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:des_ecb_p7_dec     ( ... );
```

### DES/CBC/PKCS7Padding

```
string    des_cbc_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_cbc_p7_enc     ( ... );

string    des_cbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_cbc_p7_dec     ( ... );
```

### DES/NCBC/PKCS7Padding

```
string    des_ncbc_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_ncbc_p7_enc    ( ... );

string    des_ncbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_ncbc_p7_dec    ( ... );
```

### 3DES/ECB/PKCS7Padding

- 3DES的KEY输入要求：KEY == K1 .. K2 .. K3
- K1不存在或不足时，以\x00补齐
- K2不存在时，K2 == K1。不足时，以\x00补齐
- K3不存在时，K3 == K1。不足时，以\x00补齐

```
string    des_ecb3_pkcs7padding_encrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:des_ecb3_p7_enc    ( ... );

string    des_ecb3_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    string:des_ecb3_p7_dec    ( ... );
```

### DES/ECB/NoPadding

```
string    des_ecb_encrypt           ( string data, string key );
string    des_ecb_decrypt           ( string data, string key );
string    string:des_ecb_enc        ( ... );
string    string:des_ecb_dec        ( ... );
```

### DES/CBC/NoPadding

```
string    des_cbc_encrypt           (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_cbc_enc        ( ... );

string    des_cbc_decrypt           (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_cbc_dec        ( ... );
```

### DES/NCBC/NoPadding

``` 
string    des_ncbc_encrypt          (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_ncbc_enc     ( ... );

string    des_ncbc_decrypt          (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    string:des_ncbc_dec     ( ... );
```

### 3DES/ECB/NoPadding

```
string    des_ecb3_encrypt        ( string data, string key );
string    string:des_ecb3_enc     ( ... );

string    des_ecb3_decrypt          ( string data, string key );
string    string:des_ecb3_dec     ( ... );
```

---- ---- ---- ----

## MEM

- 所有指针以interger形式传输，而不是采用light userdata形式，是考虑指针运算，格式化输出等便利
- 操作失败，抛出错误

```
string    readmem         ( void* lpmem, number size = 1 );

void      writemem        ( void* lpmem, string writebytes );

void*     newmem          ( number size );  // 以byte计

bool      deletemem       ( void* lpmem );  // 释放由newmem申请的内存

/*
  参数value不存在时，视为读内存操作，成功则返回相应值，失败则抛出错误
  参数value存在时，视为读内存操作，成功则无返回值，失败则抛出错误
*/
          mkb             ( void* lp [, number v] ); // 读/写无符号byte值，小端
          mkB             ( void* lp [, number v] ); // 读/写无符号byte值，大端
          mkbs            ( void* lp [, number v] ); // 读/写有符号byte值，小端
          mkBs            ( void* lp [, number v] ); // 读/写有符号byte值，大端

          mkw             ( void* lp [, number v] ); // 读/写无符号word值，小端
          mkW             ( void* lp [, number v] ); // 读/写无符号word值，大端
          mkws            ( void* lp [, number v] ); // 读/写有符号word值，小端
          mkWs            ( void* lp [, number v] ); // 读/写有符号word值，大端

          mkd             ( void* lp [, number v] ); // 读/写无符号dword值，小端
          mkD             ( void* lp [, number v] ); // 读/写无符号dword值，大端
          mkds            ( void* lp [, number v] ); // 读/写有符号dword值，小端
          mkDs            ( void* lp [, number v] ); // 读/写有符号dword值，大端

          mkq             ( void* lp [, number v] ); // 读/写无符号qword值，小端
          mkQ             ( void* lp [, number v] ); // 读/写无符号qword值，大端
          mkqs            ( void* lp [, number v] ); // 读/写有符号qword值，小端
          mkQs            ( void* lp [, number v] ); // 读/写有word值，大端

          mkf             ( void* lp [, number v] ); // 读/写float值，小端
          mkF             ( void* lp [, number v] ); // 读loat值，大端

          mkdb            ( void* lp [, number v] ); // 读/写double值，小端
          mkDBs           ( void* lp [, number v] ); // 读/写double值，大端

number    bswap           ( number v, number size = 4|8 ); // 指定翻转数据
number    bswap_byte      ( number v );
number    bswap_word      ( number v );
number    bswap_dword     ( number v );
number    bswap_qword     ( number v );
```

---- ---- ---- ----

## HOOK

- 操作失败，抛出错误

```
void*     hook            (
                          void*     hookmem,
                          number    hooksize,
                          string    data_descibe,
                          string    len_descibe,
                          bool      logfirst
                          );
void*     hook            (
                          void*     hookmem,
                          string    data_descibe,
                          string    len_descibe,
                          bool      calltable_offset,
                          bool      logfirst
                          );
void      unhook          ( void*   node ); // 当unhook不给node参数时，卸载全部hook
```

---- ---- ---- ----

## RSA

- RsaKey操作失败，抛出错误。加解密失败，仅返回空串
- 填充模式：PKCS1Padding

```
private:  RsaKey;

void      RsaKey:__gc               ( );
string    RsaKey:__tostring         ( );  // 返回"RsaKey*:####"

RsaKey    rsa_open_public_key       ( string filename );
RsaKey    rsa_open_private_key      ( string filename );
RsaKey    rsa_set_public_key        ( string rsakey );
RsaKey    rsa_set_private_key       ( string rsakey );


string    rsa_public_encrypt        ( string data, RsaKey key );
string    rsa_public_decrypt        ( string data, RsaKey key );
string    string:rsa_pub_enc        ( ... );
string    string:rsa_pub_dec        ( ... );


string    rsa_private_encrypt       ( string data, RsaKey key );
string    rsa_private_decrypt       ( string data, RsaKey key );
string    string:rsa_prv_enc        ( ... );
string    string:rsa_prv_dec        ( ... );
```

---- ---- ---- ----

## Base64

- 加解密失败，返回空串

```
string    base64_encode             ( string data );
string    base64_decode             ( string data );
string    string:b64_enc            ( );
string    string:b64_dec            ( );
```

---- ---- ---- ----

## HMAC

- hmac支持的algo有"sha512/sha256/sha1/md5/sha224/sha384"
- algo无视大小写，无法支持或识别时，抛出错误

```
string    hmac                      (
                                    string    data,
                                    string    key,
                                    string    algo
                                    );
string    string:hmac               ( ... );
```

---- ---- ---- ----

## SHA

- SHA支持的algo有：256 / 512 / 1 / 0 / 224 / 384
- algo无法支持或识别时，抛出错误

```
string    sha                       ( string data, number alog = 256 );
string    string:sha                ( ... );

//以下是SHA便捷函数组
string    sha1                      ( string data );
string    sha224                    ( string data );
string    sha256                    ( string data );
string    sha384                    ( string data );
string    sha512                    ( string data );

string    string:sha1               ( );
string    string:sha224             ( );
string    string:sha256             ( );
string    string:sha384             ( );
string    string:sha512             ( );
```

---- ---- ---- ----

## PE

```
table     PE              ( number hmod = nullptr );
table     PE              ( string mod_name );

/*
  返回值解析如下：
  {
  HMODULE                 hmod,
  const IMAGE_DOS_HEADER* dos_head,
  bool                    ispe,
  const IMAGE_NT_HEADERS* pe_head,
  void*                   entry,
  table                   image,
    {
    void*                 start,
    void*                 end,
    size_t                size,
    }
  table                   code,
    {
    void*                 start,
    void*                 end,
    size_t                size,
    }
  }
*/
```

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

以下函数，Server不支持

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

以下函数，Client不支持

- 当不提供timeout时，默认超时值-1，即阻塞直到连接发生
- 当提供timeout(毫秒计)时，阻塞指定时间，直到连接发生或超时返回
- 连接发生时，返回新连接的TCP对象
- 超时返回nil, "timeout"
```
TCP       TCP:accept      ( number timeout == -1 );
```

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

---- ---- ---- ----

## Windows API

```
void      Sleep           ( number ms = 0 );    // 暂停线程ms毫秒，ms允许为空
number    GetTickCount    ( );                  // 获取系统启动时间毫秒数

HMODULE   GetModuleHandle ( string mod_name = "" );
HMODULE   LoadLibrary     ( string lib_name );  // 失败返回nil, errorcode
void      FreeLibrary     ( HMODULE mode );     // 失败返回errorcode
void*     GetProcAddress  ( HMODULE mode, string name );
```

---- ---- ---- ----

## xhttp

- 访问错误时，抛出错误

```
number response_code, table response_headers, string response_body
          xhttp                     ( string url, table options = {} );
```

- options表可以设置如下参数(注意小写名称)：
```
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
```

示例代码：
```
  local c, h, b = xhttp( "http://www.qq.com" );
  for k, v in pairs( h ) do
    xlog( "key:" .. k, "value:" .. v );
  end

  local c, h, b = xhttp( "http://www.qq.com",
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
```

---- ---- ---- ----

## xlog

- 无条件输出debugview
- 注意，如果在加载前此函数被预定义，则使用预定义
```
void      xlog            ( ... );
```

- 此函数用于在xlog被替换的情况下，还可选择输出到debugview
```
void      dbgview         ( ... );
```

- 输出控制

```
enum xlog_level_enum =
  {
  off         = 0, // 屏蔽输出
  fatal       = 1, // 致命错误，程序无法继续执行
  error       = 2, // 反映错误，例如一些API的调用失败
  warn        = 3, // 反映某些需要注意的可能有潜在危险的情况，可能会造成崩溃或逻辑错误之类
  info        = 4, // 表示程序进程的信息
  debug       = 5, // 普通的调试信息，这类信息发布时一般不输出
  trace       = 6, // 最精细的调试信息，多用于定位错误，查看某些变量的值
  on          = 7, // 全输出（默认）
  }

number    xlog_level = xlog_level_enum.on;
```

- 根据xlog_level的动态调试等级，决定是否输出信息
- 函数组底层调用xlog输出信息，修改xlog函数能实现信息转向
```
void      xfail           ( ... );
void      xerr            ( ... );
void      xwarn           ( ... );
void      xinfo           ( ... );
void      xdbg            ( ... );
void      xtrace          ( ... );
```
- 将信息输出函数组加入string表
```
void      stirng:xlog     ( ... );
void      stirng:xfail    ( ... );
void      string:xerr     ( ... );
void      string:xwarn    ( ... );
void      string:xinfo    ( ... );
void      string:xdbg     ( ... );
void      string:xtrace   ( ... );
```

---- ---- ---- ----

## zlib

- 压缩/解压失败，返回"" & 错误码

```
string    zlib_compress   ( string data );
string    zlib_uncompress ( string data );
string    string:zcp      ( );
string    string:zup      ( );
```

---- ---- ---- ----

## gzip

- 压缩/解压失败，返回"" & 错误码 [& 错误码]
- gzip解压时，尝试带head/不带head的gzip解压，以及deflate解压

```
string    gzip_compress   ( string data );
string    gzip_uncompress ( string data );
string    string:gzcp     ( );
string    string:gzup     ( );
```
