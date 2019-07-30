--[[这里一个物理ZigBee节点支持多个端点，通常情况下，
这些设置中的许多设置将应用于整个节点，也就是说，
它们对于设备上的每个端点都是相同的。 
在这种情况下，它们可以为节点实施一次，
并映射到每个端点。]]--
local zcl=require "zcl"
local basic = {}
basic.cluster			=0x0000
basic.cmd_reset		=0x00
-- Basic Device Information
basic.attr_zclver		=0x0000
basic.attr_app_ver		=0x0001
basic.attr_stack_ver	=0x0002
basic.attr_hw_ver		=0x0003
basic.attr_manufacturer_Name	=0x0004
basic.attr_modelIdentifier		=0x0005
basic.attr_data_code			=0x0006
basic.attr_POWER_SOURCE 		=0x0007
basic.attr_SW_BUILD_ID			=0x4000
--Device Settings
basic.attr_LOCATION_DESC       =0x0010
basic.attr_PHYSICAL_ENV        =0x0011
basic.attr_DEVICE_ENABLED      =0x0012
basic.attr_ALARM_MASK          =0x0013
basic.attr_DISABLE_LOCAL_CONFIG =0x0014


--Values of the PowerSource Attribute  
basic.Power_unkonw       =0x00
basic.Power_single_phase =0x01
basic.Power_three_phase  =0x02
basic.Power_battery      =0x03
basic.Power_DC_source    =0x04
basic.Power_Emergency    =0x05	--紧急电源
basic.Power_transfer	 =0x06

basic[0x0000]={name="ZCLVersion",type=zcl.UINT8,data=0x01} --00-255
basic[0x0001]={name="ApplicationVersion",type=zcl.UINT8,data=0x00} --00-255
basic[0x0002]={name="StackVersion",type=zcl.UINT8,data=0x00} --00-255
basic[0x0003]={name="HWVersion",type=zcl.UINT8,data=0x00} --00-255
basic[0x0004]={name="ManufacturerName",type=zcl.CHAR_STR,data=""} --0-32
basic[0x0005]={name="ModelIdentifier",type=zcl.CHAR_STR,data=""} --0-32
basic[0x0006]={name="DateCode",type=zcl.CHAR_STR,data=""} --0-16
basic[0x0007]={name="PowerSource",type=zcl.ENUM8,data=0x00} --0-255

basic.report=function(type,cmd,data,cb)
	if(cmd ~= zcl.READ_RSP and cmd ~= zcl.REPORT)then
		return
	end
	
	local a=zcl.strtocmd(cmd,data)
		if(a)then
			for k,v in ipairs(a) do
				local b=basic[v.attr_id]
				if b then
					if( zcl.UINT8==v.data_type )
					then
						b.data=v.payload:byte(1)
					elseif(zcl.CHAR_STR==v.data_type)
					then
						b.data=v.payload
					end
					if(cb)then
						cb(v.attr_id,b)
					end
				end	
				print(b.name,b.data)
			end
		end		
end


return basic
