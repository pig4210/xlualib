--[=======[
-------- -------- -------- --------
                hex&str²Ù×÷
-------- -------- -------- --------
]=======]

--[=======[
¡ñ
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
]=======]
string.str2hexs       = function( ... ) return str2hexs                    ( ... ); end
string.hex2show       = function( ... ) return hex2show                    ( ... ); end
string.hex2str        = function( ... ) return hex2str                     ( ... ); end