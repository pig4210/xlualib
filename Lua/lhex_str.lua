--[=======[
-------- -------- -------- --------
                hex&str����
-------- -------- -------- --------
]=======]

--[=======[
��
    string    hex2bin         (
                               string   hex
                               [,
                               bool     errexit       = false,
                               bool     errbreak      = false
                               ]
                               );                                       [-1|2|3, +1, c]
    string    showbin         (
                              string    data
                              [,
                              int       flag
                              ]
                              );                                          [-1|2, +1, c]
      -- Ϊ�˼򻯲��������flag
      -- flag >= 0x10�Ĳ��ֱ�����prews����
      -- flag & 1 ��ʾASCII��flag & 2 ��ʾUnicode��flag & 8 ��ʾUTF8
      -- flag & 4 ��ʾisup == false
    string    bin2hex         (
                              string    data
                              [,
                              bool      isup          = false
                              ]
                              );                                          [-1|2, +1, c]

    string    string:hex2bin  (
                              [
                              bool      errexit       = false,
                              bool      errbreak      = false
                              ]
                              );                                       [-0|1|2, +1, -]
    string    string:showbin  (
                              [
                              int       flag
                              ]
                              );                                          [-1|2, +1, c]
    string    string:bin2hex  (
                              [
                              bool      isup          = false
                              ]
                              );                                          [-0|1, +1, -]

                              
    string.bins = string.hex2bin;
    string.show = string.showbin;
]=======]
string.hex2bin        = function( ... ) return hex2bin                    ( ... ); end
string.showbin        = function( ... ) return showbin                    ( ... ); end
string.bin2hex        = function( ... ) return bin2hex                    ( ... ); end

string.bins           = function( ... ) return hex2bin                    ( ... ); end
string.show           = function( ... ) return showbin                    ( ... ); end


--[=======[
��
    string    ws2s            ( string s );     --unicodeת��Ϊascii        [-1, +1, c]
    string    s2ws            ( string ws );    --asciiת��Ϊunicode        [-1, +1, c]

    string    string:ws2s     ( );              --unicodeת��Ϊascii        [-0, +1, -]
    string    string:s2ws     ( );              --asciiת��Ϊunicode        [-0, +1, -]
]=======]
string.ws2s           = function( ... ) return ws2s                        ( ... ); end
string.s2ws           = function( ... ) return s2ws                        ( ... ); end


--[=======[
��
    string    utf82ws         ( string utf8 );   --utf8ת��Ϊunicode        [-1, +1, c]
    string    ws2utf8         ( string ws );     --unicodeת��Ϊutf8        [-1, +1, c]
    string    utf82s          ( string utf8 );   --utf8ת��Ϊascii          [-1, +1, c]
    string    s2utf8          ( string s );      --asciiת��Ϊutf8          [-1, +1, c]
    
    string    string:utf82ws  ( );                                          [-0, +1, -]
    string    string:ws2utf8  ( );                                          [-0, +1, -]
    string    string:utf82s   ( );                                          [-0, +1, -]
    string    string:s2utf8   ( );                                          [-0, +1, -]
]=======]
string.utf82ws        = function( ... ) return utf82ws                     ( ... ); end
string.ws2utf8        = function( ... ) return ws2utf8                     ( ... ); end
string.utf82s         = function( ... ) return utf82s                      ( ... ); end
string.s2utf8         = function( ... ) return s2utf8                      ( ... ); end


--[=======[
��
    string    readfile        (
                              string filename
                              [, string mode = "rb" ]
                              );                                          [-1|2, +1, c]
    void      writefile       (
                              string data,
                              string filename
                              [, string mode = "wb"]
                              );                                          [-2|3, +0, c]
    string    string:read     ( [ string mode = "rb" ] );                 [-0|1, +1, -]
    void      string:write    ( string filename [, string mode = "wb" ] );[-1|2, +0, -]
]=======]
function readfile( filename, mode )
  local file, err = io.open( filename, mode or "rb" );
  if file then
    local data = file:read( "*a" );
    file:close();
    return data;
  end
  return error( err );
end

function writefile( data, filename, mode )
  local file, err = io.open( filename, mode or "wb" );
  if file then
    file:write( data );
    file:close();
    return;
  end
  return error( err );
end

string.read           = function( ... ) return readfile                    ( ... ); end
string.write          = function( ... ) return writefile                   ( ... ); end


--Ϊ�˼���֮ǰ��xlualib���������

str2hexs = hex2bin;
hex2show = showbin;
hex2str = bin2hex;

string.str2hexs       = function( ... ) return hex2bin                     ( ... ); end
string.hex2show       = function( ... ) return showbin                     ( ... ); end
string.hex2str        = function( ... ) return bin2hex                     ( ... ); end

string.hexs = string.str2hexs;

