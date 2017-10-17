--[=======[
-------- -------- -------- --------
          algorithm操作
-------- -------- -------- --------
]=======]

--[=======[
●
    int       xrand           ( [int mod = 0] );                          [-0|1, +1, -]
]=======]

--[=======[
●
    string    strxor          ( string data, int|string xor );              [-2, +1, c]
    string    string:xor      ( int|string xor );                           [-1, +1, c]
      --xor因子可以为字符串原始值如"\x12\x34"
      --当xor因子为数值时，将转换成小端序的字符串原始值，并去除高位00
      --如0x00001234等价于"\x34\x12"
      --请不要提供0值，因为没有意义，函数会视为空参数
]=======]
string.xor            =  function( ... ) return strxor                     ( ... ); end

--[=======[
●
    string    md5             ( string data );                              [-1, +1, c]
    string    string:md5      ( );                                          [-0, +1, -]
]=======]
string.md5            =  function( ... ) return md5                        ( ... ); end

--[=======[
●
    int       crc16           ( string data );                              [-1, +1, c]
    int       crc32           ( string data );                              [-1, +1, c]
    int       crc64           ( string data );                              [-1, +1, c]
    int       crcccitt        ( string data );                              [-1, +1, c]

    int       string:crc16    ( );                                          [-0, +1, -]
    int       string:crc32    ( );                                          [-0, +1, -]
    int       string:crc64    ( );                                          [-0, +1, -]
    int       string:crcccitt ( );                                          [-0, +1, -]
]=======]
string.crc16          =  function( ... ) return crc16                      ( ... ); end
string.crc32          =  function( ... ) return crc32                      ( ... ); end
string.crc64          =  function( ... ) return crc64                      ( ... ); end
string.crcccitt       =  function( ... ) return crcccitt                   ( ... ); end
--[=======[
●
    string    tovarint        ( int value[, bool signed] );               [-1|2, +1, c]
    int value, int userbytes
              getvarint       ( string data[, bool signed] );             [-1|2, +2, c]
    int value, int userbytes
              string:getvarint( [bool signed] );                          [-0|2, +2, -]
]=======]
string.getvarint      =  function( ... ) return getvarint                  ( ... ); end

--[=======[
●
    string    TeanEncrypt     ( string data, string key );                  [-2, +1, c]
    string    TeanDecrypt     ( string data, string key );                  [-2, +1, c]
    string    TeaEncrypt      (
                              string  data,
                              string  key,
                              int     delta,
                              int     round
                              );                                            [-4, +1, c]
    string    TeaDecrypt      (
                              string  data,
                              string  key,
                              int     delta,
                              int     round
                              );                                            [-4, +1, c]
    string    TeanEncipher    ( string data, string key );                  [-2, +1, c]
    string    TeanDecipher    ( string data, string key );                  [-2, +1, c]
    string    XTeanEncrypt    ( string data, string key );                  [-2, +1, c]
    string    XTeanDecrypt    ( string data, string key );                  [-2, +1, c]
    string    XxTeaEncrypt    ( string data, string key );                  [-2, +1, c]
    string    XxTeaDecrypt    ( string data, string key );                  [-2, +1, c]
    
    string    string:tean_enc ( string key );                               [-1, +1, c]
    string    string:tean_dec ( string key );                               [-1, +1, c]
    string    string:tea_enc  ( string key, int delta, int round );         [-3, +1, c]
    string    string:tea_dec  ( string key, int delta, int round );         [-3, +1, c]
    string    string:tea_enr  ( string key );                               [-1, +1, c]
    string    string:tea_der  ( string key );                               [-1, +1, e]
    string    string:xtean_enc( string key );                               [-1, +1, e]
    string    string:xtean_dec( string key );                               [-1, +1, e]
    string    string:xxtea_enc( string key );                               [-1, +1, e]
    string    string:xxtea_dec( string key );                               [-1, +1, e]
]=======]
string.tean_enc       =  function( ... ) return TeanEncrypt                ( ... ); end
string.tean_dec       =  function( ... ) return TeanDecrypt                ( ... ); end
string.tea_enc        =  function( ... ) return TeaEncrypt                 ( ... ); end
string.tea_dec        =  function( ... ) return TeaDecrypt                 ( ... ); end
string.tea_enr        =  function( ... ) return TeanEncipher               ( ... ); end
string.tea_der        =  function( ... ) return TeanDecipher               ( ... ); end
string.xtean_enc      =  function( ... ) return XTeanEncrypt               ( ... ); end
string.xtean_dec      =  function( ... ) return XTeanDecrypt               ( ... ); end
string.xxtea_enc      =  function( ... ) return XxTeaEncrypt               ( ... ); end
string.xxtea_dec      =  function( ... ) return XxTeaDecrypt               ( ... ); end

--[=======[
●
    string    aes_encrypt     ( string data, string key );                  [-2, +1, c]
    string    aes_decrypt     ( string data, string key );                  [-2, +1, c]
    
    string    string:aes_enc  ( string key );                               [-1, +1, c]
    string    string:aes_dec  ( string key );                               [-1, +1, c]
]=======]
string.aes_enc        =  function( ... ) return aes_encrypt                ( ... ); end
string.aes_dec        =  function( ... ) return aes_decrypt                ( ... ); end

--[=======[
●
    string    des_encrypt     ( string data, string key );                  [-2, +1, c]
    string    des_decrypt     ( string data, string key );                  [-2, +1, c]
    
    string    string:des_enc  ( string key );                               [-1, +1, c]
    string    string:des_dec  ( string key );                               [-1, +1, c]
]=======]
string.des_enc        =  function( ... ) return des_encrypt                ( ... ); end
string.des_dec        =  function( ... ) return des_decrypt                ( ... ); end