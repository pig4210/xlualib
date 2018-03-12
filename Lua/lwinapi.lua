--[=======[
---- ---- ---- ----

## Windows API

```
void      Sleep           ( number ms = 0 );    // 暂停线程ms毫秒，ms允许为空
number    GetTickCount    ( );                  // 获取系统启动时间毫秒数
        
// Lua5.3以下无法正确使用以下函数
HMODULE   GetModuleHandle ( string mod_name = "" );
HMODULE   LoadLibrary     ( string lib_name );  // 失败返回nil, errorcode
void      FreeLibrary     ( HMODULE mode );     // 失败返回errorcode
void*     GetProcAddress  ( HMODULE mode, string name );
```
]=======]