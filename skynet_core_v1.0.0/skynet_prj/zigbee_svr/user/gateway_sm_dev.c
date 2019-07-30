/***************************************************************************//**
 * @file
 * @brief Definitions for the Gateway Relay MQTT plugin.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "af.h"
#include "app/framework/util/af-main.h"
#include "app/framework/util/util.h"
#include EMBER_AF_API_DEVICE_TABLE
#include EMBER_AF_API_LINKED_LIST

#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "stack/include/trust-center.h"

#include <sys/time.h>
#include <stdlib.h>

#include "lua_zigbeepush.h"

#if 0
//command struct array definition
static const EmAfPluginGatewayRelayMqttCommand commandArray[] = {

  { .commandString = COMMAND_STRING_DEVICE_TABLE_CLEAR,
    .functionPtr = &emAfPluginGatewayRelayMqttDeviceTableClear },
};
#endif

void gateway_sm_devicepush(EmberAfPluginDeviceTableEntry *dev)
{	
	int inlen=0,outlen=0;
	uint8_t incluster[20]={0};
	uint8_t outcluster[20]={0};
	uint16_t clusterIdIndex;
	
	//	deviceTable[nodeIndex].nodeId;
	//	deviceTable[nodeIndex].state;
	//	deviceTable[nodeIndex].deviceId;
	//	timeSinceLastMessage -= deviceTable[nodeIndex].lastMsgTimestamp;
	//	timeSinceLastMessage /= MILLISECOND_TICKS_PER_SECOND;

 for (clusterIdIndex = 0;
       clusterIdIndex < EMBER_AF_PLUGIN_DEVICE_TABLE_CLUSTER_SIZE;
       clusterIdIndex++) 
      {
			if (dev->clusterIds[clusterIdIndex] != ZCL_NULL_CLUSTER_ID) 
			{
				if (clusterIdIndex < dev->clusterOutStartPosition) 
				{
					incluster[inlen++]=dev->clusterIds[clusterIdIndex]&0xff;
					incluster[inlen++]=(dev->clusterIds[clusterIdIndex]>>8)&0xff;
				} 
				else 
				{
					outcluster[outlen++]=dev->clusterIds[clusterIdIndex]&0xff;
					outcluster[outlen++]=(dev->clusterIds[clusterIdIndex]>>8)&0xff;			
				}
			} 
  	}	
	lua_dev_online(dev->eui64, dev->endpoint,dev->deviceId,incluster,inlen,outcluster,outlen);
}


static void _buildNodeinfo(uint16_t nodeIndex)
{
  EmberAfPluginDeviceTableEntry *deviceTable = emberAfDeviceTablePointer();

  gateway_sm_devicepush(&deviceTable[nodeIndex]);

}
//========================================================================================================

void  gateway_sm_publishDevices(void)
{
  uint16_t nodeIndex;

  for (nodeIndex = 0;nodeIndex < EMBER_AF_PLUGIN_DEVICE_TABLE_DEVICE_TABLE_SIZE; nodeIndex++) 
  {
    if(emberAfDeviceTableGetNodeIdFromIndex(nodeIndex)!= EMBER_AF_PLUGIN_DEVICE_TABLE_NULL_NODE_ID) 
	{
      _buildNodeinfo(nodeIndex);
    }
  }
}
						  
//=====================================================================================================
static void gateway_sm_publishdevicejoined(EmberEUI64 eui64)
{
	uint16_t deviceTableIndex;

	deviceTableIndex = emberAfDeviceTableGetFirstIndexFromEui64(eui64);
	_buildNodeinfo(deviceTableIndex);
}

void emberAfPluginDeviceTableNewDeviceCallback(EmberEUI64 nodeEui64)
{
 	 gateway_sm_publishdevicejoined(nodeEui64);
}

void emberAfPluginDeviceTableEpCallback(EmberEUI64 nodeEui64,int cont,uint8_t*list)
{
 	 lua_dev_ep(nodeEui64,cont,list);
}

//=====================================================================================================

static void gateway_sm_publishdeviceleft(EmberEUI64 eui64)
{
	lua_dev_left(eui64);
}

void emberAfPluginDeviceTableDeviceLeftCallback(EmberEUI64 nodeEui64)
{
  gateway_sm_publishdeviceleft(nodeEui64);
}
//=====================================================================================================
// dev online again
void emberAfPluginDeviceTableRejoinDeviceCallback(EmberEUI64 nodeEui64)
{
  gateway_sm_publishdevicejoined(nodeEui64);
}
//=====================================================================================================

static void gateway_sm_publishdevicestatechange(EmberEUI64 eui64,uint8_t state)
{
	lua_dev_change(eui64,state);
}

void emberAfPluginDeviceTableStateChangeCallback(EmberNodeId nodeId,uint8_t state)
{
  EmberEUI64 nodeEui64;
  emberAfDeviceTableGetEui64FromNodeId(nodeId, nodeEui64);
  gateway_sm_publishdevicestatechange(nodeEui64, state);
}

//=====================================================================================================
// API
//=====================================================================================================

void emberAfPluginDeviceTableClearedCallback(void)
{
  
}

void gateway_sm_devicetableClear(void)
{
	emberAfDeviceTableClear();
}



