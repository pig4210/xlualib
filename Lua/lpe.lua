--[=======[
-------- -------- -------- --------
              PE操作
-------- -------- -------- --------
]=======]

--[=======[
●
    table     PE   ( HMOUDLE hmod = nullptr | string mod_name );          [-0|1, +1, c]
        --返回
          {
          HMODULE                 hmod,
          const IMAGE_DOS_HEADER* dos_head,
          bool                    ispe,
          const IMAGE_NT_HEADERS* pe_head,
          void*                   entry,
          table                   image =
            {
            void*                 start,
            void*                 end,
            size_t                size,
            },
          table                   code =
            {
            void*                 start,
            void*                 end,
            size_t                size,
            },
          }
        --Lua5.3以下无法正确使用此函数
]=======]