--[=======[
-------- -------- -------- --------
             DES
-------- -------- -------- --------
DES加密：使用padding时，自动pkcs7padding。否则不对齐部分不加密，忽略之
DES解密：不对齐部分不解密，忽略之
不使用padding时，默认块大小==8
提供的KEY长度不足8时，以0补足
]=======]

--[=======[
●
    string    des_ecb_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                                [-2|3, +1, c]
    string    des_ecb_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    des.ecb.p7enc             ( ... );                          [-2|3, +1, c]
    string    des.ecb.p7dec             ( ... );                        [-2|3, +1, c|v]
    string    string:des_ecb_p7_enc     ( ... );                          [-1|2, +1, c]
    string    string:des_ecb_p7_dec     ( ... );                        [-1|2, +1, c|v]
    
    string    des_cbc_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3|4, +1, c]
    string    des_cbc_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    des.cbc.p7enc             ( ... );                        [-2|3|4, +1, c]
    string    des.cbc.p7dec             ( ... );                      [-2|3|4, +1, c|v]
    string    string:des_cbc_p7_enc     ( ... );                        [-1|2|3, +1, c]
    string    string:des_cbc_p7_dec     ( ... );                      [-1|2|3, +1, c|v]
    
    string    des_ncbc_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3|4, +1, c]
    string    des_ncbc_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    des.ncbc.p7enc             ( ... );                       [-2|3|4, +1, c]
    string    des.ncbc.p7dec             ( ... );                     [-2|3|4, +1, c|v]
    string    string:des_ncbc_p7_enc     ( ... );                       [-1|2|3, +1, c]
    string    string:des_ncbc_p7_dec     ( ... );                     [-1|2|3, +1, c|v]

    
    string    des_ecb3_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                                [-2|3, +1, c]
    string    des_ecb3_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    des.ecb3.p7enc             ( ... );                         [-2|3, +1, c]
    string    des.ecb3.p7dec             ( ... );                       [-2|3, +1, c|v]
    string    string:des_ecb3_p7_enc     ( ... );                         [-1|2, +1, c]
    string    string:des_ecb3_p7_dec     ( ... );                       [-1|2, +1, c|v]
    -------- -------- -------- -------- 
    string    des_ecb_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                                [-2|3, +1, c]
    string    des_ecb_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    des.ecb.enc            ( ... );                             [-2|3, +1, c]
    string    des.ecb.enc            ( ... );                           [-2|3, +1, c|v]
    string    string:des_ecb_enc     ( ... );                             [-1|2, +1, c]
    string    string:des_ecb_dec     ( ... );                           [-1|2, +1, c|v]
    
    string    des_cbc_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3|4, +1, c]
    string    des_cbc_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    des.cbc.enc            ( ... );                           [-2|3|4, +1, c]
    string    des.cbc.dec            ( ... );                         [-2|3|4, +1, c|v]
    string    string:des_cbc_enc     ( ... );                           [-1|2|4, +1, c]
    string    string:des_cbc_dec     ( ... );                         [-1|2|4, +1, c|v]
    
    string    des_ncbc_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3|4, +1, c]
    string    des_ncbc_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 8
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    des.ncbc.enc            ( ... );                          [-2|3|4, +1, c]
    string    des.ncbc.dec            ( ... );                        [-2|3|4, +1, c|v]
    string    string:des_ncbc_enc     ( ... );                          [-1|2|3, +1, c]
    string    string:des_ncbc_dec     ( ... );                        [-1|2|3, +1, c|v]

    string    des_ecb3_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                                [-2|3, +1, c]
    string    des_ecb3_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 8
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    des.ecb3.enc            ( ... );                            [-2|3, +1, c]
    string    des.ecb3.enc            ( ... );                          [-2|3, +1, c|v]
    string    string:des_ecb3_enc     ( ... );                            [-1|2, +1, c]
    string    string:des_ecb3_dec     ( ... );                          [-1|2, +1, c|v]

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