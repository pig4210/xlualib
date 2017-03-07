--[=======[
-------- -------- -------- --------
                hex&str����
-------- -------- -------- --------
]=======]

--[=======[
��
    string    str2hexs        (
                               string   str
                               [,
                               bool     errexit       = false,
                               bool     errbreak      = false
                               ]
                               );                                       [-1|2|3, +1, c]
    string    hex2show        (
                              string    data
                              [,
                              int       flag
                              ]
                              );                                          [-1|2, +1, c]
      -- Ϊ�˼򻯲��������flag
      -- flag >= 0x10�Ĳ��ֱ�����prews����
      -- flag & 1 ��ʾASCII��flag & 2 ��ʾUnicode��flag & 8 ��ʾUTF8
      -- flag & 4 ��ʾisup == false
    string    hex2str         (
                              string    data
                              [,
                              bool      isup          = false
                              ]
                              );                                          [-1|2, +1, c]

    string    string:str2hexs (
                              [
                              bool      errexit       = false,
                              bool      errbreak      = false
                              ]
                              );                                       [-0|1|2, +1, -]
    string    string:hex2show (
                              [
                              int       flag
                              ]
                              );                                          [-1|2, +1, c]
    string    string:hex2str  (
                              [
                              bool      isup          = false
                              ]
                              );                                          [-0|1, +1, -]

                              
    string.hexs = string.str2hexs;
    string.show = string.hex2show;
]=======]
string.str2hexs       = function( ... ) return str2hexs                    ( ... ); end
string.hex2show       = function( ... ) return hex2show                    ( ... ); end
string.hex2str        = function( ... ) return hex2str                     ( ... ); end

string.hexs           = function( ... ) return str2hexs                    ( ... ); end
string.show           = function( ... ) return hex2show                    ( ... ); end


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