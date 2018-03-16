--[=======[
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
]=======]

string.bf_ecb_p7_enc = function( ... ) return blowfish_ecb_pkcs7padding_encrypt( ... ); end
string.bf_ecb_p7_dec = function( ... ) return blowfish_ecb_pkcs7padding_decrypt( ... ); end
string.bf_ecb_enc    = function( ... ) return blowfish_ecb_encrypt             ( ... ); end
string.bf_ecb_dec    = function( ... ) return blowfish_ecb_decrypt             ( ... ); end

string.bf_cbc_p7_enc = function( ... ) return blowfish_cbc_pkcs7padding_encrypt( ... ); end
string.bf_cbc_p7_dec = function( ... ) return blowfish_cbc_pkcs7padding_decrypt( ... ); end
string.bf_cbc_enc    = function( ... ) return blowfish_cbc_encrypt             ( ... ); end
string.bf_cbc_dec    = function( ... ) return blowfish_cbc_decrypt             ( ... ); end