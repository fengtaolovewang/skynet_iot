package.path = '/dbconf/dbzigbee_lua/lua_modle/?.lua;'

local zigbee_entrance = require( "zigbee_entrance" )
local aux_core = require( "aux_core" )
local zcl = require( "zcl" )
local coap=require("aux_coap")


	

local dev_elec_colle=
{
heartbeat=28800,
logic_id=0x00e0,
endpoint=
{
	{
		ep_id=0x01,
        profile=0x0104,
        dev_id=0x00e0,
        device_version=0,
        in_cluster=
        {
            0x0000,
            0x0003,
			0x0004,
			0x0005,
			0x1000,
            0x0702,
			0x0b01,
			0x0b04,
			
        },
        out_cluster=
		{
			0x0019,
		},
    },
}
}

local  elec_colle_notify=function(mac,child_id,code)

	print("a new elec notify = " .. code)

	if(zigbee_entrance.online==code)then
		zbdev.online(mac)
	elseif(zigbee_entrance.annce ==code)then
		zbdev.online(mac)
	elseif(zigbee_entrance.offline==code or zigbee_entrance.leave == code)then
		zbdev.offline(mac)
	end
	
end

local elec_colle_zigbee_recive =function (mac,child_id,rssi,data_type,data)
	
	print("elec_colle_zigbee_recive" .. " mac=" .. aux_core.StrToHexStr(mac,":"))
	print("child_id = " .. child_id)
	if(data_type==0x4900)then
	
		local packg= zigbee.new_zcl(data)	
		local ep,cluster,fc_type,cmd,payload=packg:get()		
		print(aux_core.StrToHexStr(payload,nil))
		
		local attr= zcl.strtocmd(cmd,payload)
		
		zbdev.report(mac,zcl.zcl_analog(attr[1].data_type,attr[1].payload),rssi)
	end
end


local elec_colle_send_prcess_prcess=function(mac,code,rssi,data_type,data)
	print("elec_colle_send_prcess_prcess code =" .. code )
end

local elec_colle_coap_recive=function(mac,child_id,fd,token,pdu)

	print("elec_coap_recive")
	print(pdu:get_payload())

	local payload= pdu:get_payload()
	pdu:clear()
	zbdev.coap_send(pdu,fd,token,coap.COAP_RES_CODE(202),nil,nil)
	
	local zcl_write={}
		zcl_write.attr_id=0x400a
		zcl_write.data_type=0x42
		local payload_len= #payload >> 1
		
		zcl_write.payload= zcl.byteToHexC(0) .. payload
		zcl_write.payload= zcl.byteToHexC(payload_len) .. zcl_write.payload
		--zcl_write.payload= zcl.byteToHexC(payload_len+2) .. zcl_write.payload
		
		print(zcl_write.payload)
		
		zcl_write.data_len=payload_len +2
		
		local write_req={}
		write_req[1]=zcl_write
		
		local pack=zigbee.new_zcl(nil)
		
		pack:set(mac,0x01,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))
		
		zbdev.send(mac,0x0900,pack,pack:size(),elec_send_prcess_prcess,nil)
		
		write_req=nil
		pack=nil
		zcl_write=nil	
end


zigbee_obj_register_devmodle("dev_elec_colle",dev_elec_colle,elec_colle_coap_recive,elec_colle_zigbee_recive,elec_colle_notify,0)
