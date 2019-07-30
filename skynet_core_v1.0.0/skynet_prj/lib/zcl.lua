local aux_core = require( "aux_core" )

local module = {}


--Client/Server Directions
module.CLIENT_SERVER_DIR                    = 0x00
module.SERVER_CLIENT_DIR                    = 0x01

--Frame Types
module.PROFILE_CMD                     = 0x00
module.SPECIFIC_CMD                    = 0x01

--cluster
module.ZCL_BASIC_CLUSTER_ID                                               = 0x0000
module.ZCL_POWER_CONFIG_CLUSTER_ID                                        = 0x0001
module.ZCL_DEVICE_TEMP_CLUSTER_ID                                         = 0x0002
module.ZCL_IDENTIFY_CLUSTER_ID                                            = 0x0003
module.ZCL_GROUPS_CLUSTER_ID                                              = 0x0004
module.ZCL_SCENES_CLUSTER_ID                                              = 0x0005
module.ZCL_ON_OFF_CLUSTER_ID                                              = 0x0006
module.ZCL_ON_OFF_SWITCH_CONFIG_CLUSTER_ID                                = 0x0007
module.ZCL_LEVEL_CONTROL_CLUSTER_ID                                       = 0x0008
module.ZCL_ALARM_CLUSTER_ID                                               = 0x0009
module.ZCL_TIME_CLUSTER_ID                                                = 0x000A
module.ZCL_RSSI_LOCATION_CLUSTER_ID                                       = 0x000B
module.ZCL_BINARY_INPUT_BASIC_CLUSTER_ID                                  = 0x000F
module.ZCL_COMMISSIONING_CLUSTER_ID                                       = 0x0015
module.ZCL_PARTITION_CLUSTER_ID                                           = 0x0016
module.ZCL_OTA_BOOTLOAD_CLUSTER_ID                                        = 0x0019
module.ZCL_POWER_PROFILE_CLUSTER_ID                                       = 0x001A
module.ZCL_APPLIANCE_CONTROL_CLUSTER_ID                                   = 0x001B
module.ZCL_POLL_CONTROL_CLUSTER_ID                                        = 0x0020
module.ZCL_GREEN_POWER_CLUSTER_ID                                         = 0x0021
module.ZCL_KEEPALIVE_CLUSTER_ID                                           = 0x0025
module.ZCL_SHADE_CONFIG_CLUSTER_ID                                        = 0x0100
module.ZCL_DOOR_LOCK_CLUSTER_ID                                           = 0x0101
module.ZCL_WINDOW_COVERING_CLUSTER_ID                                     = 0x0102
module.ZCL_BARRIER_CONTROL_CLUSTER_ID                                     = 0x0103
module.ZCL_PUMP_CONFIG_CONTROL_CLUSTER_ID                                 = 0x0200
module.ZCL_THERMOSTAT_CLUSTER_ID                                          = 0x0201
module.ZCL_FAN_CONTROL_CLUSTER_ID                                         = 0x0202
module.ZCL_DEHUMID_CONTROL_CLUSTER_ID                                     = 0x0203
module.ZCL_THERMOSTAT_UI_CONFIG_CLUSTER_ID                                = 0x0204
module.ZCL_COLOR_CONTROL_CLUSTER_ID                                       = 0x0300
module.ZCL_BALLAST_CONFIGURATION_CLUSTER_ID                               = 0x0301
module.ZCL_ILLUM_MEASUREMENT_CLUSTER_ID                                   = 0x0400
module.ZCL_ILLUM_LEVEL_SENSING_CLUSTER_ID                                 = 0x0401
module.ZCL_TEMP_MEASUREMENT_CLUSTER_ID                                    = 0x0402
module.ZCL_PRESSURE_MEASUREMENT_CLUSTER_ID                                = 0x0403
module.ZCL_FLOW_MEASUREMENT_CLUSTER_ID                                    = 0x0404
module.ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID                       = 0x0405
module.ZCL_OCCUPANCY_SENSING_CLUSTER_ID                                   = 0x0406
module.ZCL_CARBON_MONOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID           = 0x040C
module.ZCL_CARBON_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID            = 0x040D
module.ZCL_ETHYLENE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                  = 0x040E
module.ZCL_ETHYLENE_OXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID            = 0x040F
module.ZCL_HYDROGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID                  = 0x0410
module.ZCL_HYDROGEN_SULPHIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID         = 0x0411
module.ZCL_NITRIC_OXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID              = 0x0412
module.ZCL_NITROGEN_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID          = 0x0413
module.ZCL_OXYGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID                    = 0x0414
module.ZCL_OZONE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                     = 0x0415
module.ZCL_SULFUR_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID            = 0x0416
module.ZCL_DISSOLVED_OXYGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID          = 0x0417
module.ZCL_BROMATE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                   = 0x0418
module.ZCL_CHLORAMINES_CONCENTRATION_MEASUREMENT_CLUSTER_ID               = 0x0419
module.ZCL_CHLORINE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                  = 0x041A
module.ZCL_FECAL_COLIFORM_AND_E_COLI_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041B
module.ZCL_FLUORIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                  = 0x041C
module.ZCL_HALOACETIC_ACIDS_CONCENTRATION_MEASUREMENT_CLUSTER_ID          = 0x041D
module.ZCL_TOTAL_TRIHALOMETHANES_CONCENTRATION_MEASUREMENT_CLUSTER_ID     = 0x041E
module.ZCL_TOTAL_COLIFORM_BACTERIA_CONCENTRATION_MEASUREMENT_CLUSTER_ID   = 0x041F
module.ZCL_TURBIDITY_CONCENTRATION_MEASUREMENT_CLUSTER_ID                 = 0x0420
module.ZCL_COPPER_CONCENTRATION_MEASUREMENT_CLUSTER_ID                    = 0x0421
module.ZCL_LEAD_CONCENTRATION_MEASUREMENT_CLUSTER_ID                      = 0x0422
module.ZCL_MANGANESE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                 = 0x0423
module.ZCL_SULFATE_CONCENTRATION_MEASUREMENT_CLUSTER_ID                   = 0x0424
module.ZCL_BROMODICHLOROMETHANE_CONCENTRATION_MEASUREMENT_CLUSTER_ID      = 0x0425
module.ZCL_BROMOFORM_CONCENTRATION_MEASUREMENT_CLUSTER_ID                 = 0x0426
module.ZCL_CHLORODIBROMOMETHANE_CONCENTRATION_MEASUREMENT_CLUSTER_ID      = 0x0427
module.ZCL_CHLOROFORM_CONCENTRATION_MEASUREMENT_CLUSTER_ID                = 0x0428
module.ZCL_SODIUM_CONCENTRATION_MEASUREMENT_CLUSTER_ID                    = 0x0429
module.ZCL_IAS_ZONE_CLUSTER_ID                                            = 0x0500
module.ZCL_IAS_ACE_CLUSTER_ID                                             = 0x0501
module.ZCL_IAS_WD_CLUSTER_ID                                              = 0x0502
module.ZCL_GENERIC_TUNNEL_CLUSTER_ID                                      = 0x0600
module.ZCL_BACNET_PROTOCOL_TUNNEL_CLUSTER_ID                              = 0x0601
module.ZCL_11073_PROTOCOL_TUNNEL_CLUSTER_ID                               = 0x0614
module.ZCL_ISO7816_PROTOCOL_TUNNEL_CLUSTER_ID                             = 0x0615
module.ZCL_PRICE_CLUSTER_ID                                               = 0x0700
module.ZCL_DEMAND_RESPONSE_LOAD_CONTROL_CLUSTER_ID                        = 0x0701
module.ZCL_SIMPLE_METERING_CLUSTER_ID                                     = 0x0702
module.ZCL_MESSAGING_CLUSTER_ID                                           = 0x0703
module.ZCL_TUNNELING_CLUSTER_ID                                           = 0x0704
module.ZCL_PREPAYMENT_CLUSTER_ID                                          = 0x0705
module.ZCL_ENERGY_MANAGEMENT_CLUSTER_ID                                   = 0x0706
module.ZCL_CALENDAR_CLUSTER_ID                                            = 0x0707
module.ZCL_DEVICE_MANAGEMENT_CLUSTER_ID                                   = 0x0708
module.ZCL_EVENTS_CLUSTER_ID                                              = 0x0709
module.ZCL_MDU_PAIRING_CLUSTER_ID                                         = 0x070A
module.ZCL_SUB_GHZ_CLUSTER_ID                                             = 0x070B
module.ZCL_KEY_ESTABLISHMENT_CLUSTER_ID                                   = 0x0800
module.ZCL_INFORMATION_CLUSTER_ID                                         = 0x0900
module.ZCL_DATA_SHARING_CLUSTER_ID                                        = 0x0901
module.ZCL_GAMING_CLUSTER_ID                                              = 0x0902
module.ZCL_DATA_RATE_CONTROL_CLUSTER_ID                                   = 0x0903
module.ZCL_VOICE_OVER_ZIGBEE_CLUSTER_ID                                   = 0x0904
module.ZCL_CHATTING_CLUSTER_ID                                            = 0x0905
module.ZCL_PAYMENT_CLUSTER_ID                                             = 0x0A01
module.ZCL_BILLING_CLUSTER_ID                                             = 0x0A02
module.ZCL_APPLIANCE_IDENTIFICATION_CLUSTER_ID                            = 0x0B00
module.ZCL_METER_IDENTIFICATION_CLUSTER_ID                                = 0x0B01
module.ZCL_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_ID                          = 0x0B02
module.ZCL_APPLIANCE_STATISTICS_CLUSTER_ID                                = 0x0B03
module.ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID                              = 0x0B04
module.ZCL_DIAGNOSTICS_CLUSTER_ID                                         = 0x0B05
module.ZCL_ZLL_COMMISSIONING_CLUSTER_ID                                   = 0x1000
module.ZCL_SAMPLE_MFG_SPECIFIC_CLUSTER_ID                                 = 0xFC00
module.ZCL_OTA_CONFIGURATION_CLUSTER_ID                                   = 0xFC01
module.ZCL_MFGLIB_CLUSTER_ID                                              = 0xFC02

-- Foundation Command IDs
module.READ                                   = 0x00
module.READ_RSP                               = 0x01
module.WRITE                                  = 0x02
module.WRITE_UNDIVIDED                        = 0x03
module.WRITE_RSP                              = 0x04
module.WRITE_NO_RSP                           = 0x05
module.CONFIG_REPORT                          = 0x06
module.CONFIG_REPORT_RSP                      = 0x07
module.READ_REPORT_CFG                        = 0x08
module.READ_REPORT_CFG_RSP                    = 0x09
module.REPORT                                 = 0x0a
module.DEFAULT_RSP                            = 0x0b
module.DISCOVER_ATTRS                         = 0x0c
module.DISCOVER_ATTRS_RSP                     = 0x0d
module.DISCOVER_CMDS_RECEIVED                 = 0x11
module.DISCOVER_CMDS_RECEIVED_RSP             = 0x12
module.DISCOVER_CMDS_GEN                      = 0x13
module.DISCOVER_CMDS_GEN_RSP                  = 0x14
module.DISCOVER_ATTRS_EXT                     = 0x15
module.DISCOVER_ATTRS_EXT_RSP                 = 0x16
--/*** Data Types ***/
module.NO_DATA                            = 0x00
module.DATA8                              = 0x08
module.DATA16                             = 0x09
module.DATA24                             = 0x0a
module.DATA32                             = 0x0b
module.DATA40                             = 0x0c
module.DATA48                             = 0x0d
module.DATA56                             = 0x0e
module.DATA64                             = 0x0f
module.BOOLEAN                            = 0x10
module.BITMAP8                            = 0x18
module.BITMAP16                           = 0x19
module.BITMAP24                           = 0x1a
module.BITMAP32                           = 0x1b
module.BITMAP40                           = 0x1c
module.BITMAP48                           = 0x1d
module.BITMAP56                           = 0x1e
module.BITMAP64                           = 0x1f
module.UINT8                              = 0x20
module.UINT16                             = 0x21
module.UINT24                             = 0x22
module.UINT32                             = 0x23
module.UINT40                             = 0x24
module.UINT48                             = 0x25
module.UINT56                             = 0x26
module.UINT64                             = 0x27
module.INT8                               = 0x28
module.INT16                              = 0x29
module.INT24                              = 0x2a
module.INT32                              = 0x2b
module.INT40                              = 0x2c
module.INT48                              = 0x2d
module.INT56                              = 0x2e
module.INT64                              = 0x2f
module.ENUM8                              = 0x30
module.ENUM16                             = 0x31
module.SEMI_PREC                          = 0x38
module.SINGLE_PREC                        = 0x39
module.DOUBLE_PREC                        = 0x3a
module.OCTET_STR                          = 0x41
module.CHAR_STR                           = 0x42
module.LONG_OCTET_STR                     = 0x43
module.LONG_CHAR_STR                      = 0x44
module.ARRAY                              = 0x48
module.STRUCT                             = 0x4c
module.SET                                = 0x50
module.BAG                                = 0x51
module.TOD                                = 0xe0
module.DATE                               = 0xe1
module.UTC                                = 0xe2
module.CLUSTER_ID                         = 0xe8
module.ATTR_ID                            = 0xe9
module.BAC_OID                            = 0xea
module.IEEE_ADDR                          = 0xf0
module.BIT_SEC_KEY                        = 0xf1
module.UNKNOWN                            = 0xff


module.zcl_type=
{
	[0x00]=0,
	[0xff]=0,
	[0x08]=1,
	[0x10]=1,
	[0x18]=1,
	[0x20]=1,
	[0x28]=1,
	[0x30]=1,
	[0x09]=2,
	[0x19]=2,
	[0x21]=2,
	[0x29]=2,
	[0x31]=2,
	[0x38]=2,
	[0xe8]=2,
	[0xe9]=2,
	[0x0a]=3,
	[0x1a]=3,
	[0x22]=3,
	[0x2a]=3,
	[0x0b]=4,
	[0x1b]=4,
	[0x23]=4,
	[0x2b]=4,
	[0x39]=4,
	[0xe0]=4,
	[0xe1]=4,
	[0xe2]=4,
	[0xea]=4,
	[0x24]=5,
	[0x2c]=5,
	[0x25]=6,
	[0x2d]=6,	
	[0x26]=7,
	[0x2e]=7,	
	[0x3a]=8,
	[0xf0]=8,
	[0x27]=8,
	[0x2f]=8,	
	[0xf1]=16,
}

module.zcl_analogtype=
{

	[0x20]=1,
	[0x21]=1,
	[0x22]=1,
	[0x23]=1,	
	[0x24]=1,
	[0x25]=1,
	[0x26]=1,
	[0x27]=1,
	[0x28]=1,
	[0x29]=1,	
	[0x2a]=1,
	[0x2b]=1,
	[0x2c]=1,
	[0x2d]=1,
	[0x2e]=1,
	[0x2f]=1,
	[0x38]=1,
	[0x39]=1,
	[0x3a]=1,
	[0xe0]=1,
	[0xe1]=1,
	[0xe2]=1,	
}

function module.zcl_analog(data_type,buff)

	if(module.zcl_analogtype[data_type] ==1)then
		return buff:reverse()  -- 翻转
	else
		return buff
	end
end

function module.zcl_len(buff,data_type)

	local data_len=0
	if(data_type==module.LONG_CHAR_STR or data_type==module.LONG_OCTET_STR)then
		data_len=module.little_Uint16(buff:byte(1,2))
		
		return data_len,2 -- index=2
		
	elseif(data_type==module.CHAR_STR or data_type==module.OCTET_STR)then
		data_len=buff:byte(1,1)
		return data_len,1 -- index=1
	else
		return module.zcl_type[data_type],0 -- index=0
	end
end

function module.zcl_index(data_type)

	if(data_type==module.LONG_CHAR_STR or data_type==module.LONG_OCTET_STR)then
		return 2 -- index=2
		
	elseif(data_type==module.CHAR_STR or data_type==module.OCTET_STR)then
		return 1 -- index=1
	else
		return 0 -- index=0
	end
end

function module.little_Uint16(low1,high2)
	if(aux_core.IsNumber(low1) and aux_core.IsNumber(high2) )then
		return high2*256 + low1
	end
end


function module.little_Uint32(byte1,byte2,byte3,byte4)
	if(aux_core.IsNumber(byte1) and aux_core.IsNumber(byte1) and aux_core.IsNumber(byte3) and aux_core.IsNumber(byte4) )then
		return (byte4 <<24) + (byte3 <<16) + (byte2 <<8) + byte1
	end
end

function module.Hi16(n)
	return (n >> 8)&0xff
end

function module.Lo16(n)
	return n & 0xff
end

function module.H2S(buff)
    return aux_core.StrToHexStr(buff)
end

function module.S2H(buff)
	return aux_core.HexStrToStr(buff)
end

function module.byteToHexC(n)
	local data = "%02X"
		return string.format(data, n )
end

function module.intToHexC(n)
	local buff={}
	buff[1]=module.byteToHexC(n & 0xff)
	buff[2]=module.byteToHexC((n>>8) & 0xff)
	buff[3]=module.byteToHexC((n>>16) & 0xff)
	buff[4]=module.byteToHexC((n>>24) & 0xff)
	
	return table.concat(buff)
end

function module.shortToHexC(n)
	local buff={}
	buff[1] = module.byteToHexC(n & 0xff)
	buff[2] = module.byteToHexC((n>>8) & 0xff)
	
	return table.concat(buff)
end

local cmdtostr_switch=
{
--[[
local attribute_read_req=
{
	0x0000,
	0x0001,
	0x0003,
	0x0004
}
]]--
	[0]=function(read_req)
	
		local read_buff={}
		
		for i = 1, #read_req, 1 do
			read_buff[i]=module.shortToHexC(read_req[i])						
		end
		local s=table.concat(read_buff)
		print("read_req::" .. s)
		return aux_core.HexStrToStr(s)
	end,	
--[[
local attribute_write_req=
{
	{
		attr_id=0x0001,
		data_type=21,
		data_len =15,
		payload="0102030405",
	},
}
]]--	
	[2]=function(write_req)
	
		local write_buff={}
		
		for i = 1, #write_req, 1 do
		
			table.insert(write_buff,module.shortToHexC(write_req[i].attr_id))
			table.insert(write_buff,module.byteToHexC(write_req[i].data_type))
			
			local index= module.zcl_index(write_req[i].data_type)
				if(2 == index)then
					table.insert(write_buff,module.shortToHexC(write_req[i].data_len))	
				elseif(1==index)then
					table.insert(write_buff,module.byteToHexC(write_req[i].data_len))
				end
				table.insert(write_buff,write_req[i].payload)			
		end
		local s=table.concat(write_buff)
		print("write_req::",s)
		return aux_core.HexStrToStr(s)
	end,
}
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
local strtocmd_switch=
{

--[[
local attribute_read_rsp=
{
	[1]={
		attr_id=0x0001,
		data_type=21,
		data_len=1，
		payload="0102030405"
	},
}
]]--
	[1]=function(read_rspbuff)
	
		local read_rsp={}
		local i = 1
		local m=1
		

		while(i<#read_rspbuff)do
			local node={}
			local aid= module.little_Uint16(read_rspbuff:byte(i,i+1))
			i=i+2
			local t=read_rspbuff:byte(i)
			i=i+1
			if(t == 0)then
				node.attr_id=aid
				node.data_type=read_rspbuff:byte(i)
				i=i+1
				node.data_len,index =module.zcl_len(read_rspbuff:sub(i),node.data_type)		
			
				i=i+index

				node.payload = read_rspbuff:sub(i,i+node.data_len)
				i=i+node.data_len
				
				read_rsp[m]=node
				m=m+1
			else
				print("read_rsp error",t,aid)
			end

		end
	
		return read_rsp
	end,	
--[[
local attribute_write_rsp=
{
	[1]={
		attr_id=0x0002,
		status = 0x00,
	},
	[2]={
		attr_id=0x0001,
		status = 0x00,
	},	
}
]]--
	[4]=function(write_buff)
	
		local write_rsp ={}
		local i = 1
		local m=1
		
		while(i<#write_buff) do
			local node={}
			node.attr_id= module.little_Uint16(write_buff:byte(i,i+1))
			i=i+2
			node.status = write_buff:byte(i)
			
			write_rsp[m]=node
			i=i+1
			m=m+1	
			
		end
		
		return write_rsp
	end,
--[[
local attribute_report_buff=
{
	[1]={
		attr_id=0x0002,
		data_type = 0x00,
		data_len=5
		payload="0102030405",
	},	
}
]]--	
	[10]=function(report_buff)
	
		local report ={}
		local i = 1
		local m=1
		print(#report_buff)
		
		while(i<#report_buff) do
		
			local node={}
--			print("report")
			node.attr_id= module.little_Uint16(report_buff:byte(i,i+1))
			i=i+2
			
			node.data_type = report_buff:byte(i)
			i=i+1
			
			node.data_len ,index =module.zcl_len(report_buff:sub(i),node.data_type)
			
			i=i+index
						
			node.payload = report_buff:sub(i,i+node.data_len)				
			report[m]=node
			
			i=i+node.data_len
			m=m+1
		end
		
		return report
		
	end,
--[[
local attribute_default_rsp=
{
	cmd_id=0x02,
	status_code = 0x00,
}
]]--	
	[11]=function(default_buff)
	
		local default_rsp={}
		
		default_rsp.cmd_id = default_buff:byte(1,1)
		default_rsp.status_code=  default_buff:byte(2,2)
			
		return default_rsp
	end,	
}
--[[
local f=module.zcl_cmd_switch[cmd_id]
if(f)then
	f(data)
end
]]--
function module.strtocmd(cmd_id,data)

	local f=strtocmd_switch[cmd_id]
	if(f)then
		return f(data)
	end
	return nil
end

function module.cmdtostr(cmd_id,data)

	local f=cmdtostr_switch[cmd_id]
	if(f)then
		return f(data)
	end
	return nil
end

return module
