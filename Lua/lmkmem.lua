--[=======[
-------- -------- -------- --------
            mem操作
-------- -------- -------- --------
]=======]

--[=======[
●
    string    readmem         ( void* lpmem, size_t size );                 [-2, +1, c]
                                                --读取指定内存位置指定长度数据
    
    --以下函数需要5.3及以上的string.unpack支持。低版本请自行修改源码添加之
    int       mkb             ( void* lpmem );  --读取有符号byte值，小端  [-1, +1, c|e]
    int       mkB             ( void* lpmem );  --读取无符号byte值，小端  [-1, +1, c|e]
    int       mkbb            ( void* lpmem );  --读取有符号byte值，大端  [-1, +1, c|e]
    int       mkBB            ( void* lpmem );  --读取无符号byte值，大端  [-1, +1, c|e]

    int       mkw             ( void* lpmem );  --读取有符号word值，小端  [-1, +1, c|e]
    int       mkW             ( void* lpmem );  --读取无符号word值，小端  [-1, +1, c|e]
    int       mkww            ( void* lpmem );  --读取有符号word值，大端  [-1, +1, c|e]
    int       mkWW            ( void* lpmem );  --读取无符号word值，大端  [-1, +1, c|e]
    
    int       mkd             ( void* lpmem );  --读取有符号dword值，小端 [-1, +1, c|e]
    int       mkD             ( void* lpmem );  --读取无符号dword值，小端 [-1, +1, c|e]
    int       mkdd            ( void* lpmem );  --读取有符号dword值，大端 [-1, +1, c|e]
    int       mkDD            ( void* lpmem );  --读取无符号dword值，大端 [-1, +1, c|e]
    
    float     mkf             ( void* lpmem );  --读取float值，小端       [-1, +1, c|e]
    double    mkF             ( void* lpmem );  --读取double值，小端      [-1, +1, c|e]
    float     mkff            ( void* lpmem );  --读取float值，大端       [-1, +1, c|e]
    double    mkFF            ( void* lpmem );  --读取double值，大端      [-1, +1, c|e]
]=======]
if string.unpack then

function mkb( lp )
  return string.unpack("<i1", readmem(lp, 1));
end

function mkB( lp )
  return string.unpack("<I1", readmem(lp, 1));
end

function mkbb( lp )
  return string.unpack(">i1", readmem(lp, 1));
end

function mkBB( lp )
  return string.unpack(">I1", readmem(lp, 1));
end

function mkw( lp )
  return string.unpack("<i2", readmem(lp, 2));
end

function mkW( lp )
  return string.unpack("<I2", readmem(lp, 2));
end

function mkww( lp )
  return string.unpack(">i2", readmem(lp, 2));
end

function mkWW( lp )
  return string.unpack(">I2", readmem(lp, 2));
end

function mkd( lp )
  return string.unpack("<i4", readmem(lp, 4));
end

function mkD( lp )
  return string.unpack("<I4", readmem(lp, 4));
end

function mkdd( lp )
  return string.unpack(">i4", readmem(lp, 4));
end

function mkDD( lp )
  return string.unpack(">I4", readmem(lp, 4));
end

function mkf( lp )
  return string.unpack("<f", readmem(lp, string.packsize("<f")));
end

function mkF( lp )
  return string.unpack("<d", readmem(lp, string.packsize("<d")));
end

function mkff( lp )
  return string.unpack(">f", readmem(lp, string.packsize(">f")));
end

function mkFF( lp )
  return string.unpack(">d", readmem(lp, string.packsize(">d")));
end

end