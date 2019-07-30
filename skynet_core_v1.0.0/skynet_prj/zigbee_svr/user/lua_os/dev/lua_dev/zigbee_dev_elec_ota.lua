package.path = '/dbconf/dbzigbee_lua/lua_modle/?.lua;'

local zigbee_entrance = require( "zigbee_entrance" )
local aux_core = require( "aux_core" )
local zcl = require( "zcl" )
local coap=require("aux_coap")
local ota=require("aux_ota")

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
	

local dev_ota_elec=
{
heartbeat=28800,
logic_id=0x0053,
endpoint=
{
	{
		ep_id=0x01,
        profile=0x0104,
        dev_id=0x0053,
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

local elec_ota={}


local elec_ota_read_prcess_prcess=function(mac,code,rssi,data_type,data)
	print("elec ack msg !!!!")
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

local elec_ota_read_child=function(mac)

		local read_req={}
		read_req[1]=0x0306
		
		local pack=zigbee.new_zcl(nil)
		
		pack:set(mac,1,0x0702,zcl.PROFILE_CMD,zcl.READ,0,0,zcl.cmdtostr(zcl.READ,read_req))
		
		zbdev.send(mac,0x0900,pack,pack:size(),elec_read_prcess_prcess,nil)
end


local  elec_ota_notify=function(mac,child_id,code)

	print("a new elec notify = " .. code)

	if(zigbee_entrance.online==code)then
		zbdev.online(mac)
	elseif(zigbee_entrance.annce ==code)then
		elec_ota_read_child(mac)
	elseif(zigbee_entrance.offline==code or zigbee_entrance.leave == code)then
		zbdev.offline(mac)
	end
end

local elec_ota_zigbee_timeout=function(ota_info)
	print("elec_ota_zigbee_timeout elec_ota.index= " .. elec_ota.index .. ":::" .. ota_info.index)
	
	if(elec_ota.index == ota_info.index)then
		zbdev.send(ota_info.mac,0x0900,ota_info.packeg,ota_info.packeg:size(),nil,nil)
	end
		
	ota_info=nil
end

local  elec_ota_zigbee_rsp_msg=function(ota_info)
	
	zbdev.send(ota_info.mac,0x0900,ota_info.packeg,ota_info.packeg:size(),nil,nil)
	
	zbdev.time(elec_ota_zigbee_timeout,ota_info,2000)
end

local elec_ota_zigbee_recive =function (mac,child_id,rssi,data_type,data)
	
	print("elec_zigbee_recive" .. " mac=" .. aux_core.StrToHexStr(mac,":"))
	print("child_id = " .. child_id)
	if(data_type==0x4900)then
	
		local packg= zigbee.new_zcl(data)	
		local ep,cluster,fc_type,cmd,payload=packg:get()		
		print(aux_core.StrToHexStr(payload,nil))
--		print("cluster=" .. cluster)
--		print("cmd=" .. cmd)
		
		if(cluster == 0x0019)then
			--packg=ota.ota_process(mac,ep,cmd,elec_ota,packg,payload)		
			local ota_msg={}
			ota_msg.mac=mac
			ota_msg.packeg,elec_ota.index =ota.ota_process(mac,ep,cmd,elec_ota,packg,payload)
			ota_msg.index=elec_ota.index
			
			zbdev.time(elec_ota_zigbee_rsp_msg,ota_msg,300)
			
			return
		end
		
		local attr= zcl.strtocmd(cmd,payload)
		
		zbdev.report(mac,zcl.zcl_analog(attr[1].data_type,attr[1].payload),rssi)
	end
end


local elec_ota_send_prcess_prcess=function(mac,code,rssi,data_type,data)
	print("elec_ota_send_prcess_prcess code =" .. code )
end

local elec_ota_send_msg=function(payload)

local zcl_write={}
		zcl_write.attr_id=0x400a
		zcl_write.data_type=0x42
		local payload_len= #payload >> 1
		
		zcl_write.payload= zcl.byteToHexC(0) .. payload
		zcl_write.payload= zcl.byteToHexC(payload_len) .. zcl_write.payload
		zcl_write.payload= zcl.byteToHexC(payload_len+2) .. zcl_write.payload
		
		print(zcl_write.payload)
		
		zcl_write.data_len=payload_len +3
		
		local write_req={}
		write_req[1]=zcl_write
		
		local pack=zigbee.new_zcl(nil)
		
		pack:set(mac,1,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))
		
		zbdev.send(mac,0x0900,pack,pack:size(),elec_ota_send_prcess_prcess,nil)
		
		write_req=nil
		pack=nil
		zcl_write=nil	
end

local elec_ota_coap_recive=function(mac,child_id,fd,token,pdu)

	print("elec_coap_recive")
	print(pdu:get_payload())

	
	
	local uri= pdu:get_options(coap.OPTION_LOCATION_QUERY)
	if(uri == "zigbee_ota")then
	
		elec_ota.len,elec_ota.file =zbdev.lode_file("/dbconf/zigbee_ota/nxp_elec.ota")
		elec_ota.menucode = 0x1037
		elec_ota.type = 0x0101
		elec_ota.version= 2
		elec_ota.index=0
		
		print("size =" .. elec_ota.len)
		if(elec_ota.len)then
		local pack=zigbee.new_zcl(nil)
		
			pack:set(mac,0x01,0x0019,zcl.SPECIFIC_CMD,ota.COMMAND_IMAGE_NOTIFY,0,0,ota.image_notify(ota.NOTIFY_PAYLOAD_JITTER,0x48))
			
			zbdev.send(mac,0x0900,pack,pack:size(),nil,nil)
			pack=nil
		end
	else
		local payload= pdu:get_payload()
		elec_ota_send_msg(payload)
	end
	pdu:clear()
	zbdev.coap_send(pdu,fd,token,coap.COAP_RES_CODE(202),nil,nil)
	
	
end


zigbee_obj_register_devmodle("dev_ota_elec",dev_ota_elec,elec_ota_coap_recive,elec_ota_zigbee_recive,elec_ota_notify,0)
