--[=======[
-------- -------- -------- --------
                zlib操作
-------- -------- -------- --------
]=======]

--[=======[
●
    string    zlib_compress   ( string ); --zlib数据压缩                  [-1, +1, c|v]
    string    zlib_uncompress ( string ); --zlib数据解压缩                [-1, +1, c|v]
    string    gzip_compress   ( string ); --gzip数据压缩                  [-1, +1, c|v]
    string    gzip_uncompress ( string ); --gzip数据解压缩                [-1, +1, c|v]

    string    string:zcp      ( );        --zlib数据压缩                    [-0, +1, v]
    string    string:zup      ( );        --zlib数据解压缩                  [-0, +1, v]
    string    string:gzcp     ( );        --gzip数据压缩                    [-0, +1, v]
    string    string:gzup     ( );        --gzip数据解压缩                  [-0, +1, v]
]=======]
string.zcp            = function( ... ) return zlib_compress               ( ... ); end
string.zup            = function( ... ) return zlib_uncompress             ( ... ); end

string.gzcp           = function( ... ) return gzip_compress               ( ... ); end
string.gzup           = function( ... ) return gzip_uncompress             ( ... ); end