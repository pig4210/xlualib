--[=======[
---- ---- ---- ----

## RSA

- RsaKey操作失败，抛出错误。加解密失败，仅返回空串
- 填充模式：PKCS1Padding

```
private:  RsaKey;

void      RsaKey:__gc               ( );
string    RsaKey:__tostring         ( );  // 返回"RsaKey*:####"

RsaKey    rsa_open_public_key       ( string filename );
RsaKey    rsa.pub.open              ( ... );

RsaKey    rsa_set_public_key        ( string rsakey );
RsaKey    rsa.pub.set               ( ... );

RsaKey    rsa_open_private_key      ( string filename );
RsaKey    rsa.prv.open              ( ... );

RsaKey    rsa_set_private_key       ( string rsakey );
RsaKey    rsa.prv.set               ( ... );

string    rsa_public_encrypt        ( string data, RsaKey key );
string    rsa.pub.enc               ( ... );
string    string:rsa_pub_enc        ( ... );

string    rsa_public_decrypt        ( string data, RsaKey key );
string    rsa.pub.dec               ( ... );
string    string:rsa_pub_dec        ( ... );


string    rsa_private_encrypt       ( string data, RsaKey key );
string    rsa.prv.enc               ( ... );
string    string:rsa_prv_enc        ( ... );

string    rsa_private_decrypt       ( string data, RsaKey key );
string    rsa.prv.dec               ( ... );
string    string:rsa_prv_dec        ( ... );
```
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
---- ---- ---- ----

## Base64

- 加解密失败，返回空串

```
string    base64_encode             ( string data );
string    base64_decode             ( string data );
string    string:b64_enc            ( );
string    string:b64_dec            ( );
```
]=======]
string.b64_enc        = function( ... ) return base64_encode               ( ... ); end
string.b64_dec        = function( ... ) return base64_decode               ( ... ); end
--[=======[
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
]=======]
string.hmac           = function( ... ) return hmac                        ( ... ); end

--[=======[
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
]=======]
string.sha            = function( ... ) return sha                         ( ... ); end

function sha1( data )         return sha( data, 1 );    end
function sha224( data )       return sha( data, 224 );  end
function sha256( data )       return sha( data, 256 );  end
function sha384( data )       return sha( data, 384 );  end
function sha512( data )       return sha( data, 512 );  end

string.sha1           = function( ... ) return sha1                        ( ... ); end
string.sha224         = function( ... ) return sha224                      ( ... ); end
string.sha256         = function( ... ) return sha256                      ( ... ); end
string.sha384         = function( ... ) return sha384                      ( ... ); end
string.sha512         = function( ... ) return sha512                      ( ... ); end
