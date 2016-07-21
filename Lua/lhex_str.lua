--[=======[
-------- -------- -------- --------
                hex&str操作
-------- -------- -------- --------
]=======]

--[=======[
●
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
                              int       prews         = 0,
                              string    code          = "u"|"8"|"a",
                              bool      isup          = true
                              ]
                              );                                      [-1|2|3|4, +1, c]
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
                              int       prews         = 0,
                              string    code          = "u"|"8"|"a",
                              bool      isup          = true
                              ]
                              );                                      [-0|1|2|3, +1, c]
    string    string:hex2str  (
                              [
                              bool      isup          = false
                              ]
                              );                                          [-0|1, +1, -]

                              
    string.hexs = string.str2hexs;
    string.shwo = string.hex2show;
]=======]
string.str2hexs       = function( ... ) return str2hexs                    ( ... ); end
string.hex2show       = function( ... ) return hex2show                    ( ... ); end
string.hex2str        = function( ... ) return hex2str                     ( ... ); end

string.hexs           = function( ... ) return str2hexs                    ( ... ); end
string.show           = function( ... ) return hex2show                    ( ... ); end


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