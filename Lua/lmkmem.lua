--[=======[
---- ---- ---- ----

## MEM

- 所有指针以interger形式传输，而不是采用light userdata形式，是考虑指针运算，格式化输出等便利
- 操作失败，抛出错误

```
string    readmem         ( void* lpmem, number size = 1 );

void      writemem        ( void* lpmem, string writebytes );

void*     newmem          ( number size );  // 以byte计

bool      deletemem       ( void* lpmem );  // 释放由newmem申请的内存

/*
  参数value不存在时，视为读内存操作，成功则返回相应值，失败则抛出错误
  参数value存在时，视为读内存操作，成功则无返回值，失败则抛出错误
*/
          mkb             ( void* lp [, number v] ); // 读/写无符号byte值，小端
          mkB             ( void* lp [, number v] ); // 读/写无符号byte值，大端
          mkbs            ( void* lp [, number v] ); // 读/写有符号byte值，小端
          mkBs            ( void* lp [, number v] ); // 读/写有符号byte值，大端

          mkw             ( void* lp [, number v] ); // 读/写无符号word值，小端
          mkW             ( void* lp [, number v] ); // 读/写无符号word值，大端
          mkws            ( void* lp [, number v] ); // 读/写有符号word值，小端
          mkWs            ( void* lp [, number v] ); // 读/写有符号word值，大端

          mkd             ( void* lp [, number v] ); // 读/写无符号dword值，小端
          mkD             ( void* lp [, number v] ); // 读/写无符号dword值，大端
          mkds            ( void* lp [, number v] ); // 读/写有符号dword值，小端
          mkDs            ( void* lp [, number v] ); // 读/写有符号dword值，大端

          mkq             ( void* lp [, number v] ); // 读/写无符号qword值，小端
          mkQ             ( void* lp [, number v] ); // 读/写无符号qword值，大端
          mkqs            ( void* lp [, number v] ); // 读/写有符号qword值，小端
          mkQs            ( void* lp [, number v] ); // 读/写有word值，大端

          mkf             ( void* lp [, number v] ); // 读/写float值，小端
          mkF             ( void* lp [, number v] ); // 读loat值，大端

          mkdb            ( void* lp [, number v] ); // 读/写double值，小端
          mkDBs           ( void* lp [, number v] ); // 读/写double值，大端

number    bswap           ( number v, number size = 4|8 ); // 指定翻转数据
number    bswap_byte      ( number v );
number    bswap_word      ( number v );
number    bswap_dword     ( number v );
number    bswap_qword     ( number v );
```
]=======]
local function mkX( fmt, lp, value )
  if not value then
    return ( string.unpack( fmt,
      readmem( lp, string.packsize( fmt) )
      ) );
  end

  return writemem( lp, string.pack( fmt, value ) );
end

function mkb  ( lp, value )  return mkX( "<I1", lp, value );  end
function mkB  ( lp, value )  return mkX( ">I1", lp, value );  end
function mkbs ( lp, value )  return mkX( "<i1", lp, value );  end
function mkBs ( lp, value )  return mkX( ">i1", lp, value );  end

function mkw  ( lp, value )  return mkX( "<I2", lp, value );  end
function mkW  ( lp, value )  return mkX( ">I2", lp, value );  end
function mkws ( lp, value )  return mkX( "<i2", lp, value );  end
function mkWs ( lp, value )  return mkX( ">i2", lp, value );  end

function mkd  ( lp, value )  return mkX( "<I4", lp, value );  end
function mkD  ( lp, value )  return mkX( ">I4", lp, value );  end
function mkds ( lp, value )  return mkX( "<i4", lp, value );  end
function mkDs ( lp, value )  return mkX( ">i4", lp, value );  end

function mkq  ( lp, value )  return mkX( "<I8", lp, value );  end
function mkQ  ( lp, value )  return mkX( ">I8", lp, value );  end
function mkqs ( lp, value )  return mkX( "<i8", lp, value );  end
function mkQs ( lp, value )  return mkX( ">i8", lp, value );  end

function mkf  ( lp, value )  return mkX( "<f", lp, value );   end
function mkF  ( lp, value )  return mkX( ">f", lp, value );   end

function mkdb ( lp, value )  return mkX( "<d", lp, value );   end
function mkDBs( lp, value )  return mkX( ">d", lp, value );   end

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
--[=======[
---- ---- ---- ----

## HOOK

- 操作失败，抛出错误

```
void*     hook            (
                          void*     hookmem,
                          number    hooksize,
                          string    data_descibe,
                          string    len_descibe,
                          bool      logfirst
                          );
void*     hook            (
                          void*     hookmem,
                          string    data_descibe,
                          string    len_descibe,
                          bool      calltable_offset,
                          bool      logfirst
                          );
void      unhook          ( void*   node ); // 当unhook不给node参数时，卸载全部hook
```
]=======]
function unhook( ... )          return hook( ... ); end