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
#include "gateway_sm_dev.h"
#include <sys/time.h>
#include <stdlib.h>
#include "gateway_sm_main.h"

#include "lua_obj.h"
#include "lua_zigbeepush.h"


struct gateway_stata G;

void gateway_sm_publishSettings(void)
{
  EmberNetworkParameters parameters;
  EmberNodeType nodeType;
  EmberVersion versionStruct;
  uint8_t ncpEzspProtocolVer;
  uint8_t ncpStackType;
  uint16_t ncpStackVer;
  uint8_t hostEzspProtocolVer = EZSP_PROTOCOL_VERSION;

  memset(&G,0,sizeof(struct gateway_stata));
  
   
  // Save our EUI information
  emberAfGetEui64(G.gatewayEui64);

  EmberStatus status = emberAfGetNetworkParameters(&nodeType, &parameters);

  ncpEzspProtocolVer = ezspVersion(hostEzspProtocolVer,
                                   &ncpStackType,
                                   &ncpStackVer);

  if (EZSP_SUCCESS == ezspGetVersionStruct(&versionStruct)) {
    sprintf(G.ncpStackVerString,
            "%d.%d.%d-%d",
            versionStruct.major,
            versionStruct.minor,
            versionStruct.patch,
            versionStruct.build);
  } 
  else 
  {
    sprintf(G.ncpStackVerString, "%2x", ncpStackVer);
  }

  if (!emberAfNcpNeedsReset() && status == EMBER_SUCCESS) 
  	{
  		
  		printf("ncpv=%s\n",G.ncpStackVerString);
		printf("radioTxPower=%d\n",parameters.radioTxPower);//+22
		printf("radioChannel=%d\n",parameters.radioChannel);
		printf("panId=%x\n",parameters.panId);

		G.panId=parameters.panId;
		G.power=parameters.radioTxPower;
		G.radioChannel=parameters.radioTxPower;

  } 
  lua_network_state(G.gatewayEui64,G.power,G.panId, G.radioChannel,G.ncpStackVerString);

}
//-----------------------------------------------------------------------------------------------------------
 
 EmberApsFrame localApsFrame;
 
 void setApsFrame(uint16_t clusterId)
 {
   localApsFrame.options = EMBER_AF_DEFAULT_APS_OPTIONS;
   localApsFrame.clusterId = clusterId;
 }

 uint8_t gateway_sm_ZclSend(EmberEUI64 eui64,uint8_t endpoint,uint8_t *appZclBuffer,uint16_t appZclBufferLen)
 {
   uint16_t index;
   EmberNodeId nodeId;
 
   index = emberAfDeviceTableGetIndexFromEui64AndEndpoint(eui64, endpoint);
 
   if (index != EMBER_AF_PLUGIN_DEVICE_TABLE_NULL_INDEX) {
	 nodeId = emberAfDeviceTableGetNodeIdFromIndex(index);
	 localApsFrame.sourceEndpoint = 1;
	 localApsFrame.destinationEndpoint = endpoint;
	 return  emberAfSendUnicast(EMBER_OUTGOING_DIRECT,
								 nodeId,
								 &localApsFrame,
								 appZclBufferLen,
								 appZclBuffer);
   }

   return 1;
 }
 


boolean emberAfStackStatusCallback(EmberStatus status)
{
  	if (status == EMBER_NETWORK_DOWN) 
	{
		gateway_sm_publishSettings();
	} 
	else if(status == EMBER_NETWORK_UP) 
	{
//		gateway_sm_publishSettings();
		gateway_sm_publishDevices();
	}
	else if(status == EMBER_JOIN_FAILED)
	{

	}
	else if(status == EMBER_MOVE_FAILED)// ¹Â¶ùÇëÇóÊ§°Ü
	{

	}
  return false;
}


void emberAfMainInitCallback(void)
{
  printf("emberAfMainInitCallback\n");      
  lua_vm_init();
}




