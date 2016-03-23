--[=======[
-------- -------- -------- --------
             AES
-------- -------- -------- --------
AES加密：使用padding时，自动pkcs7padding。否则不对齐部分不加密，忽略之
AES解密：不对齐部分不解密，忽略之
不使用padding时，默认块大小==16
提供的KEY长度不足16时，以0补足
]=======]

--[=======[
●
    string    aes_ecb_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    aes_ecb_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    aes.ecb.p7enc             ( ... );                        [-2|3, +1, c|v]
    string    aes.ecb.p7dec             ( ... );                        [-2|3, +1, c|v]
    string    string:aes_ecb_p7_enc     ( ... );                        [-1|2, +1, c|v]
    string    string:aes_ecb_p7_dec     ( ... );                        [-1|2, +1, c|v]
    
    string    aes_cbc_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    aes_cbc_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    aes.cbc.p7enc             ( ... );                      [-2|3|4, +1, c|v]
    string    aes.cbc.p7dec             ( ... );                      [-2|3|4, +1, c|v]
    string    string:aes_cbc_p7_enc     ( ... );                      [-1|2|3, +1, c|v]
    string    string:aes_cbc_p7_dec     ( ... );                      [-1|2|3, +1, c|v]
    
    -------- -------- -------- -------- 
    string    aes_ecb_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    aes_ecb_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    aes.ecb.enc            ( ... );                           [-2|3, +1, c|v]
    string    aes.ecb.enc            ( ... );                           [-2|3, +1, c|v]
    string    string:aes_ecb_enc     ( ... );                           [-1|2, +1, c|v]
    string    string:aes_ecb_dec     ( ... );                           [-1|2, +1, c|v]
    
    string    aes_cbc_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    aes_cbc_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    aes.cbc.enc            ( ... );                         [-2|3|4, +1, c|v]
    string    aes.cbc.dec            ( ... );                         [-2|3|4, +1, c|v]
    string    string:aes_cbc_enc     ( ... );                         [-1|2|4, +1, c|v]
    string    string:aes_cbc_dec     ( ... );                         [-1|2|4, +1, c|v]
    
]=======]

aes = {};

aes.ecb = {};
aes.cbc = {};

aes.ecb.p7enc         = function( ... ) return aes_ecb_pkcs7padding_encrypt( ... ); end
aes.ecb.p7dec         = function( ... ) return aes_ecb_pkcs7padding_decrypt( ... ); end
aes.ecb.enc           = function( ... ) return aes_ecb_encrypt             ( ... ); end
aes.ecb.dec           = function( ... ) return aes_ecb_decrypt             ( ... ); end

aes.cbc.p7enc         = function( ... ) return aes_cbc_pkcs7padding_encrypt( ... ); end
aes.cbc.p7dec         = function( ... ) return aes_cbc_pkcs7padding_decrypt( ... ); end
aes.cbc.enc           = function( ... ) return aes_cbc_encrypt             ( ... ); end
aes.cbc.dec           = function( ... ) return aes_cbc_decrypt             ( ... ); end

string.aes_ecb_p7_enc = function( ... ) return aes_ecb_pkcs7padding_encrypt( ... ); end
string.aes_ecb_p7_dec = function( ... ) return aes_ecb_pkcs7padding_decrypt( ... ); end
string.aes_ecb_enc    = function( ... ) return aes_ecb_encrypt             ( ... ); end
string.aes_ecb_dec    = function( ... ) return aes_ecb_decrypt             ( ... ); end

string.aes_cbc_p7_enc = function( ... ) return aes_cbc_pkcs7padding_encrypt( ... ); end
string.aes_cbc_p7_dec = function( ... ) return aes_cbc_pkcs7padding_decrypt( ... ); end
string.aes_cbc_enc    = function( ... ) return aes_cbc_encrypt             ( ... ); end
string.aes_cbc_dec    = function( ... ) return aes_cbc_decrypt             ( ... ); end