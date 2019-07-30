local zcl = require( "zcl" )

local onoff = {}

onoff.cluster		=0x0006
onoff.att_onoff	    =0x0000
onoff.att_ontime	=0x4001
onoff.att_offwait	=0x4002
onoff.cmd_off		=0x00
onoff.cmd_on		=0x01
onoff.cmd_togger	=0x02

onoff[0x0000]={name="OnOff",type=zcl.BOOLEAN,data=0}
--onoff[0x0000]={name="OnOff",type=zcl.BOOLEAN,data=0}


function onoff.write(attrid,data)
	local a=onoff[attrid]
		if a then
			if(data==a.data) then
				return nil
			end
			local cmd=zcl.byteToHexC(data)
				
				return zcl.S2H(cmd)
		end
end

function onoff.read()
	local a={0x0000}
	
	return zcl.cmdtostr(zcl.READ,a)
end

function onoff.report(type,cmd,data,cb)

	if(cmd ~= zcl.READ_RSP and cmd ~= zcl.REPORT)then
		return
	end
	
	local a=zcl.strtocmd(cmd,data)
	
	if a then
		local c= onoff[a.attr_id]
		if c then
			c.data=a.payload:byte(1)
			if(cb)then
				cb(a.attr_id,c)
			end
		end
		print(c.name,c.data)
	end
end

return onoff