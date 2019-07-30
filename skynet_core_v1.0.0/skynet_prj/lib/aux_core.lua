--[[

    aux_core.lua

    Programmer:		Hetao

    Remark:

--]]

local module = {}

function module.IsNil( value )

	return "nil" == type( value )
end

function module.IsBoolean( value )

	return "boolean" == type( value )
end

function module.IsNumber( value )

	return "number" == type( value )
end

function module.IsString( value )

	return "string" == type( value )
end

function module.IsTable( value )

	return "table" == type( value )
end

function module.IsUserData( value )

	return "userdata" == type( value )
end

function module.IsFunction( value )

	return "function" == type( value )
end

function module.IsThread( value )

	return "thread" == type( value )
end

function module.IsAlnum( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return ( n > 47 and n < 58 ) or ( n > 64 and n < 91 ) or ( n > 96 and n < 123 )
end

function module.IsAlpha( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return ( n > 64 and n < 91 ) or ( n > 96 and n < 123 )
end

function module.IsCntrl( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return n < 32 or n > 127
end

function module.IsDigit( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return n > 47 and n < 58
end

function module.IsLower( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return n > 96 and n < 123
end

function module.IsPrint( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return n > 31 and n < 127
end

function module.IsSpace( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return ( n > 8 and n < 14 ) or 32 == n
end

function module.IsUpper( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return n > 64 and n < 91
end

function module.IsHexadecimal( c )

	local n = ( module.IsNumber( c ) and c ) or c:byte()

	return ( n > 47 and n < 58 ) or ( n > 64 and n < 71 ) or ( n > 96 and n < 103 )
end
-- spacing 分隔符
function module.StrToHexStr( value, spacing )

	if module.IsNil( value ) then

		return ""
	end

	assert( module.IsString( value ), "string expected" )

	local result = {}

	if #value > 0 then

		local s_format ={}
		table.insert(result,string.format("%02X", value:byte( 1 ) ))
		
		if module.IsString( spacing ) then
			table.insert(s_format,spacing)
		end
			table.insert(s_format,"%02X")
			s_format=table.concat(s_format)
			
		for i = 2, #value, 1 do
			table.insert(result,string.format( s_format, value:byte( i ) ))
		end
	end

	return table.concat(result)
end

-- HexStrToStr 标示 把HEX 字符串流 转换为 HEX转义字符流
function module.HexStrToStr( value )

	if module.IsNil( value ) then

		return ""

	end

	assert( module.IsString( value ), "string expected" )

	local c = 0

	local count = 1

	local result = {}

	for i = 1, #value, 1 do

		local n = value:byte( i, i )

		if module.IsHexadecimal( n ) then

			if count % 2 == 1 then

				c = ( module.IsDigit( n ) and n - 48 ) or ( module.IsUpper( n ) and n - 65 + 10 ) or n - 97 + 10
			else

				c = ( c << 4 ) | ( ( module.IsDigit( n ) and n - 48 ) or ( module.IsUpper( n ) and n - 65 + 10 ) or n - 97 + 10 )
				
				table.insert(result,string.char( c ))
			--	result = result .. string.char( c )
			end

			count = count + 1
		end
	end

	return table.concat(result)
end

return module
