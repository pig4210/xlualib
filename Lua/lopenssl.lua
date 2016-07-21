--[=======[
-------- -------- -------- --------
                openssl操作
-------- -------- -------- --------
]=======]

--[=======[
●  private:  RsaKey;
    void      RsaKey:__gc               ( );                                [-0, +0, c]
    string    RsaKey:__tostring         ( );                                [-0, +1, c]
        --返回"RsaKey*:####"
]=======]
--[=======[
●
    RsaKey    rsa_open_public_key       ( string filename );                [-1, +1, v]
    RsaKey    rsa_set_public_key        ( string rsakey );                  [-1, +1, v]

    RsaKey    rsa_open_private_key      ( string filename );                [-1, +1, v]
    RsaKey    rsa_set_private_key       ( string rsakey );                  [-1, +1, v]
    
    string    rsa_public_encrypt        ( string data, RsaKey key );      [-2, +1, c|v]
    string    rsa_private_encrypt       ( string data, RsaKey key );      [-2, +1, c|e]

    string    rsa_public_decrypt        ( string data, RsaKey key );      [-2, +1, c|e]
    string    rsa_private_decrypt       ( string data, RsaKey key );      [-2, +1, c|e]
]=======]


--[=======[
●
    RsaKey    rsa.pub.open              ( string filename );                [-1, +1, v]
    RsaKey    rsa.pub.set               ( string rsakey );                  [-1, +1, v]
    string    rsa.pub.enc               ( string data, RsaKey key );      [-2, +1, c|e]
    string    rsa.pub.dec               ( string data, RsaKey key );      [-2, +1, c|e]

    RsaKey    rsa.prv.open              ( string filename );                [-1, +1, v]
    RsaKey    rsa.prv.set               ( string rsakey );                  [-1, +1, v]
    string    rsa.prv.enc               ( string data, RsaKey key );      [-2, +1, c|e]
    string    rsa.prv.dec               ( string data, RsaKey key );      [-2, +1, c|e]
    
    string    string:rsa_pub_enc        ( RsaKey key );                   [-1, +1, c|e]
    string    string:rsa_pub_dec        ( RsaKey key );                   [-1, +1, c|e]
    string    string:rsa_prv_enc        ( RsaKey key );                   [-1, +1, c|e]
    string    string:rsa_prv_dec        ( RsaKey key );                   [-1, +1, c|e]
]=======]

rsa = {};
rsa.pub = {};
rsa.prv = {};

rsa.pub.open          = function( ... ) return rsa_open_public_key         ( ... ); end
rsa.pub.set           = function( ... ) return rsa_set_public_key          ( ... ); end
rsa.pub.enc           = function( ... ) return rsa_public_encrypt          ( ... ); end
rsa.pub.dec           = function( ... ) return rsa_public_decrypt          ( ... ); end
                                                                           
rsa.prv.open          = function( ... ) return rsa_open_private_key        ( ... ); end
rsa.prv.set           = function( ... ) return rsa_set_private_key         ( ... ); end
rsa.prv.enc           = function( ... ) return rsa_private_encrypt         ( ... ); end
rsa.prv.dec           = function( ... ) return rsa_private_decrypt         ( ... ); end
                                                                           
string.rsa_pub_enc    = function( ... ) return rsa_public_encrypt          ( ... ); end
string.rsa_pub_dec    = function( ... ) return rsa_public_decrypt          ( ... ); end
string.rsa_prv_enc    = function( ... ) return rsa_private_encrypt         ( ... ); end
string.rsa_prv_dec    = function( ... ) return rsa_private_decrypt         ( ... ); end

--[=======[
    string    base64_encode             (
                                        string    data
                                        [,
                                        bool      newline         = false
                                        ]
                                        );                                [-1|2, +1, c]
    string    base64_decode             (
                                        string    data
                                        [,
                                        bool      newline         = false
                                        ]
                                        );                                [-1|2, +1, c]
    
    string    string:b64_enc            ( [bool newline = false] );       [-0|1, +1, c]
    string    string:b64_dec            ( [bool newline = false] );       [-0|1, +1, c]
]=======]
string.b64_enc        = function( ... ) return base64_encode               ( ... ); end
string.b64_dec        = function( ... ) return base64_decode               ( ... ); end

--[=======[
    string    sha                       (
                                        string    data
                                        [,
                                        int       algo            = 256
                                        ]
                                        );                                [-1|2, +1, c]
        --sha支持的algo有256/512/1/0/224/384
    string    string:sha                ( [int algo] );                   [-0|1, +1, c]

    --以下是SHA便捷函数组
    string    sha0                      ( string  data );                   [-1, +1, c]
    string    sha1                      ( string  data );                   [-1, +1, c]
    string    sha224                    ( string  data );                   [-1, +1, c]
    string    sha256                    ( string  data );                   [-1, +1, c]
    string    sha384                    ( string  data );                   [-1, +1, c]
    string    sha512                    ( string  data );                   [-1, +1, c]

    string    string:sha0               ( );                                [-0, +1, c]
    string    string:sha1               ( );                                [-0, +1, c]
    string    string:sha224             ( );                                [-0, +1, c]
    string    string:sha256             ( );                                [-0, +1, c]
    string    string:sha384             ( );                                [-0, +1, c]
    string    string:sha512             ( );                                [-0, +1, c]
]=======]
string.sha            = function( ... ) return sha                         ( ... ); end

function sha0( data )         return sha( data, 0 );    end
function sha1( data )         return sha( data, 1 );    end
function sha224( data )       return sha( data, 224 );  end
function sha256( data )       return sha( data, 256 );  end
function sha384( data )       return sha( data, 384 );  end
function sha512( data )       return sha( data, 512 );  end

string.sha0           = function( ... ) return sha0                        ( ... ); end
string.sha1           = function( ... ) return sha1                        ( ... ); end
string.sha224         = function( ... ) return sha224                      ( ... ); end
string.sha256         = function( ... ) return sha256                      ( ... ); end
string.sha384         = function( ... ) return sha384                      ( ... ); end
string.sha512         = function( ... ) return sha512                      ( ... ); end

--[=======[
    string    hmac                      (
                                        string    data,
                                        string    key,
                                        string    algo
                                        );                                  [-3, +1, c]
        --hmac支持的algo有"sha512/sha256/sha1/md5/sha224/sha384/sha"
    string    string:hmac               ( string key, string algo );        [-2, +1, c]
]=======]
string.hmac           = function( ... ) return hmac                        ( ... ); end