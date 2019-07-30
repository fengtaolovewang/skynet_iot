local zcl = require( "zcl" )

local ias = {}

local zonetype={}
zonetype[0x000d]={name="Motion sensor"}
zonetype[0x0015]={name="Contact switch"}
zonetype[0x0028]={name="Fire sensor"}
zonetype[0x002a]={name="Water sensor"}
zonetype[0x002b]={name="Gas sensor "}
zonetype[0x002c]={name="Personal emergency"}
zonetype[0x002d]={name="Vibration/Movement sensor"}
zonetype[0x010f]={name="Remote Control"}
zonetype[0x0115]={name="Key fob"}
zonetype[0x021d]={name="Keypad"}
zonetype[0x0225]={name="Warning Device"}

local zonestatus={}
zonestatus[0]={name="Alarm1"}
zonestatus[1]={name="Alarm2"}
zonestatus[2]={name="Tamper"}
zonestatus[3]={name="Battery"}
zonestatus[4]={name="Supervision"}
zonestatus[5]={name="Restore"}
zonestatus[6]={name="Trouble"}
zonestatus[7]={name="AC"}
zonestatus[8]={name="Test"}
zonestatus[9]={name="Battery Defect"}

ias.change=0x00
ias.Enroll=0x01

ias[0x0001]={name="ZoneType",type=zcl.ENUM16,data=0x0000}
ias[0x0002]={name="ZoneStatus",type=zcl.BITMAP16,data=0x0000}

ias.report=function(type,cmd,data,cb)
	if(type) then
		if(cmd==ias.change)then
			local i=ias[0x0002]
			i.data=zcl.little_Uint16(data:byte(1,2))
			
			print(i.name,i.data)
			if(cb)then
				cb(0x0002,i)
			end
		elseif(cmd==ias.Enroll)then
			local i=ias[0x0001]
			i.data=zcl.little_Uint16(data:byte(1,2))
			print(i.name,zonetype[i.data])
			if(cb)then
				cb(0x0001,i)
			end
		end		
	else
		
	
	end
end


return ias