--[=======[
---- ---- ---- ----

## HEX & BIN

### hex2bin

```
string    hex2bin         (
                          string   hex,
                          bool     errexit       = false,
                          bool     errbreak      = false
                          );
string    string:hex2bin  ( ... );
string    string.bins     ( ... );
```

### bin2hex

```
string    bin2hex         ( string data, bool isup = false );
string    string:bin2hex  ( ... );
```

### showbin

为了简化参数，设计flag

- flag & 1 表示显示编码为ASCII。（编码不冲突，优先顺序如示）
- flag & 2 表示显示编码为Unicode
- flag & 8 表示显示编码为UTF8
- flag & 4 表示显示hex为小写，默认hex大写显示
- flag >= 0x10的部分被当作prews参数，如0x20表示前缀2个空格

```
string    showbin         ( string data, number flag = 1 );
string    string:showbin  ( ... );
string    string:show     ( ... );
```
]=======]
string.hex2bin        = function( ... ) return hex2bin                    ( ... ); end
string.showbin        = function( ... ) return showbin                    ( ... ); end
string.bin2hex        = function( ... ) return bin2hex                    ( ... ); end

string.bins           = function( ... ) return hex2bin                    ( ... ); end
string.show           = function( ... ) return showbin                    ( ... ); end

--[=======[
---- ---- ---- ----

## ASCII & UCS2

```
string    ws2s            ( string s );
string    string:ws2s     ( );

string    s2ws            ( string ws );
string    string:s2ws     ( );
```
]=======]
string.ws2s           = function( ... ) return ws2s                        ( ... ); end
string.s2ws           = function( ... ) return s2ws                        ( ... ); end


--[=======[
---- ---- ---- ----

## UTF8

```
string    utf82ws         ( string utf8 );
string    string:utf82ws  ( );

string    ws2utf8         ( string ws );
string    string:ws2utf8  ( );

string    utf82s          ( string utf8 );
string    string:utf82s   ( );

string    s2utf8          ( string s );
string    string:s2utf8   ( );
```
]=======]
string.utf82ws        = function( ... ) return utf82ws                     ( ... ); end
string.ws2utf8        = function( ... ) return ws2utf8                     ( ... ); end
string.utf82s         = function( ... ) return utf82s                      ( ... ); end
string.s2utf8         = function( ... ) return s2utf8                      ( ... ); end


--[=======[
---- ---- ---- ----

## FILE

### readfile

- 读文件失败时，返回nil, 错误信息

```
string    readfile        ( string filename, string mode = "rb" );
string    string:read     ( ... );
```

### writefile

- 写文件失败时，返回错误信息

```
void      writefile       ( string data, string filename, string mode = "wb" );
void      string:write    ( ... );
```
]=======]
function readfile( filename, mode )
  local file, err = io.open( filename, mode or "rb" );
  if file then
    local data = file:read( "*a" );
    file:close();
    return data;
  end
  return nil, err;
end

function writefile( data, filename, mode )
  local file, err = io.open( filename, mode or "wb" );
  if not file then
    return err;
  end
  local f, e = file:write( data );
  file:close();
  return e;
end

string.read           = function( ... ) return readfile                    ( ... ); end
string.write          = function( ... ) return writefile                   ( ... ); end