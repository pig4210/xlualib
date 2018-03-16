--[=======[
---- ---- ---- ----

## PE

```
table     PE              ( number hmod = nullptr );
table     PE              ( string mod_name );

/*
  返回值解析如下：
  {
  HMODULE                 hmod,
  const IMAGE_DOS_HEADER* dos_head,
  bool                    ispe,
  const IMAGE_NT_HEADERS* pe_head,
  void*                   entry,
  table                   image,
    {
    void*                 start,
    void*                 end,
    size_t                size,
    }
  table                   code,
    {
    void*                 start,
    void*                 end,
    size_t                size,
    }
  }
*/
```
]=======]