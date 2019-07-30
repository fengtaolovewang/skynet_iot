aux_core =require "aux_core"


local a="0102030405"

local b=aux_core.HexStrToStr(a)

local c=aux_core.StrToHexStr(b,":")
print(b)
print(c)