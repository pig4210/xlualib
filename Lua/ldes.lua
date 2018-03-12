--[=======[
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
string    des.ecb.p7enc             ( ... );
string    string:des_ecb_p7_enc     ( ... );

string    des_ecb_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    des.ecb.p7dec             ( ... );
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
string    des.cbc.p7enc             ( ... );
string    string:des_cbc_p7_enc     ( ... );

string    des_cbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    des.cbc.p7dec             ( ... );
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
string    des.ncbc.p7enc            ( ... );
string    string:des_ncbc_p7_enc    ( ... );

string    des_ncbc_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    des.ncbc.p7dec            ( ... );
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
string    des.ecb3.p7enc            ( ... );
string    string:des_ecb3_p7_enc    ( ... );

string    des_ecb3_pkcs7padding_decrypt
                                    (
                                    string data,
                                    string key
                                    );
string    des.ecb3.p7dec            ( ... );
string    string:des_ecb3_p7_dec    ( ... );
```

### DES/ECB/NoPadding

```
string    des_ecb_encrypt           ( string data, string key );
string    des.ecb.enc               ( ... );
string    des.ecb.enc               ( ... );

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
string    des.cbc.enc               ( ... );
string    string:des_cbc_enc        ( ... );

string    des_cbc_decrypt           (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    des.cbc.dec               ( ... );
string    string:des_cbc_dec        ( ... );
```

### DES/NCBC/NoPadding

``` 
string    des_ncbc_encrypt          (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    des.ncbc.enc            ( ... );
string    string:des_ncbc_enc     ( ... );

string    des_ncbc_decrypt          (
                                    string data,
                                    string key,
                                    string ivec = "\0\0\0\0\0\0\0\0"
                                    );
string    des.ncbc.dec            ( ... );
string    string:des_ncbc_dec     ( ... );
```

### 3DES/ECB/NoPadding

```
string    des_ecb3_encrypt          ( string data, string key );
string    des.ecb3.enc            ( ... );
string    string:des_ecb3_enc     ( ... );

string    des_ecb3_decrypt          ( string data, string key );
string    des.ecb3.enc            ( ... );
string    string:des_ecb3_dec     ( ... );
```
]=======]

des = {};

des.ecb = {};
des.cbc = {};
des.ncbc = {};
des.ecb3 = {};

des.ecb.p7enc         = function( ... ) return des_ecb_pkcs7padding_encrypt ( ... );end
des.ecb.p7dec         = function( ... ) return des_ecb_pkcs7padding_decrypt ( ... );end
des.ecb.enc           = function( ... ) return des_ecb_encrypt              ( ... );end
des.ecb.dec           = function( ... ) return des_ecb_decrypt              ( ... );end

des.cbc.p7enc         = function( ... ) return des_cbc_pkcs7padding_encrypt ( ... );end
des.cbc.p7dec         = function( ... ) return des_cbc_pkcs7padding_decrypt ( ... );end
des.cbc.enc           = function( ... ) return des_cbc_encrypt              ( ... );end
des.cbc.dec           = function( ... ) return des_cbc_decrypt              ( ... );end

des.ncbc.p7enc        = function( ... ) return des_ncbc_pkcs7padding_encrypt( ... );end
des.ncbc.p7dec        = function( ... ) return des_ncbc_pkcs7padding_decrypt( ... );end
des.ncbc.enc          = function( ... ) return des_ncbc_encrypt             ( ... );end
des.ncbc.dec          = function( ... ) return des_ncbc_decrypt             ( ... );end

des.ecb3.p7enc        = function( ... ) return des_ecb3_pkcs7padding_encrypt( ... );end
des.ecb3.p7dec        = function( ... ) return des_ecb3_pkcs7padding_decrypt( ... );end
des.ecb3.enc          = function( ... ) return des_ecb3_encrypt             ( ... );end
des.ecb3.dec          = function( ... ) return des_ecb3_decrypt             ( ... );end

string.des_ecb_p7_enc = function( ... ) return des_ecb_pkcs7padding_encrypt ( ... );end
string.des_ecb_p7_dec = function( ... ) return des_ecb_pkcs7padding_decrypt ( ... );end
string.des_ecb_enc    = function( ... ) return des_ecb_encrypt              ( ... );end
string.des_ecb_dec    = function( ... ) return des_ecb_decrypt              ( ... );end

string.des_cbc_p7_enc = function( ... ) return des_cbc_pkcs7padding_encrypt ( ... );end
string.des_cbc_p7_dec = function( ... ) return des_cbc_pkcs7padding_decrypt ( ... );end
string.des_cbc_enc    = function( ... ) return des_cbc_encrypt              ( ... );end
string.des_cbc_dec    = function( ... ) return des_cbc_decrypt              ( ... );end

string.des_ncbc_p7_enc= function( ... ) return des_ncbc_pkcs7padding_encrypt( ... );end
string.des_ncbc_p7_dec= function( ... ) return des_ncbc_pkcs7padding_decrypt( ... );end
string.des_ncbc_enc   = function( ... ) return des_ncbc_encrypt             ( ... );end
string.des_ncbc_dec   = function( ... ) return des_ncbc_decrypt             ( ... );end

string.des_ecb3_p7_enc= function( ... ) return des_ecb3_pkcs7padding_encrypt( ... );end
string.des_ecb3_p7_dec= function( ... ) return des_ecb3_pkcs7padding_decrypt( ... );end
string.des_ecb3_enc   = function( ... ) return des_ecb3_encrypt             ( ... );end
string.des_ecb3_dec   = function( ... ) return des_ecb3_decrypt             ( ... );end