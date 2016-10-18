--[=======[
-------- -------- -------- --------
            mem操作
-------- -------- -------- --------

所有指针以interger形式传输，而不是采用light userdata形式，是考虑指针运算，格式化输出等便利
]=======]

--[=======[
●
    string [, string errmsg]
              readmem         ( void* lpmem, size_t size );               [-2, +1|2, c]
                                                --读取指定内存位置指定长度数据
                                                --无论指定内存位置是否有效都无异常抛出
                                                --读取失败返回空串, 错误消息
    bool [, string errmsg]
              writemem        ( void* lpmem, string writebytes );         [-2, +1|2, c]
                                                --写入指定内存位置指定长度数据
                                                --无论指定内存位置是否有效都无异常抛出
                                                --写失败返回false, 错误消息
    unsigned char*
              newmem          ( size_t size );                              [-1, +1, c]
                                                --指定字节大小，申请内存
                                                --成功则返回内存指针
                                                --size == 0或失败则返回nullptr
    bool      deletemem       ( unsigned char* lpmem );                     [-1, +1, c]
                                                --释放由newmem申请的内存
    
    --以下函数需要5.3及以上的string.unpack支持。低版本请自行修改源码添加之
    int|bool [, string]
              mkb             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号byte值，小端
    int|bool [, string]
              mkB             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号byte值，小端
    int|bool [, string]
              mkbb            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号byte值，大端
    int|bool [, string]
              mkBB            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号byte值，大端

    int|bool [, string]
              mkw             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号word值，小端
    int|bool [, string]
              mkW             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号word值，小端
    int|bool [, string]
              mkww            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号word值，大端
    int|bool [, string]
              mkWW            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号word值，大端
                                                
    int|bool [, string]
              mkd             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号dword值，小端
    int|bool [, string]
              mkD             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号dword值，小端
    int|bool [, string]
              mkdd            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号dword值，大端
    int|bool [, string]
              mkDD            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号dword值，大端
    
    int|bool [, string]
              mkq             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号qword值，小端
    int|bool [, string]
              mkQ             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号qword值，小端
    int|bool [, string]
              mkqq            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写有符号qword值，大端
    int|bool [, string]
              mkQQ            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写无符号qword值，大端
    
    float|bool [, string]
              mkf             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写float值，小端
    double|bool [, string]
              mkF             ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写double值，小端
    float|bool [, string]
              mkff            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写float值，大端
    double|bool [, string]
              mkFF            ( void* lpmem [, int value] );          [-1|2, +1|2, c|e]
                                                --读/写double值，大端
                                                
    int       bswap           ( int value [, int size] );                 [-1|2, +1, c]
                                                --指定翻转数据
    int       bswap_byte      ( int value );                                [-1, +1, c]
    int       bswap_word      ( int value );                                [-1, +1, c]
    int       bswap_dword     ( int value );                                [-1, +1, c]
    int       bswap_qword     ( int value );                                [-1, +1, c]
]=======]
if string.unpack then

local function mkX( net, sign, size, lp, value )
  local fmt;
  if net then
    fmt = ">";
  else
    fmt = "<";
  end
  if sign then
    fmt = fmt .. "i";
  else
    fmt = fmt .. "I";
  end
  fmt = fmt .. size;
  
  if not value then
    return string.unpack( fmt, readmem( lp, size ) );
  end
  value = string.pack( fmt, value );
  return writemem( lp, value );
end

function mkb( lp, value )
  return mkX( false, true, 1, lp, value );
end

function mkB( lp, value )
  return mkX( false, false, 1, lp, value );
end

function mkbb( lp, value )
  return mkX( true, true, 1, lp, value );
end

function mkBB( lp, value )
  return mkX( true, false, 1, lp, value );
end

function mkw( lp, value )
  return mkX( false, true, 2, lp, value );
end

function mkW( lp, value )
  return mkX( false, false, 2, lp, value );
end

function mkww( lp, value )
  return mkX( true, true, 2, lp, value );
end

function mkWW( lp, value )
  return mkX( true, false, 2, lp, value );
end

function mkd( lp, value )
  return mkX( false, true, 4, lp, value );
end

function mkD( lp, value )
  return mkX( false, false, 4, lp, value );
end

function mkdd( lp, value )
  return mkX( true, true, 4, lp, value );
end

function mkDD( lp, value )
  return mkX( true, false, 4, lp, value );
end

function mkq( lp, value )
  return mkX( false, true, 8, lp, value );
end

function mkQ( lp, value )
  return mkX( false, false, 8, lp, value );
end

function mkqq( lp, value )
  return mkX( true, true, 8, lp, value );
end

function mkQQ( lp, value )
  return mkX( true, false, 8, lp, value );
end

function mkf( lp, value )
  if not value then
    return string.unpack( "<f", readmem( lp, string.packsize( "<f" ) ) );
  end
  value = string.pack( "<f", value );
  return writemem( lp, value );
end

function mkF( lp, value )
  if not value then
    return string.unpack( "<d", readmem( lp, string.packsize( "<d" ) ) );
  end
  value = string.pack( "<d", value );
  return writemem( lp, value );
end

function mkff( lp, value )
  if not value then
    return string.unpack( ">f", readmem( lp, string.packsize( ">f" ) ) );
  end
  value = string.pack( ">f", value );
  return writemem( lp, value );
end

function mkFF( lp, value )
  if not value then
    return string.unpack( ">d", readmem( lp, string.packsize( ">d" ) ) );
  end
  value = string.pack( ">d", value );
  return writemem( lp, value );
end


function bswap( value, size )
  local fmt = "=I";
  if size then
    fmt = fmt .. size;
  end
  local ss = string.pack( fmt, value ):reverse();
  return (string.unpack( fmt, ss ));
end

function bswap_byte( value )      return value;             end
function bswap_word( value )      return bswap( value, 2 ); end
function bswap_dword( value )     return bswap( value, 4 ); end
function bswap_qword( value )     return bswap( value, 8 ); end

end


--[=======[
●
    void*     hook            (
                              void*     hookmem,
                              int       hooksize,
                              string    data_descibe,
                              string    len_descibe,
                              bool      docodeend
                              );                                        [-5, +1, c|e|v]
        --Hook2Log 地址
    void*     hook            (
                              void*     hookmem,
                              string    data_descibe,
                              string    len_descibe,
                              bool      calltable_offset,
                              bool      docallend
                              );                                        [-5, +1, c|e|v]
        --Hook2Log 偏移或跳转表

    void      unhook          ( void*   node );                         [-1, +0, c|e|v]
        --当unhook不给node参数时，卸载全部hook
]=======]
function unhook( ... )          return hook( ... ); end