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

xlog_level用于控制输出

- "off"       // 屏蔽输出
- "fatal"     // 致命错误，程序无法继续执行
- "error"     // 反映错误，例如一些API的调用失败
- "warn"      // 反映某些需要注意的可能有潜在危险的情况，可能会造成崩溃或逻辑错误之类
- "info"      // 表示程序进程的信息
- "debug"     // 普通的调试信息，这类信息发布时一般不输出
- "trace"     // 最精细的调试信息，多用于定位错误，查看某些变量的值
- "on"        // 全输出（默认）
```
string    xlog_level;
```
]=======]
xlog_level = "on";

local function chk_lvl()
  local lvl = xlog_level:lower();
  if lvl == "off" then
    return 0;
  elseif lvl == "fatal" then
    return 1;
  elseif lvl == "error" then
    return 2;
  elseif lvl == "warn" then
    return 3;
  elseif lvl == "info" then
    return 4;
  elseif lvl == "debug" then
    return 5;
  elseif lvl == "trace" then
    return 6;
  --else if lvl == "on" then
  end
  return 7;
end
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
function xfail( ... )   if chk_lvl( ) < 1 then return; end return xlog( ... ); end
function xerr( ... )    if chk_lvl( ) < 2 then return; end return xlog( ... ); end
function xwarn( ... )   if chk_lvl( ) < 3 then return; end return xlog( ... ); end
function xinfo( ... )   if chk_lvl( ) < 4 then return; end return xlog( ... ); end
function xdbg( ... )    if chk_lvl( ) < 5 then return; end return xlog( ... ); end
function xtrace( ... )  if chk_lvl( ) < 6 then return; end return xlog( ... ); end

string.xlog           = function( ... ) return xlog                        ( ... ); end
string.xfail          = function( ... ) return xfail                       ( ... ); end
string.xerr           = function( ... ) return xerr                        ( ... ); end
string.xwarn          = function( ... ) return xwarn                       ( ... ); end
string.xinfo          = function( ... ) return xinfo                       ( ... ); end
string.xdbg           = function( ... ) return xdbg                        ( ... ); end
string.xtrace         = function( ... ) return xtrace                      ( ... ); end