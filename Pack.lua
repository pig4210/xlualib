local function readfile( filename )
    local file, err = io.open( filename, "r" );
    if not file then
        return nil, err;
    end

    local data = file:read( "*a" );
    file:close();
    return data:gsub( "^\xEF\xBB\xBF", "" );
end

local function writefile( data, filename )
    local file, err = io.open( filename, "w" );
    if not file then
        return err;
    end
    if #data < 3 or data:sub( 1, 3 ) == "\xEF\xBB\xBF" then
        data = "\xEF\xBB\xBF" .. data;
    end
    local f, e = file:write( data );
    file:close();
    return e;
end

local function EnumFile( filter )
    local cmd, err = io.popen(
        [[for /r .\\Lua %f in (]] .. filter .. [[) do @echo %f]],
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

local luadata = EnumFile( "*.lua" );
local mddata = EnumFile( "*.md" );

for note in luadata:gmatch( '%-%-%[=======%[(.-)%]=======%]') do
    mddata = mddata .. note;
end

local dstlua, dstmd = ...;

local compiled, err = load( luadata );
if not compiled then
    writefile( luadata, dstlua );
    return error( "Compile Lua fail ! " .. err );
end

writefile( string.dump( compiled, true ), dstlua );
writefile( mddata, dstmd );