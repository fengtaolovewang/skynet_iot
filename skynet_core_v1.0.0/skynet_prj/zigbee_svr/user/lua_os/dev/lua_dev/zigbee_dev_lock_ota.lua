package.path = '/dbconf/dbzigbee_lua/lua_modle/?.lua;'

local zigbee_entrance = require( "zigbee_entrance" )
local aux_core = require( "aux_core" )
local zcl = require( "zcl" )
local coap=require("aux_coap")
local ota=require("aux_ota")

local dev_ota_lock=
{
heartbeat=180,
logic_id=0x0162,

endpoint=
{
	{
		ep_id=0x0b,
        profile=0x0104,
        dev_id=0x0162,
        device_version=0,
        in_cluster=
        {
            0x0000,
            0x0003,
            0x0415,
        },
        out_cluster=
        {
            0x0003,
        },
    },
}
}

local lock_ota={}

local  lock_ota_notify=function(mac,child_id,code)

--	print("a new lock notify = " .. code)
	if(zigbee_entrance.online==code or zigbee_entrance.annce==code)then
		zbdev.online(mac)
	elseif(zigbee_entrance.offline==code or zigbee_entrance.leave == code)then
		zbdev.offline(mac)
	end
end


local lock_ota_send_prcess_prcess=function(mac,code,rssi,data_type,data)

	print("lock_send_prcess_prcess code =" .. code )
	
	if(zbdev.get_fifo_num(mac) > 0)then
		zbdev.send_fifo(mac)
	end
end

local lock_ota_zigbee_recive =function(mac,child_id,rssi,data_type,data)
	print(aux_core.StrToHexStr(mac,":"))
	
	if(data_type==0x4900)then
	
		local packg= zigbee.new_zcl(data)
	
		local ep,cluster,fc_type,cmd,payload=packg:get()
		
		if(cluster == 0x0019)then
			packg=ota.ota_process(mac,ep,cmd,lock_ota,packg,payload)		
			
			zbdev.send(mac,0x0900,packg,packg:size(),nil,nil)
			
			return
		end
		
		local attr= zcl.strtocmd(cmd,payload)
		if(#attr==1)then
			zbdev.report(mac,attr[1].payload,rssi)
		elseif(cmd == zcl.REPORT)then
			local zcl_write={}
				zcl_write.attr_id=0x410d
				zcl_write.data_type=0x20
				zcl_write.data_len=1
				local num=zbdev.get_fifo_num(mac)
				
				zcl_write.payload= zcl.byteToHexC(num)
				
				print(zcl_write.payload)
		
				local write_req={}
				write_req[1]=zcl_write
		
				local pack=zigbee.new_zcl(nil)
		
				pack:set(mac,ep,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))			
				zbdev.send(mac,0x0900,pack,pack:size(),lock_ota_send_prcess_prcess,nil)
		end
	end
end

local lock_ota_add_fifo=function(payload)
	local zcl_write={}
		zcl_write.attr_id=0x410d
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
		
		pack:set(mac,0x0b,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))
		
		zbdev.add_fifo(mac,0x0900,pack,pack:size(),lock_ota_send_prcess_prcess,nil)
		
		write_req=nil
		pack=nil
		zcl_write=nil	
end

local lock_ota_coap_recive=function(mac,child_id,fd,token,pdu)
	print("lock_ota_coap_recive")
	print(pdu:get_payload())
	
	local payload= pdu:get_payload()
	local uri= pdu:get_options(OPTION_LOCATION_QUERY)
	
	if(uri == "zigbee_ota")then
	
		lock_ota.len,lock_ota.file =zbdev.lode_file("/dbconf/zigbee_ota/lock.hex")
		lock_ota.menucode = 0x1037
		lock_ota.type = 0x1234
		lock_ota.version= 3
		
		if(lock_ota.len)then
		local pack=zigbee.new_zcl(nil)
			pack:set(mac,0x0b,0x0019,zcl.SPECIFIC_CMD,ota.COMMAND_IMAGE_NOTIFY,0,0,ota.image_notify(ota.NOTIFY_PAYLOAD_JITTER,0x48))
			
			zbdev.add_fifo(mac,0x0900,pack,pack:size(),nil,nil)
		end
		
	else	
		lock_ota_add_fifo(payload)
	end
	
	pdu:clear()
	zbdev.send(pdu,fd,token,coap.COAP_RES_CODE(202),nil,nil)
end

--[[
	zigbee_dev_check_modle 校验 模板
	zigbee_obj_register(name,dev_info,coap_recive,zigbee_recive,dev_notify,fifo_flag)
]]--

zigbee_obj_register_devmodle("dev_ota_lock",dev_ota_lock,lock_ota_coap_recive,lock_ota_zigbee_recive,lock_ota_notify,1)


