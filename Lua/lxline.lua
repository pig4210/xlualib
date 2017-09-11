--[=======[
-------- -------- -------- --------
             xline����
-------- -------- -------- --------
]=======]
--[=======[
��
    xline     xline:new                 ( );                                [-0, +1, -]
        --xline��Ҫ5.3�����ϵ�string.pack��string.unpack֧�֡��Ͱ汾�������޸�Դ�����֮
        --�ṩxlineʱ������xline״̬������
        --δ�ṩ״̬ʱ��Ĭ��С�ˣ�ͷ��С2 byte��������ͷ���������β0
        --xline������˵����
          line          ��������(string)
          net_flag      ���ݴ������(boolean)��true��� falseС�ˣ�Ĭ��С��
                        0x1234���д�룬����Ϊ"\x12\x34"��С��д�룬����Ϊ"\x34\x12"
          head_size     ���ݴ���ͷ��С(int)��1byte 2word 4dword��Ĭ��2
                        "AA"��Ϊlineд��ʱ������Ϊ"\x02\x00\x41\x41"
          head_self     ���ݴ���ͷ�Ƿ����ͷ��С(boolean)
                        "AA"��Ϊline���Ұ���ͷ��Сд��ʱ������Ϊ"\x04\x00\x41\x41"
          deal_zero_end ���ݴ����β0
                        д���ַ���ʱ������д���β0
                        ��ȡ�ַ��������������β0
          nets          ����net_flag����Ϊ���">"��С��"<"
]=======]
if string.pack and string.unpack then

xline = {};
xline.__index = xline;

function xline:new( )
  local nline = {};
  self = self or xline;

  setmetatable(nline, self);
  self.__index = self;

  nline.line            = self.line         or    "";     --��������
  nline.net_flag        = self.net_flag     or    false;  --����˳��
  nline.head_size       = self.head_size    or    2;      --ͷ��С
  nline.head_self       = self.head_self    or    false;  --�Ƿ����ͷ
  nline.deal_zero_end   = self.deal_zero_end or   false;  --�������β0

  if nline.net_flag then
    nline.nets = ">";
  else
    nline.nets = "<";
  end

  return nline;
end

--[=======[
��
    xline     xline:newline             ( [string] );                     [-0|1, +1, -]
        --ָ���������ݣ���ʼ��Ϊxline
]=======]
function xline:newline( str )
  local nline = self:new();
  nline.line = str or "";
  return nline;
end

-------- -------- -------- --------
--[=======[
��
    ...       xline:pick                ( string fmt );                     [-1, +?, e]
        --ָ����ȡ��ʽ����ȡ���ݲ��޸Ĵ�������
]=======]
function xline:pick( fmt )
  local v = { string.unpack(self.nets .. fmt, self.line) };
  self.line = self.line:sub(table.unpack(v, #v, #v));
  return table.unpack(v, 1, #v - 1);
end

--[=======[
    int       xline:get_byte            ( );  --��ȡһ��byte                [-0, +1, e]
    int       xline:get_word            ( );  --��ȡһ��word                [-0, +1, e]
    int       xline:get_dword           ( );  --��ȡһ��dword               [-0, +1, e]
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
function xline:get_qword()
  return self:pick("I8");
end

--[=======[
    int       xline:get_varint          ( );  --��ȡһ��varint              [-0, +1, e]
]=======]
function xline:get_varint()
  local i, size = getvarint( self.line );
  self.line = self.line:sub( size );
  return i;
end

--[=======[
    int head_size, int real_size
              xline:get_head            ( );                                [-0, +2, e]
        --��ȡ����ͷ����������ͷֵ����ʵ����ͷֵ(��ȥ���ܰ���������ͷ��С)
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
        --��ȡָ����С���ݣ������µ�xline��ͬʱ����״̬
        --���size <= 0��δָ���������µ�xline��ԭʼxline�������������������1
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
        --��ȡ������ͷ��һ������
        --���������µ�xline���Լ�����ͷԭʼ��С
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
        --��ȡָ�������ַ��������ַ������ʹ�С��
        --����δ�ṩʱ����ȡ��0��β���ַ���
        --���ʹ�Сδ�ṩʱ��Ĭ��Ϊ1����ascii�ַ���
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
        --��ȡָ������ascii�ַ���
    string    xline:get_unicode_str     ( [int size] );                   [-0|1, +1, e]
        --��ȡָ������unicode�ַ���
]=======]
function xline:get_ascii_str( size )
  return self:get_str( size, 1 );
end
function xline:get_unicode_str( size )
  return self:get_str( size, 2 );
end
--[=======[
    string    xline:get_head_ascii      ( [int size] );                   [-0|1, +1, e]
        --��ȡ��ͷ��ascii�ַ���
    string    xline:get_head_unicode    ( [int size] );                   [-0|1, +1, e]
        --��ȡ��ͷ��unicode�ַ���
]=======]
function xline:get_head_ascii()
  local size, rsize = self:get_head();
  return self:get_ascii_str( rsize );
end
function xline:get_head_unicode()
  local size, rsize = self:get_head();
  return self:get_unicode_str( rsize );
end
--[=======[
    string    xline:get_vhead_str       ( );                                [-0, +1, e]
        --��ȡ��ͷ���ַ���
]=======]
function xline:get_vhead_str()
  local size, usize = self:get_varint();
  return self:getstr( size, 1 );
end

-------- -------- -------- --------
--[=======[
��
    xline     xline:clear               ( );                                [-0, +1, -]
        --�����������
    xline     xline:assign              ( xline );                          [-1, +1, c]
        --����һxline�ж�ȡ��������
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
        --ָ��д���ʽ��д������
]=======]
function xline:push( fmt, ... )
  self.line = self.line .. string.pack( self.nets .. fmt,  ... );
  return self;
end
--[=======[
    xline     xline:set_byte            ( int v );      --д��һ��byte      [-1, +1, c]
    xline     xline:set_word            ( int v );      --д��һ��word      [-1, +1, c]
    xline     xline:set_dword           ( int v );      --д��һ��dword     [-1, +1, c]
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
function xline:set_qword( v )
  return self:push("I8", v);
end
--[=======[
    xline     xline:set_varint            ( int v );      --д��һ��varin   [-1, +1, c]
]=======]
function xline:set_varint( v )
  self.line = self.line .. tovarint( v );
  return self;
end

--[=======[
    xline     xline:set_head            ( int size );                       [-1, +1, c]
        --д������ͷ���Զ�����״̬��������ͷ��С
]=======]
function xline:set_head( size )
  if self.head_self then
    size = size + self.head_size;
  end
  return self:push("I" .. self.head_size, size);
end
--[=======[
    xline     xline:set_line            ( xline );  --д��xline������       [-1, +1, c]
]=======]
function xline:set_line( nline )
  self.line = self.line .. nline.line;
  return self;
end
--[=======[
    xline     xline:set_head_line       ( xline );  --д�������ͷxline���� [-1, +1, c]
]=======]
function xline:set_head_line( nline )
  self:set_head( #nline.line );
  self:set_line( nline );
  return self;
end
--[=======[
    xline     xline:set_str             ( string str, int type_size );      [-2, +1, c]
        --д���������ݣ��Զ�����״̬�����β0
]=======]
function xline:set_str( str, type_size )
  self.line = self.line .. str;
  if self.deal_zero_end then
    self.line = self.line .. string.rep("\0", type_size);
  end
  return self;
end
--[=======[
    xline     xline:set_ascii_str       ( string str ); --д��ascii         [-1, +1, c]
    xline     xline:set_unicode_str     ( string str ); --д��unicode       [-1, +1, c]
]=======]
function xline:set_ascii_str( str )
  return self:set_str(str, 1);
end

function xline:set_unicode_str( str )
  return self:set_str(str, 2);
end
--[=======[
    xline     xline:set_head_ascii      ( string str ); --д���ͷ��ascii   [-1, +1, c]
    xline     xline:set_head_unicode    ( string str ); --д���ͷ��unicode [-1, +1, c]
]=======]
function xline:set_head_ascii( str )
  self:set_head(#str);
  return self:set_ascii_str(str);
end
function xline:set_head_unicode( str )
  self:set_head(#str);
  return self:set_unicode_str(str);
end
--[=======[
    xline     xline:set_vhead_str       ( string str ); --д���ͷ���ַ���  [-1, +1, c]
]=======]
function xline:set_vhead_str( str )
  self:set_varint( #str );
  return self:set_str( str, 1 );
end

-------- -------- -------- --------
--[=======[
��
    xline.gb    = xline.get_byte;
    xline.gw    = xline.get_word;
    xline.gd    = xline.get_dword;
    xline.gv    = xline.get_varint;
    xline.gh    = xline.get_head;
    xline.gl    = xline.get_line;
    xline.ghl   = xline.get_head_line;
    xline.gs    = xline.get_str;
    xline.ga    = xline.get_ascii_str;
    xline.gu    = xline.get_unicode_str;
    xline.gha   = xline.get_head_ascii;
    xline.ghu   = xline.get_head_unicode;
    xline.gvs   = xline.get_vhead_str;

    xline.cl    = xline.clear;
    xline.as    = xline.assign;

    xline.sb    = xline.set_byte;
    xline.sw    = xline.set_word;
    xline.sd    = xline.set_dword;
    xline.sv    = xline.set_varint;
    xline.sh    = xline.set_head;
    xline.sl    = xline.set_line;
    xline.shl   = xline.set_head_line;
    xline.ss    = xline.set_str;
    xline.sa    = xline.set_ascii_str;
    xline.su    = xline.set_unicode_str;
    xline.sha   = xline.set_head_ascii;
    xline.shu   = xline.set_head_unicode;
    xline.svs   = xline.set_vhead_str;
]=======]
-------- -------- -------- --------
xline.gb    = xline.get_byte;
xline.gw    = xline.get_word;
xline.gd    = xline.get_dword;
xline.gv    = xline.get_varint;
xline.gq    = xline.get_qword;
xline.gh    = xline.get_head;
xline.gl    = xline.get_line;
xline.ghl   = xline.get_head_line;
xline.gs    = xline.get_str;
xline.ga    = xline.get_ascii_str;
xline.gu    = xline.get_unicode_str;
xline.gha   = xline.get_head_ascii;
xline.ghu   = xline.get_head_unicode;
xline.gvs   = xline.get_vhead_str;

xline.cl    = xline.clear;
xline.as    = xline.assign;

xline.sb    = xline.set_byte;
xline.sw    = xline.set_word;
xline.sd    = xline.set_dword;
xline.sq    = xline.set_qword;
xline.sv    = xline.set_varint;
xline.sh    = xline.set_head;
xline.sl    = xline.set_line;
xline.shl   = xline.set_head_line;
xline.ss    = xline.set_str;
xline.sa    = xline.set_ascii_str;
xline.su    = xline.set_unicode_str;
xline.sha   = xline.set_head_ascii;
xline.shu   = xline.set_head_unicode;
xline.svs   = xline.set_vhead_str;

-------- -------- -------- --------
--[=======[
��
    xline     line;                               --С�ˣ�ͷ��С2 byte��������ͷ���������β0
    xline     netline;                            --��ˣ�ͷ��С2 byte��������ͷ���������β0
]=======]
line = xline;
netline = xline:new(); netline.net_flag = true;

end