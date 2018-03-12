--[=======[
---- ---- ---- ----

## zlib

- 压缩/解压失败，返回nil & 错误码

```
string    zlib_compress   ( string data );
string    zlib_uncompress ( string data );
string    string:zcp      ( );
string    string:zup      ( );
```
]=======]
string.zcp            = function( ... ) return zlib_compress               ( ... ); end
string.zup            = function( ... ) return zlib_uncompress             ( ... ); end
--[=======[
---- ---- ---- ----

## gzip

- 压缩/解压失败，返回nil & 错误信息
- gzip解压时，尝试带head/不带head的gzip解压，以及deflate解压

```
string    gzip_compress   ( string data );
string    gzip_uncompress ( string data );
string    string:gzcp     ( );
string    string:gzup     ( );
```
]=======]
string.gzcp           = function( ... ) return gzip_compress               ( ... ); end
string.gzup           = function( ... ) return gzip_uncompress             ( ... ); end