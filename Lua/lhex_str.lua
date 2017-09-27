--[=======[
-------- -------- -------- --------
                hex&str操作
-------- -------- -------- --------
]=======]

--[=======[
●
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
      -- 为了简化参数，设计flag
      -- flag >= 0x10的部分被当作prews参数
      -- flag & 1 表示ASCII、flag & 2 表示Unicode、flag & 8 表示UTF8
      -- flag & 4 表示isup == false
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
●
    string    ws2s            ( string s );     --unicode转换为ascii        [-1, +1, c]
    string    s2ws            ( string ws );    --ascii转换为unicode        [-1, +1, c]

    string    string:ws2s     ( );              --unicode转换为ascii        [-0, +1, -]
    string    string:s2ws     ( );              --ascii转换为unicode        [-0, +1, -]
]=======]
string.ws2s           = function( ... ) return ws2s                        ( ... ); end
string.s2ws           = function( ... ) return s2ws                        ( ... ); end


--[=======[
●
    string    utf82ws         ( string utf8 );   --utf8转换为unicode        [-1, +1, c]
    string    ws2utf8         ( string ws );     --unicode转换为utf8        [-1, +1, c]
    string    utf82s          ( string utf8 );   --utf8转换为ascii          [-1, +1, c]
    string    s2utf8          ( string s );      --ascii转换为utf8          [-1, +1, c]
    
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
●
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


--为了兼容之前的xlualib，特意添加

str2hexs = hex2bin;
hex2show = showbin;
hex2str = bin2hex;

string.str2hexs       = function( ... ) return hex2bin                     ( ... ); end
string.hex2show       = function( ... ) return showbin                     ( ... ); end
string.hex2str        = function( ... ) return bin2hex                     ( ... ); end

string.hexs = string.str2hexs;

