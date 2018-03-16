local argv = { ... };

local function readfile( filename )
    local file, err = io.open( filename, "rb" );
    if not file then
        return nil, err;
    end

    local data = file:read( "*a" );
    file:close();
    return data:gsub( "^\xEF\xBB\xBF", "" );
end

local function writefile( data, filename )
    local file, err = io.open( filename, "wb" );
    if not file then
        return err;
    end
    local f, e = file:write( data );
    file:close();
    return e;
end

local function EnumFile( path, filter )
    print( "EnumFile", path, filter );
    local cmd, err = io.popen(
        [[for /r "]] .. path .. [[" %f in (]] .. filter .. [[) do @echo %f]],
        "r"
    );
    if not cmd then
        return error( "Lua popen fail ! " .. err );
    end
    
    local datas = "";
    
    while true do
        local filename = cmd:read();
        if not filename then
            break;
        end
        local data, err = readfile( filename );
        if not data then
            return error( err );
        end
        datas = datas .. data;
        print( "", ( filename:gsub( [=[[^%\%/]+[%\%/]]=], "" ) ) );
    end
    
    cmd:close();

    return datas;
end

if #argv < 3 then
    return error( "Pack.lua Path1 [Path2 Path3 ...] DestLua DestMd" );
end

local luadata = "";
local mddata = "";
for k = 1, #argv - 2 do
    luadata = luadata .. EnumFile( argv[k], "*.lua" );
    mddata = mddata .. EnumFile( argv[k], "*.md" );
end

for note in luadata:gmatch( '%-%-%[=======%[(.-)%]=======%]') do
    mddata = mddata .. note;
end

local dstlua, dstmd = argv[ #argv - 1 ], argv[ #argv ];

local compiled, err = load( luadata );
if not compiled then
    writefile( luadata, dstlua );
    return error( "Compile Lua fail ! " .. err );
end

--writefile( luadata, dstlua );
writefile( string.dump( compiled, true ), dstlua );
writefile( "\xEF\xBB\xBF" .. mddata, dstmd );