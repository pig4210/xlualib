--[=======[
-------- -------- -------- --------
          serialcomm操作
-------- -------- -------- --------
]=======]

--[=======[
●
    string[]  serialcomm.gets           ( );                                [-0, +1, v]
        --获取串行通讯口列表
        --返回表如下：
          {
          string serialcomm_1 ( "COM#" ),
          string serialcomm_2,
          ...
          }

    SerailComm serialcomm.raw_open      ( string SerialCommName );        [-1, +1, c|v]
        --指定串口名称，打开串口
        --返回打开成功的串口对象
        --此函数为原始打开，并未记录已打开的串口对象，无法通过serialcomm.close自动释放，建议使用serialcomm.open函数
]=======]

--[=======[
●
    SerialComm[] serialcomm.opened;
        --保存使用serialcomm.open打开的串口对象，以便查询或通过serialcomm.close自动释放
        --开放此表是为了方便查询，用户被禁止修改此表，以免造成serialcomm.close无法自动释放所有已打开的串口对象
        --表格式如示：{ SerailComm, SerailComm, ... };
        ex:
            for k, v in pairs(serialcomm.opened) do
              xlog(k ,v);
            end
]=======]

local opened = {};

serialcomm.opened = setmetatable(
  {},
  {
  __index = opened;
  __pairs = function( t )
    return next, opened, nil;
  end;
  __newindex = function()
    return error( "serialcomm.opened forbid modify!" );
  end;
  }
  );
--[=======[
    SerailComm serialcomm.open          ( string SerialCommName );        [-1, +1, c|v]
        --指定串口名称，打开串口，同时保存已经打开的对象至表serialcomm.opened
        --使用此函数打开的串口对象可以使用serialcomm.close自动释放
]=======]
function serialcomm.open( name )
  local sc = serialcomm.raw_open( name );
  opened[#opened + 1] = sc;
  return sc;
end
--[=======[
    void      serialcomm.close          ( );                                [-0, +0, c]
        --释放表serialcomm.opened中所有的SerialComm对象并清空表
]=======]
function serialcomm.close( )
  for k, v in pairs(opened) do
    v:close();
  end
  opened = {};
end
--[=======[
●
    private:  SerialComm;
    以下函数为串口对象操作函数
    未作特别说明情况下，串口对象操作失败，将抛出异常，同时!!释放串口对象!!

    string    SerialComm:__tostring     ( );                                [-0, +1, c]
        --输出串口对象信息
        --信息格式如示："SerialComm [COM#] (Closed & N/A | Invalid | ####)"

    void      SerialComm:__gc           ( );                                [-0, +0, c]
        --串口对象释放
        --如果串口对象有效，返回true，否则返回false

    bool      SerialComm:close          ( );                                [-0, +0, c]
        --同SerialComm:__gc，用于主动释放串口对象

    int       SerialComm:baudrate       ( [int newvalue] );             [-0|1, +1, c|v]
        --当指定newvalue时，设置串口对象BaudRate，同时返回旧值
        --未指定newvalue时，仅仅返回当前BaudRate
        --以下几个函数同理
    int       SerialComm:bytesize       ( [int newvalue] );             [-0|1, +1, c|v]
    int       SerialComm:stopbits       ( [int newvalue] );             [-0|1, +1, c|v]
    int       SerialComm:parity         ( [int newvalue] );             [-0|1, +1, c|v]

    int       SerialComm:write          ( string data );                  [-1, +1, c|v]
        --指定写入数据到串口对象，返回写入数据字节数
        --保证将指定数据全部写入串口对象

    string    SerialComm:read           ( );                              [-0, +1, c|v]
        --从串口对象读取数据
        --如果串口无数据，返回空串

    HANDLE    SerialComm:handle         ( );      --返回串口对象句柄        [-0, +1, c]

    bool      SerialComm:isopen         ( );      --检测串口有效性          [-0, +1, c]

    string    SerialComm:name           ( );      --返回串口名称，如："COM#"[-0, +1, c]

    string    SerialComm:type           ( );      --返回串口类型"SerialComm"[-0, +1, c]
]=======]

