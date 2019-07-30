-- dev module
zcl =require "zcl"

local module = {}

module[0x0000]	=      	{name="onoff_switch",out_cluster={0x0003,0x0006}}
module[0x0001]	=     	{name="level_control_switch",out_cluster={0x0003,0x0006,0x0008}}
module[0x0002]	=     	{name="onoff_output",in_cluster= {0x0004,0x0005,0x0006}}
module[0x0003]	=		{name="level_controllable_Output",in_cluster= {0x0004,0x0005,0x0006,0x0008}}
module[0x0004]	=       {name="scene_selector",out_cluster={0x0004,0x0005}}

module[0x0009]	= 		{name="mains_power_outlet",in_cluster= {0x0004,0x0005,0x0006}}
module[0x000A]	=		{name="door_lock",in_cluster= {0x0004,0x0005,0x0101}}
module[0x000B]	=	 	{name="door_lock_controller",out_cluster={0x0004,0x0005,0x0101}}
module[0x000C]	=		{name="simple_sensor",in_cluster= {0x000F},out_cluster={0x0003}}

module[0x000d] =		{name="consumption_awareness_device",in_cluster= {0x0702},out_cluster={0x0003}}
module[0x0051] =		{name="Smart_Plug",in_cluster= {0x0006,0x0702},out_cluster={0x0003}}
module[0x0052] =		{name="White_Goods"}
module[0x0053] =		{name="Meter_Interface",in_cluster= {0x0702,0x0b01},out_cluster={0x0003}}

--LIGHT
module[0x0100] =        {name="On/Off_Light",in_cluster= {0x0004,0x0005,0x0006}}
module[0x0101] =        {name="Dimmable_Light",in_cluster= {0x0004,0x0005,0x0006,0x0008}}
module[0x0102] =        {name="Color_Dimmable_Light",in_cluster= {0x0004,0x0005,0x0006,0x0008,0x0300}}
module[0x0103] =        {name="On/Off_Light_Switch",out_cluster={0x0003,0x0006}}

module[0x0104] =        {name="Dimmer_Switch",out_cluster={0x0003,0x0006,0x0008}}
module[0x0105] =        {name="Color_Dimmer_Switch",out_cluster={0x0003,0x0006,0x0008,0x0300}}

module[0x0106] =        {name="Light_Sensor",in_cluster= {0x0400},out_cluster={0x0003}}
module[0x0107] =        {name="Occupancy_Sensor",in_cluster= {0x0406},out_cluster={0x0003}}

--closure
module[0x0200] = 		{name="Shade",in_cluster= {0x0004,0x0005,0x0006,0x0008,0x0100}}
module[0x0201] = 		{name="Shade_Controller",out_cluster={0x0003,0x0006,0x0008}} 
module[0x0202] =  		{name="Window_Covering_Device",in_cluster= {0x0004,0x0005,0x0102}}
module[0x0203] =  		{name="Window_Covering_Controller",out_cluster={0x0003,0x0102}}

--HVAC Devices

--Intruder Alarm System Devices
module[0x0402] = 		{name="IAS_Zone",in_cluster= {0x0500}}
module[0x0403] = 		{name="IAS_Warning_Device",in_cluster= {0x0500,0x0502}}

local function Mclister(inc,outc,id)
	local M=module[id]
	
	if not M then
		return false
	end
	local ic={}
	local oc={}
	local i=1
	
	while(i<#inc)do
		local n=zcl.little_Uint16(inc:byte(i,i+1))
		ic[n]=1
		i=i+2
	end
	i=1
	while(i<#outc)do
		local n=zcl.little_Uint16(outc:byte(i,i+1))
		oc[n]=1
		i=i+2
	end	
	if M.in_cluster then
		for i = 1, #M.in_cluster, 1 do
			local c=M.in_cluster[i]
			
			if not ic[c] then
				return false
			end
		end
	end
	
	if M.out_cluster then
		for i = 1, #M.out_cluster, 1 do
			local c=M.out_cluster[i]
			
			if not oc[c] then
				return false
			end
		end
	end	
	
	print(M.name)
	return true
end

module.find=function(eplist)
	local result={}
	
	for k,v in ipairs(eplist) do
		local M=Mclister(v.inc,v.outc,v.id)
		
			if(M)then
				table.insert(result,v.id)
			end			
	end
	table.sort(result)
end
 
return module