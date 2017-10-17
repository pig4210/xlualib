--[=======[
-------- -------- -------- --------
          protobuf²Ù×÷
-------- -------- -------- --------
]=======]
--[=======[
¡ñ
    table     xpbc                      ( string protobuf );                [-0, +1, -]
]=======]

local function get_protobuf_index_type( pb )
  local v, size = getvarint( pb );
  if size == 0 then
    return nil;
  end
  return pb:sub( size + 1 ), v >> 3, v & 7;
end

local function get_protobuf_varint( pb )
  local v, size = getvarint( pb );
  if size == 0 then
    return nil;
  end
  return pb:sub( size + 1 ), v;
end

function xpbc( pb )
  local rets = {};
  local index;
  local types;
  local v;
  local ss;
  while #pb ~= 0 do
    pb, index, types = get_protobuf_index_type( pb );
    if not pb then
      --xdbg( "no index&type" );
      return;
    end
    --xdbg( "index&type", index, types );
    if types == 0 then
      pb, v = get_protobuf_varint( pb );
      if not pb then
        --xdbg( "    no varint" );
        return;
      end
    elseif types == 2 then
      pb, v = get_protobuf_varint( pb );
      if not pb then
        --xdbg( "    no size" );
        return;
      end
      if #pb < v then
        --xdbg( "    size error" );
        return;
      end
      ss = pb:sub( 1, v );
      pb = pb:sub( v + 1 );

      v = xpbc( ss );
      if not v then
        --xdbg( "    string not protobuf" );
        v = ss;
      end
    else
      return;
    end
    if rets[ index ] then
      if type( rets[ index ] ) == "table" then
        rets[ index ][ #rets[ index ] + 1 ] = v;
      else
        rets[ index ] = { v };
      end
    else
      rets[ index ] = v;
    end
  end
  return rets;
end