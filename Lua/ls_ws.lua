--[=======[
-------- -------- -------- --------
             s&ws操作
-------- -------- -------- --------
]=======]

--[=======[
●
    string    ws2s            ( string s );     --unicode转换为ascii        [-1, +1, c]
    string    s2ws            ( string ws );    --ascii转换为unicode        [-1, +1, c]

    string    string:ws2s     ( );              --unicode转换为ascii        [-0, +1, -]
    string    string:s2ws     ( );              --ascii转换为unicode        [-0, +1, -]
]=======]
string.ws2s           = function( ... ) return ws2s                        ( ... ); end
string.s2ws           = function( ... ) return s2ws                        ( ... ); end