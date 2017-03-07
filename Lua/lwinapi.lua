--[=======[
-------- -------- -------- --------
            api操作
-------- -------- -------- --------
]=======]
--[=======[
●
    void      Sleep           ( [int ms = 0] );                           [-0|1, +0, -]
        --暂停线程ms毫秒，ms允许为空
        
    int       GetTickCount    ( );                                          [-0, +1, -]
        --获取系统启动时间毫秒数
        
    --Lua5.3以下无法正确使用以下函数
    HMODULE   GetModuleHandle ( [string mod_name] );                      [-0|1, +1, -]

    HMODULE [, int errcode]
              LoadLibrary     ( string lib_name );                        [-1, +1|2, -]

    bool [, int errocde]
              FreeLibrary     ( HMODULE mode );                           [-1, +1|2, -]

    void*     GetProcAddress  ( HMODULE mode, string name );              [-1, +1|2, -]

]=======]