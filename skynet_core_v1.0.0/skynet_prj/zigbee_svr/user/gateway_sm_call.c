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

#include "lua_zigbeepush.h"

#include <sys/time.h>
#include <stdlib.h>

//==================================LOG==================================================================================  

#define COMMAND_OFFSET 2

//Non-cluster related callbacks
bool emberAfMessageSentCallback(EmberOutgoingMessageType type,
                                uint16_t indexOrDestination,
                                EmberApsFrame* apsFrame,
                                uint16_t msgLen,
                                uint8_t* message,
                                EmberStatus status)
{
	EmberEUI64 eui64;
	emberAfDeviceTableGetEui64FromNodeId((EmberNodeId)indexOrDestination, eui64);

  	lua_aps_sent(eui64,status, apsFrame->clusterId,message[COMMAND_OFFSET]);

  // track the state of the device, except for broadcasts
  if (emberIsZigbeeBroadcastAddress(indexOrDestination)) 
  {
    return false;
  }

  emberAfPluginDeviceTableMessageSentStatus(indexOrDestination,
                                            status,
                                            apsFrame->profileId,
                                            apsFrame->clusterId);

  if (status != EMBER_SUCCESS) {
    emberAfAppPrintln("%2x failed with code %x",
                      indexOrDestination,
                      status);

    if (indexOrDestination >= EMBER_DISCOVERY_ACTIVE_NODE_ID) {
      return false;
    }
    emberAfPluginDeviceTableInitiateRouteRepair(indexOrDestination);
  }

  return false;
}
								
//-------------------------------------------------------------------------------------------
							
//host recive msg 
bool emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incomingMessage)
{
	EmberEUI64 eui64;
	if(emberAfDeviceTableGetEui64FromNodeId((EmberNodeId)incomingMessage->source,eui64))
	{
		lua_rssi_report(eui64, incomingMessage->lastHopRssi,incomingMessage->lastHopLqi);
	}

    return false;
}

//-------------------------------------------------------------------------------------------

