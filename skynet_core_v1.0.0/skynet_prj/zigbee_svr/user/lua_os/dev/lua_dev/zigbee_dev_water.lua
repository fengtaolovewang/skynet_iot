package.path = '/dbconf/dbzigbee_lua/lua_modle/?.lua;'

local zigbee_entrance = require( "zigbee_entrance" )
local aux_core = require( "aux_core" )
local zcl = require( "zcl" )
local coap=require("aux_coap")

local	Electric_Metering=0
local	Gas_Metering =1
local	Water_Metering =2
local	Thermal_Metering =3
local	Pressure_Metering =4
local	Heat_Metering =5
local	Cooling_Metering =6
local	Mirrore__Gas_Metering=128
local	Mirrored_Water_Metering=129
local	Mirrored_Thermal_Metering =130
local	Mirrored_Pressure_Metering=131
local	Mirrored_Heat_Metering=132
local	Mirrored_Cooling_Metering=133
	

local dev_meter=
{
heartbeat=40,
logic_id=0x0053,
endpoint=
{
	{
		ep_id=0x08,
        profile=0x0104,
        dev_id=0x0053,
        device_version=0,
        in_cluster=
        {
            0x0000,
            0x0003,
            0x0702,
			0x0b01,
        },
        out_cluster= 
		{
			0x0000,
		},
    },
}
}

local meter_read_prcess_prcess=function(mac,code,rssi,data_type,data)
	print("meter ack msg !!!!")
	if(data_type==0x4900 and code ==zigbee_entrance.uart_ack)then
	
		local packg= zigbee.new_zcl(data)
	
		local ep,cluster,fc_type,cmd,payload=packg:get()
	
		print(aux_core.StrToHexStr(payload,nil))
		
		local attr= zcl.strtocmd(cmd,payload)
		print("status = " .. attr[1].status)
		print(string.format("attr_id = %04x",attr[1].attr_id))
		
		if(attr[1].status== 0 and attr[1].attr_id== 0x0306)then
		
			zbdev.set_child(mac,attr[1].payload:byte(1))
			
			zbdev.online(mac)
		end
	else
		zbdev.leave(mac)
	end
end

local meter_read_child=function(mac)

		local read_req={}
		read_req[1]=0x0306
		
		local pack=zigbee.new_zcl(nil)
		
		pack:set(mac,0x08,0x0702,zcl.PROFILE_CMD,zcl.READ,0,0,zcl.cmdtostr(zcl.READ,read_req))
		
		zbdev.send(mac,0x0900,pack,pack:size(),meter_read_prcess_prcess,nil)
end


local  meter_notify=function(mac,child_id,code)

	print("a new meter notify = " .. code)

	if(zigbee_entrance.online==code)then
		zbdev.online(mac)
	elseif(zigbee_entrance.annce ==code)then
		meter_read_child(mac)
	elseif(zigbee_entrance.offline==code or zigbee_entrance.leave == code)then
		zbdev.offline(mac)
	end
	
end

local meter_zigbee_recive =function (mac,child_id,rssi,data_type,data)
	
	print("meter_zigbee_recive" .. " mac=" .. aux_core.StrToHexStr(mac,":"))
	print("child_id = " .. child_id)
	if(data_type==0x4900)then
	
		local packg= zigbee.new_zcl(data)	
		local ep,cluster,fc_type,cmd,payload=packg:get()		
		print(aux_core.StrToHexStr(payload,nil))
		
		local attr= zcl.strtocmd(cmd,payload)
		
		zbdev.report(mac,zcl.zcl_analog(attr[1].data_type,attr[1].payload),rssi)
	end
end


local meter_send_prcess_prcess=function(mac,code,rssi,data_type,data)
	print("meter_send_prcess_prcess code =" .. code )
end

local meter_coap_recive=function(mac,child_id,fd,token,pdu)

	print("meter_coap_recive")
	print(pdu:get_payload())

	local payload= pdu:get_payload()
	pdu:clear()
	zbdev.send(pdu,fd,token,coap.COAP_RES_CODE(402),nil,nil)
	
end


zigbee_obj_register_devmodle("dev_meter",dev_meter,meter_coap_recive,meter_zigbee_recive,meter_notify,0)
