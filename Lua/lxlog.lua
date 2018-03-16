--[=======[
---- ---- ---- ----

## xlog

- 无条件输出debugview
- 注意，如果在加载前此函数被预定义，则使用预定义
```
void      xlog            ( ... );
```

- 此函数用于在xlog被替换的情况下，还可选择输出到debugview
```
void      dbgview         ( ... );
```

- 输出控制

```
enum xlog_level_enum =
  {
  off         = 0, // 屏蔽输出
  fatal       = 1, // 致命错误，程序无法继续执行
  error       = 2, // 反映错误，例如一些API的调用失败
  warn        = 3, // 反映某些需要注意的可能有潜在危险的情况，可能会造成崩溃或逻辑错误之类
  info        = 4, // 表示程序进程的信息
  debug       = 5, // 普通的调试信息，这类信息发布时一般不输出
  trace       = 6, // 最精细的调试信息，多用于定位错误，查看某些变量的值
  on          = 7, // 全输出（默认）
  }

number    xlog_level = xlog_level_enum.on;
```
]=======]
xlog_level_enum = setmetatable(
  {
  off         = 0,
  fatal       = 1,
  error       = 2,
  warn        = 3,
  info        = 4,
  debug       = 5,
  trace       = 6,
  on          = 7,
  },
  {
    __newindex = function()
      return error( "ENUM禁止修改" );
    end;
  }
  );

xlog_level = xlog_level_enum.on;
--[=======[
- 根据xlog_level的动态调试等级，决定是否输出信息
- 函数组底层调用xlog输出信息，修改xlog函数能实现信息转向
```
void      xfail           ( ... );
void      xerr            ( ... );
void      xwarn           ( ... );
void      xinfo           ( ... );
void      xdbg            ( ... );
void      xtrace          ( ... );
```
- 将信息输出函数组加入string表
```
void      stirng:xlog     ( ... );
void      stirng:xfail    ( ... );
void      string:xerr     ( ... );
void      string:xwarn    ( ... );
void      string:xinfo    ( ... );
void      string:xdbg     ( ... );
void      string:xtrace   ( ... );
```
]=======]
function xfail( ... )   if xlog_level < 1 then return; end return xlog( ... ); end
function xerr( ... )    if xlog_level < 2 then return; end return xlog( ... ); end
function xwarn( ... )   if xlog_level < 3 then return; end return xlog( ... ); end
function xinfo( ... )   if xlog_level < 4 then return; end return xlog( ... ); end
function xdbg( ... )    if xlog_level < 5 then return; end return xlog( ... ); end
function xtrace( ... )  if xlog_level < 6 then return; end return xlog( ... ); end

string.xlog           = function( ... ) return xlog                        ( ... ); end
string.xfail          = function( ... ) return xfail                       ( ... ); end
string.xerr           = function( ... ) return xerr                        ( ... ); end
string.xwarn          = function( ... ) return xwarn                       ( ... ); end
string.xinfo          = function( ... ) return xinfo                       ( ... ); end
string.xdbg           = function( ... ) return xdbg                        ( ... ); end
string.xtrace         = function( ... ) return xtrace                      ( ... ); end