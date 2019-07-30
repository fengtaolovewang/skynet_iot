package.path = '/dbconf/dbzigbee_lua/lua_modle/?.lua;'

local zigbee_entrance = require( "zigbee_entrance" )
local aux_core = require( "aux_core" )
local zcl = require( "zcl" )
local coap=require("aux_coap")


local dev_lock_sleep=
{
heartbeat=0,
logic_id=0x0162,

endpoint=
{
	{
		ep_id=0x01,
        profile=0x0104,
        dev_id=0x0162,
        device_version=0,
        in_cluster=
        {
            0x0000,
			0x0001,
            0x0003,
            0x0019,
        },
        out_cluster=
        {
            0x0000,
			0x0001,
            0x0003,
            0x0019,
        },
    },
}
}
local lock_time_fun=nil
local lock_time_dev={}

local lock_time_event=function(mac)

	print("lock_time_event")
	
	for i = 1,#lock_time_dev,1 do

		if(lock_time_dev[i].mac == mac)then
			if(zbdev.get_fifo_num(mac) > 0)then
			zbdev.send_fifo(mac)
			end
			local tim_hign,tim_lite=zbdev.get_time()
			
			if(lock_time_dev[i].low ==0)then
				lock_time_dev[i].low=tim_lite
				lock_time_dev[i].high=tim_hign
			else
				lock_time_dev[i].time_off=lock_time_dev[i].time_off+(tim_hign-lock_time_dev[i].high)+(tim_lite-lock_time_dev[i].low)-30000
				

				lock_time_dev[i].low=tim_lite
				lock_time_dev[i].high=tim_hign
			end
			print("time_off =" .. lock_time_dev[i].time_off)
			
			zbdev.time(lock_time_fun,mac,30000)
			return
		end
	end
	
	mac=nil
end

local lock_pong=function(mac,code,rssi,data_type,data)
	print("lock_pong code =" .. code )
	
	if(code == zigbee_entrance.uart_ack)then
	
		lock_time_fun=lock_time_event
		local dev={}
		dev.mac=mac
		dev.time_off=0
		dev.high=0
		dev.low=0
		
		local num= #lock_time_dev
		num=num+1
		lock_time_dev[num]=dev

		
		zbdev.time(lock_time_fun,mac,28000)
	else
		lock_ping(mac)
	end
	
end
lock_ping=function(mac)
	print("lock_ping")
	local payload="AA5501040105013736"
	
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
		
		pack:set(mac,0x01,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))
		
	
--[[		
	local zcl_write={}
		zcl_write.attr_id=0x410d
		zcl_write.data_type=0x20
		zcl_write.data_len=1
				
		zcl_write.payload= zcl.byteToHexC(0)
				
				local write_req={}
				write_req[1]=zcl_write
		
				local pack=zigbee.new_zcl(nil)
		
				pack:set(mac,0x01,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))	
]]--				
				zbdev.send(mac,0x0900,pack,pack:size(),lock_pong,nil,5,6000)
		write_req=nil
		pack=nil
		zcl_write=nil				
end

local  lock_notify=function(mac,child_id,code)

--	print("a new lock notify = " .. code)
	if(zigbee_entrance.annce==code )then
		zbdev.online(mac)
		
		for i = 1,#lock_time_dev,1 do
			if(lock_time_dev[i].mac == mac)then
				lock_time_dev[i]=nil
			end
		end
		zbdev.time(lock_ping,mac,65000)
	elseif(zigbee_entrance.online==code)then
		zbdev.online(mac)
		for i = 1,#lock_time_dev,1 do
			if(lock_time_dev[i].mac == mac)then
				lock_time_dev[i]=nil
			end
		end
		lock_ping(mac)
	elseif(zigbee_entrance.offline==code or zigbee_entrance.leave == code)then
		zbdev.offline(mac)
	end
end


local lock_send_prcess_prcess=function(mac,code,rssi,data_type,data)

	print("lock_send_prcess_prcess code =" .. code )
	if(code == zigbee_entrance.uart_outime)then
			for i = 1,#lock_time_dev,1 do
			if(lock_time_dev[i].mac == mac)then
				lock_time_dev[i]=nil
			end
		end
		lock_ping(mac)

	end

end

local lock_zigbee_recive =function (mac,child_id,rssi,data_type,data)
	print(aux_core.StrToHexStr(mac,":"))
	if(data_type==0x4900)then
	
		local packg= zigbee.new_zcl(data)
	
		local ep,cluster,fc_type,cmd,payload=packg:get()

		local attr= zcl.strtocmd(cmd,payload)
		if(#attr==1)then
			zbdev.report(mac,attr[1].payload,rssi)
		end
	end
end

local lock_add_fifo=function(mac,payload)
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
		
		pack:set(mac,0x01,0x0000,zcl.PROFILE_CMD,zcl.WRITE,0,0,zcl.cmdtostr(zcl.WRITE,write_req))
		
		zbdev.add_fifo(mac,0x0900,pack,pack:size(),lock_send_prcess_prcess,nil,1,6000)
		
		write_req=nil
		pack=nil
		zcl_write=nil	
end

local lock_coap_recive=function(mac,child_id,fd,token,pdu)
	print("lock_coap_recive")
	print(pdu:get_payload())
	
	local payload= pdu:get_payload()
	
	pdu:clear()
	zbdev.coap_send(pdu,fd,token,coap.COAP_RES_CODE(202),nil,nil)
	
	lock_add_fifo(mac,payload)
	
end

--[[
	zigbee_dev_check_modle 校验 模板
	zigbee_obj_register(name,dev_info,coap_recive,zigbee_recive,dev_notify,fifo_flag)
]]--

zigbee_obj_register_devmodle("dev_lock_sleep",dev_lock_sleep,lock_coap_recive,lock_zigbee_recive,lock_notify,1)


