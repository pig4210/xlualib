--[=======[
-------- -------- -------- --------
             xline操作
-------- -------- -------- --------
]=======]
--[=======[
●
    xline     xline:new                 ( );                                [-0, +1, -]
        --xline需要5.3及以上的string.pack，string.unpack支持。低版本请自行修改源码添加之
        --提供xline时，复制xline状态及内容
        --未提供状态时，默认小端，头大小2 byte，不包含头，不处理结尾0
        --xline数据项说明：
          line          数据内容(string)
          net_flag      数据处理编码(boolean)，true大端 false小端，默认小端
                        0x1234大端写入，内容为"\x12\x34"，小端写入，内容为"\x34\x12"
          head_size     数据处理头大小(int)，1byte 2word 4dword，默认2
                        "AA"作为line写入时，内容为"\x02\x00\x41\x41"
          head_self     数据处理头是否包含头大小(boolean)
                        "AA"作为line，且包含头大小写入时，内容为"\x04\x00\x41\x41"
          deal_zero_end 数据处理结尾0
                        写入字符串时，额外写入结尾0
                        读取字符串，额外读出结尾0
          nets          根据net_flag决定为大端">"或小端"<"
]=======]
if string.pack and string.unpack then

xline = {};
xline.__index = xline;

function xline:new( )
  local nline = {};
  self = self or xline;

  setmetatable(nline, self);
  self.__index = self;

  nline.line            = self.line         or    "";     --数据内容
  nline.net_flag        = self.net_flag     or    false;  --网络顺序
  nline.head_size       = self.head_size    or    2;      --头大小
  nline.head_self       = self.head_self    or    false;  --是否包含头
  nline.deal_zero_end   = self.deal_zero_end or   false;  --不处理结尾0

  if nline.net_flag then
    nline.nets = ">";
  else
    nline.nets = "<";
  end

  return nline;
end

--[=======[
●
    xline     xline:newline             ( [string] );                     [-0|1, +1, -]
        --指定数据内容，初始化为xline
]=======]
function xline:newline( str )
  local nline = self:new();
  nline.line = str or "";
  return nline;
end

-------- -------- -------- --------
--[=======[
●
    ...       xline:pick                ( string fmt );                     [-1, +?, e]
        --指定读取格式，读取数据并修改处理索引
]=======]
function xline:pick( fmt )
  local v = { string.unpack(self.nets .. fmt, self.line) };
  self.line = self.line:sub(table.unpack(v, #v, #v));
  return table.unpack(v, 1, #v - 1);
end

--[=======[
    int       xline:get_byte            ( );  --读取一个byte                [-0, +1, e]
    int       xline:get_word            ( );  --读取一个word                [-0, +1, e]
    int       xline:get_dword           ( );  --读取一个dword               [-0, +1, e]
]=======]
function xline:get_byte()
  return self:pick("B");
end
function xline:get_word()
  return self:pick("H");
end
function xline:get_dword()
  return self:pick("L");
end

--[=======[
    int head_size, int real_size
              xline:get_head            ( );                                [-0, +2, e]
        --读取数据头，返回数据头值、真实数据头值(减去可能包含的数据头大小)
]=======]
function xline:get_head()
  local size = self:pick("I" .. self.head_size);
  if self.head_self then
    return size, size - self.head_size;
  end
  return size, size;
end
--[=======[
    xline     xline:get_line            ( [int size] );                   [-0|1, +1, e]
        --读取指定大小数据，生成新的xline，同时复制状态
        --如果size <= 0或未指定，复制新的xline，原始xline数据内容清除，索引置1
]=======]
function xline:get_line( size )
  local nline = self:new( );
  if size == nil or size <= 0 then
    nline.line = self.line;
    self.line = "";
  else
    nline.line = self:pick("c" .. size);
  end
  return nline;
end
--[=======[
    xline newline, int rawsize
              xline:get_head_line       ( );                                [-0, +2, e]
        --读取带数据头的一组数据
        --返回生成新的xline，以及数据头原始大小
]=======]
function xline:get_head_line()
  local size, rsize = self:get_head();
  return self:get_line(rsize), size;
end
--[=======[
    string    xline:get_str             (
                                        [
                                        int       size,
                                        int       type_size = 1
                                        ]
                                        );                              [-0|1|2, +1, e]
        --读取指定长度字符串，以字符串类型大小计
        --长度未提供时，提取以0结尾的字符串
        --类型大小未提供时，默认为1，即ascii字符串
]=======]
function xline:get_str( size, type_size )
  type_size = type_size or 1;
  size = size or math.huge;

  local need_break = true;
  if size ~= math.huge then
    need_break = false;
    size = size * type_size;
  end

  local str = "";
  local ends = string.rep('\0', type_size);
  while size > 0 do
    local c = self:pick("c" .. type_size);
    size = size - type_size;
    if need_break and c == ends then
      if not self.deal_zero_end then
        self.line = self.line .. c;
      end
      break;
    end
    str = str .. c;
  end
  return str;
end
--[=======[
    string    xline:get_ascii_str       ( [int size] );                   [-0|1, +1, e]
        --读取指定长度ascii字符串
    string    xline:get_unicode_str     ( [int size] );                   [-0|1, +1, e]
        --读取指定长度unicode字符串
]=======]
function xline:get_ascii_str( size )
  return self:get_str( size, 1 );
end
function xline:get_unicode_str( size )
  return self:get_str( size, 2 );
end
--[=======[
    string    xline:get_head_ascii      ( [int size] );                   [-0|1, +1, e]
        --读取带头的ascii字符串
    string    xline:get_head_unicode    ( [int size] );                   [-0|1, +1, e]
        --读取带头的unicode字符串
]=======]
function xline:get_head_ascii()
  local size, rsize = self:get_head();
  return self:get_ascii_str( rsize );
end
function xline:get_head_unicode()
  local size, rsize = self:get_head();
  return self:get_unicode_str( rsize );
end

-------- -------- -------- --------
--[=======[
●
    xline     xline:clear               ( );                                [-0, +1, -]
        --清除数据内容
    xline     xline:assign              ( xline );                          [-1, +1, c]
        --从另一xline中读取数据内容
]=======]
function xline:clear()
  self.line = "";
  return self;
end
function xline:assign( nline )
  self.line = nline.line;
  return self;
end
--[=======[
    xline     xline:push                ( stirng fmt, ... );                [-?, +1, c]
        --指定写入格式，写入数据
]=======]
function xline:push( fmt, ... )
  self.line = self.line .. string.pack( self.nets .. fmt,  ... );
  return self;
end
--[=======[
    xline     xline:set_byte            ( int v );      --写入一个byte      [-1, +1, c]
    xline     xline:set_word            ( int v );      --写入一个word      [-1, +1, c]
    xline     xline:set_dword           ( int v );      --写入一个dword     [-1, +1, c]
]=======]
function xline:set_byte( v )
  return self:push("B", v);
end
function xline:set_word( v )
  return self:push("H", v);
end
function xline:set_dword( v )
  return self:push("L", v);
end
--[=======[
    xline     xline:set_head            ( int size );                       [-1, +1, c]
        --写入数据头，自动根据状态加入数据头大小
]=======]
function xline:set_head( size )
  if self.head_self then
    size = size + self.head_size;
  end
  return self:push("I" .. self.head_size, size);
end
--[=======[
    xline     xline:set_line            ( xline );  --写入xline的内容       [-1, +1, c]
]=======]
function xline:set_line( nline )
  self.line = self.line .. nline.line;
  return self;
end
--[=======[
    xline     xline:set_head_line       ( xline );  --写入带数据头xline内容 [-1, +1, c]
]=======]
function xline:set_head_line( nline )
  self:set_head( #nline.line );
  self:set_line( nline );
  return self;
end
--[=======[
    xline     xline:set_str             ( string str, int type_size );      [-2, +1, c]
        --写入数据内容，自动根据状态加入结尾0
]=======]
function xline:set_str( str, type_size )
  self.line = self.line .. str;
  if self.deal_zero_end then
    self.line = self.line .. string.rep("\0", type_size);
  end
  return self;
end
--[=======[
    xline     xline:set_ascii_str       ( string str ); --写入ascii         [-1, +1, c]
    xline     xline:set_unicode_str     ( string str ); --写入unicode       [-1, +1, c]
]=======]
function xline:set_ascii_str( str )
  return self:set_str(str, 1);
end

function xline:set_unicode_str( str )
  return self:set_str(str, 2);
end
--[=======[
    xline     xline:set_head_ascii      ( string str ); --写入带头的ascii   [-1, +1, c]
    xline     xline:set_head_unicode    ( string str ); --写入带头的unicode [-1, +1, c]
]=======]
function xline:set_head_ascii( str )
  self:set_head(#str);
  return self:set_ascii_str(str);
end
function xline:set_head_unicode( str )
  self:set_head(#str);
  return self:set_unicode_str(str);
end

-------- -------- -------- --------
--[=======[
●
    xline.gb    = xline.get_byte;
    xline.gw    = xline.get_word;
    xline.gd    = xline.get_dword;
    xline.gh    = xline.get_head;
    xline.gl    = xline.get_line;
    xline.ghl   = xline.get_head_line;
    xline.gs    = xline.get_str;
    xline.ga    = xline.get_ascii_str;
    xline.gu    = xline.get_unicode_str;
    xline.gha   = xline.get_head_ascii;
    xline.ghu   = xline.get_head_unicode;

    xline.cl    = xline.clear;
    xline.as    = xline.assign;

    xline.sb    = xline.set_byte;
    xline.sw    = xline.set_word;
    xline.sd    = xline.set_dword;
    xline.sh    = xline.set_head;
    xline.sl    = xline.set_line;
    xline.shl   = xline.set_head_line;
    xline.ss    = xline.set_str;
    xline.sa    = xline.set_ascii_str;
    xline.su    = xline.set_unicode_str;
    xline.sha   = xline.set_head_ascii;
    xline.shu   = xline.set_head_unicode;
]=======]
-------- -------- -------- --------
xline.gb    = xline.get_byte;
xline.gw    = xline.get_word;
xline.gd    = xline.get_dword;
xline.gh    = xline.get_head;
xline.gl    = xline.get_line;
xline.ghl   = xline.get_head_line;
xline.gs    = xline.get_str;
xline.ga    = xline.get_ascii_str;
xline.gu    = xline.get_unicode_str;
xline.gha   = xline.get_head_ascii;
xline.ghu   = xline.get_head_unicode;

xline.cl    = xline.clear;
xline.as    = xline.assign;

xline.sb    = xline.set_byte;
xline.sw    = xline.set_word;
xline.sd    = xline.set_dword;
xline.sh    = xline.set_head;
xline.sl    = xline.set_line;
xline.shl   = xline.set_head_line;
xline.ss    = xline.set_str;
xline.sa    = xline.set_ascii_str;
xline.su    = xline.set_unicode_str;
xline.sha   = xline.set_head_ascii;
xline.shu   = xline.set_head_unicode;

-------- -------- -------- --------
--[=======[
●
    xline     line;                               --小端，头大小2 byte，不包含头，不处理结尾0
    xline     netline;                            --大端，头大小2 byte，不包含头，不处理结尾0
]=======]
line = xline;
netline = xline:new(); netline.net_flag = true;

end