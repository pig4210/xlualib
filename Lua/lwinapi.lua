--[=======[
-------- -------- -------- --------
            api����
-------- -------- -------- --------
]=======]
--[=======[
��
    void      Sleep           ( [int ms = 0] );                           [-0|1, +0, -]
        --��ͣ�߳�ms���룬ms����Ϊ��
        
    int       GetTickCount    ( );                                          [-0, +1, -]
        --��ȡϵͳ����ʱ�������
        
    --Lua5.3�����޷���ȷʹ�����º���
    HMODULE   GetModuleHandle ( [string mod_name] );                      [-0|1, +1, -]

    HMODULE [, int errcode]
              LoadLibrary     ( string lib_name );                        [-1, +1|2, -]

    bool [, int errocde]
              FreeLibrary     ( HMODULE mode );                           [-1, +1|2, -]

    void*     GetProcAddress  ( HMODULE mode, string name );              [-1, +1|2, -]

]=======]