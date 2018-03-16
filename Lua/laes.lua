--[=======[
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
]=======]

string.aes_ecb_p7_enc = function( ... ) return aes_ecb_pkcs7padding_encrypt( ... ); end
string.aes_ecb_p7_dec = function( ... ) return aes_ecb_pkcs7padding_decrypt( ... ); end
string.aes_ecb_enc    = function( ... ) return aes_ecb_encrypt             ( ... ); end
string.aes_ecb_dec    = function( ... ) return aes_ecb_decrypt             ( ... ); end

string.aes_cbc_p7_enc = function( ... ) return aes_cbc_pkcs7padding_encrypt( ... ); end
string.aes_cbc_p7_dec = function( ... ) return aes_cbc_pkcs7padding_decrypt( ... ); end
string.aes_cbc_enc    = function( ... ) return aes_cbc_encrypt             ( ... ); end
string.aes_cbc_dec    = function( ... ) return aes_cbc_decrypt             ( ... ); end