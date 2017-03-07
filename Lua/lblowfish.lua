--[=======[
-------- -------- -------- --------
           BLOWFISH
-------- -------- -------- --------
BLOWFISH���ܣ�ʹ��paddingʱ���Զ�pkcs7padding�����򲻶��벿�ֲ����ܣ�����֮
BLOWFISH���ܣ������벿�ֲ����ܣ�����֮
��ʹ��paddingʱ��Ĭ�Ͽ��С==8
�ṩ��KEY���Ȳ���8ʱ����0����
]=======]

--[=======[
��
    string    blowfish_ecb_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        );                              [-2|3, +1, c|v]
    string    blowfish_ecb_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        );                              [-2|3, +1, c|v]
    string    blowfish.ecb.p7enc        ( ... );                        [-2|3, +1, c|v]
    string    blowfish.ecb.p7dec        ( ... );                        [-2|3, +1, c|v]
    string    string:bf_ecb_p7_enc      ( ... );                        [-1|2, +1, c|v]
    string    string:bf_ecb_p7_dec      ( ... );                        [-1|2, +1, c|v]
    
    string    blowfish_cbc_pkcs7padding_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0"
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    blowfish_cbc_pkcs7padding_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0"
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    blowfish.cbc.p7enc        ( ... );                      [-2|3|4, +1, c|v]
    string    blowfish.cbc.p7dec        ( ... );                      [-2|3|4, +1, c|v]
    string    string:bf_cbc_p7_enc      ( ... );                      [-1|2|3, +1, c|v]
    string    string:bf_cbc_p7_dec      ( ... );                      [-1|2|3, +1, c|v]
    
    -------- -------- -------- -------- 
    string    blowfish_ecb_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 16
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    blowfish_ecb_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        int    block_size = 16
                                        ]
                                        );                              [-2|3, +1, c|v]
    string    blowfish.ecb.enc          ( ... );                        [-2|3, +1, c|v]
    string    blowfish.ecb.enc          ( ... );                        [-2|3, +1, c|v]
    string    string:bf_ecb_enc         ( ... );                        [-1|2, +1, c|v]
    string    string:bf_ecb_dec         ( ... );                        [-1|2, +1, c|v]
    
    string    blowfish_cbc_encrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 16
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    blowfish_cbc_decrypt
                                        (
                                        string data,
                                        string key
                                        [,
                                        string ivec = "\0\0\0\0\0\0\0\0",
                                        int    block_size = 16
                                        ]
                                        );                            [-2|3|4, +1, c|v]
    string    blowfish.cbc.enc          ( ... );                      [-2|3|4, +1, c|v]
    string    blowfish.cbc.dec          ( ... );                      [-2|3|4, +1, c|v]
    string    string:bf_cbc_enc         ( ... );                      [-1|2|4, +1, c|v]
    string    string:bf_cbc_dec         ( ... );                      [-1|2|4, +1, c|v]
    
]=======]

blowfish = {};

blowfish.ecb = {};
blowfish.cbc = {};

blowfish.ecb.p7enc   = function( ... ) return blowfish_ecb_pkcs7padding_encrypt( ... ); end
blowfish.ecb.p7dec   = function( ... ) return blowfish_ecb_pkcs7padding_decrypt( ... ); end
blowfish.ecb.enc     = function( ... ) return blowfish_ecb_encrypt             ( ... ); end
blowfish.ecb.dec     = function( ... ) return blowfish_ecb_decrypt             ( ... ); end

blowfish.cbc.p7enc   = function( ... ) return blowfish_cbc_pkcs7padding_encrypt( ... ); end
blowfish.cbc.p7dec   = function( ... ) return blowfish_cbc_pkcs7padding_decrypt( ... ); end
blowfish.cbc.enc     = function( ... ) return blowfish_cbc_encrypt             ( ... ); end
blowfish.cbc.dec     = function( ... ) return blowfish_cbc_decrypt             ( ... ); end

string.bf_ecb_p7_enc = function( ... ) return blowfish_ecb_pkcs7padding_encrypt( ... ); end
string.bf_ecb_p7_dec = function( ... ) return blowfish_ecb_pkcs7padding_decrypt( ... ); end
string.bf_ecb_enc    = function( ... ) return blowfish_ecb_encrypt             ( ... ); end
string.bf_ecb_dec    = function( ... ) return blowfish_ecb_decrypt             ( ... ); end

string.bf_cbc_p7_enc = function( ... ) return blowfish_cbc_pkcs7padding_encrypt( ... ); end
string.bf_cbc_p7_dec = function( ... ) return blowfish_cbc_pkcs7padding_decrypt( ... ); end
string.bf_cbc_enc    = function( ... ) return blowfish_cbc_encrypt             ( ... ); end
string.bf_cbc_dec    = function( ... ) return blowfish_cbc_decrypt             ( ... ); end