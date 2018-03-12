--[=======[
---- ---- ---- ----

## xrand

```
number    xrand           ( number mod = 0 );
```
]=======]

--[=======[
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
]=======]
string.xor            = function( ... ) return strxor                      ( ... ); end

--[=======[
---- ---- ---- ----

## MD5

```
string    md5             ( string data );
string    string:md5      ( );
```
]=======]
string.md5            = function( ... ) return md5                         ( ... ); end

--[=======[
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
]=======]
string.crc16          = function( ... ) return crc16                       ( ... ); end
string.crc32          = function( ... ) return crc32                       ( ... ); end
string.crc64          = function( ... ) return crc64                       ( ... ); end
string.crcccitt       = function( ... ) return crcccitt                    ( ... ); end
--[=======[
---- ---- ---- ----

## varint

```
string    tovarint        ( number value, bool signed = false );

// 当返回 usebytes == 0时，表示操作失败
number value, number usebytes
          getvarint       ( string data, bool signed = false );
number value, number usebytes
          string:getvarint( ... );
```
]=======]
string.getvarint      =  function( ... ) return getvarint                  ( ... ); end

--[=======[
---- ---- ---- ----

## TEA

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
]=======]
string.tean_enc       =  function( ... ) return TeanEncrypt                ( ... ); end
string.tean_dec       =  function( ... ) return TeanDecrypt                ( ... ); end
string.tea_enc        =  function( ... ) return TeaEncrypt                 ( ... ); end
string.tea_dec        =  function( ... ) return TeaDecrypt                 ( ... ); end
string.tean_enr       =  function( ... ) return TeanEncipher               ( ... ); end
string.tean_der       =  function( ... ) return TeanDecipher               ( ... ); end
string.xtean_enc      =  function( ... ) return XTeanEncrypt               ( ... ); end
string.xtean_dec      =  function( ... ) return XTeanDecrypt               ( ... ); end
string.xxtea_enc      =  function( ... ) return XxTeaEncrypt               ( ... ); end
string.xxtea_dec      =  function( ... ) return XxTeaDecrypt               ( ... ); end

--[=======[
---- ---- ---- ----

## AES RAW

```
string    aes_encrypt     ( string data, string key );
string    aes_decrypt     ( string data, string key );

string    string:aes_enc  ( ... );
string    string:aes_dec  ( ... );
```
]=======]
string.aes_enc        =  function( ... ) return aes_encrypt                ( ... ); end
string.aes_dec        =  function( ... ) return aes_decrypt                ( ... ); end

--[=======[
---- ---- ---- ----

## DES RAW

```
string    des_encrypt     ( string data, string key );
string    des_decrypt     ( string data, string key );

string    string:des_enc  ( string key );
string    string:des_dec  ( string key );
```
]=======]
string.des_enc        =  function( ... ) return des_encrypt                ( ... ); end
string.des_dec        =  function( ... ) return des_decrypt                ( ... ); end