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
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "stack/include/trust-center.h"
#include EMBER_AF_API_DEVICE_TABLE
#include EMBER_AF_API_COMMAND_RELAY
#include EMBER_AF_API_NETWORK_CREATOR
#include EMBER_AF_API_NETWORK_CREATOR_SECURITY
#include "app/framework/plugin/ota-common/ota-cli.h"
#include "app/framework/plugin/ota-server-policy/ota-server-policy.h"
#include "app/framework/plugin/ota-server/ota-server.h"
#include "app/framework/plugin/ota-storage-simple/ota-storage-simple-driver.h"
#include "app/framework/plugin/ota-storage-common/ota-storage.h"
#include "app/framework/plugin/ias-zone-client/ias-zone-client.h"
#include "gateway-relay-mqtt.h"
#include "gateway-relay-mqtt-commands.h"

#include <stdlib.h>
#include <string.h>


//------------------------------------------------------------------------------------------

static void publishMqttBindResponse(EmberNodeId nodeId,EmberApsFrame* apsFrame, uint8_t* message,uint16_t length)
{
  cJSON* objectJson;
  char* dataString;
  EmberEUI64 eui64;
  char euiString[EUI64_STRING_LENGTH] = { 0 };

  objectJson = cJSON_CreateObject();

  cJSON_AddStringToObject(objectJson, "zdoType", "bindResponse");

  emberAfDeviceTableGetEui64FromNodeId(nodeId, eui64);
  eui64ToString(eui64, euiString);
  cJSON_AddStringToObject(objectJson, "eui64", euiString);

  dataString = createOneByteHexString(
    message[DEVICE_TABLE_BIND_RESPONSE_STATUS]);
  cJSON_AddStringToObject(objectJson, "status", dataString);
  free(dataString);
  publishMqttTopic(ZDO_RESPONSE_TOPIC, objectJson);
}

static void publishMqttBindTableReponse(EmberNodeId nodeId,
                                        EmberApsFrame* apsFrame,
                                        uint8_t* message,
                                        uint16_t length)
{
  cJSON* objectJson;
  cJSON* entryArrayJson;
  cJSON* tableEntryJson;
  cJSON* deviceEndpointJson;
  uint8_t* messagePointer;
  uint8_t numEntries, entryCounter;
  char* dataString;
  char euiString[EUI64_STRING_LENGTH] = { 0 };
  EmberEUI64 eui64;

  numEntries = message[BINDING_TABLE_RESPONSE_NUM_ENTRIES]; // list count

  objectJson = cJSON_CreateObject();
  entryArrayJson = cJSON_CreateArray();

  cJSON_AddStringToObject(objectJson, "zdoType", "bindTableResponse");

  dataString = createOneByteHexString(message[BINDING_TABLE_RESPONSE_STATUS]);
  cJSON_AddStringToObject(objectJson, "status", dataString);
  free(dataString);

  emberAfDeviceTableGetEui64FromNodeId(nodeId, eui64);
  eui64ToString(eui64, euiString);
  cJSON_AddStringToObject(objectJson, "eui64", euiString);

  messagePointer = message + BINDING_TABLE_RESPONSE_ENTRIES;

  for (entryCounter = 0; entryCounter < numEntries; entryCounter++) {
    tableEntryJson = cJSON_CreateObject();

    deviceEndpointJson = buildDeviceEndpoint(
      &(messagePointer[BINDING_ENTRY_EUI]),
      messagePointer[BINDING_ENTRY_SOURCE_ENDPOINT]);
    cJSON_AddItemToObject(tableEntryJson,
                          "sourceDeviceEndpoint",
                          deviceEndpointJson);

    cJSON_AddIntegerToObject(tableEntryJson,
                             "addressMode",
                             messagePointer[BINDING_ENTRY_ADDRESS_MODE]);

    dataString =
      createTwoByteHexString(
        HIGH_LOW_TO_INT(messagePointer[BINDING_ENTRY_CLUSTER_ID + 1],
                        messagePointer[BINDING_ENTRY_CLUSTER_ID]));
    cJSON_AddStringToObject(tableEntryJson, "clusterId", dataString);
    free(dataString);

    deviceEndpointJson = buildDeviceEndpoint(
      &(messagePointer[BINDING_ENTRY_DEST_EUI]),
      messagePointer[BINDING_ENTRY_DEST_ENDPOINT]);
    cJSON_AddItemToObject(tableEntryJson,
                          "destDeviceEndpoint",
                          deviceEndpointJson);

    cJSON_AddItemToArray(entryArrayJson, tableEntryJson);

    messagePointer += BINDING_TABLE_RESPONSE_ENTRY_SIZE;
  }

  cJSON_AddItemToObject(objectJson, "bindTable", entryArrayJson);
  publishMqttTopic(ZDO_RESPONSE_TOPIC, objectJson);
}

bool gateway_sm_PreZDOMessageReceivedCallback(EmberNodeId emberNodeId, EmberApsFrame* apsFrame,uint8_t* message,uint16_t length)
{
  switch (apsFrame->clusterId) 
  {
    case ACTIVE_ENDPOINTS_RESPONSE:
		
  
      break;
    case SIMPLE_DESCRIPTOR_RESPONSE:
      break;
    case END_DEVICE_ANNOUNCE:

	
      break;
    case PERMIT_JOINING_RESPONSE:
      break;
    case LEAVE_RESPONSE:
      break;
    case BIND_RESPONSE:
      publishMqttBindResponse(emberNodeId, apsFrame, message, length);
      break;
    case BINDING_TABLE_RESPONSE:
      publishMqttBindTableReponse(emberNodeId, apsFrame, message, length);
      break;
    case NETWORK_ADDRESS_RESPONSE:
      break;
    case IEEE_ADDRESS_RESPONSE:
      break;
    default:
      break;
  }
  return false;
}
  
  bool emberAfPreZDOMessageReceivedCallback(EmberNodeId emberNodeId,
											EmberApsFrame* apsFrame,
											uint8_t* message,
											uint16_t length)
  {
  	return gateway_sm_PreZDOMessageReceivedCallback(emberNodeId,apsFrame,message,length);
  }


  

void gateway_sm_ZdoBind(EmberEUI64 destEui,uint8_t destinationEndpoint,uint16_t clusterId)
{
  EmberStatus status;
  EmberEUI64 sourceEui, destEui;// names relative to binding sent over-the-air
  EmberNodeId target;
  
  target = emberAfDeviceTableGetFirstIndexFromEui64(destEui);
  emberAfGetEui64(sourceEui);


  status = emberBindRequest(target,           // who gets the bind req
                            sourceEui,       // source eui IN the binding
                            1,
                            clusterId,
                            UNICAST_BINDING, // binding type
                            destEui,         // destination eui IN the binding
                            0,               // groupId for new binding
                            destinationEndpoint,
                            EMBER_AF_DEFAULT_APS_OPTIONS);
  UNUSED_VAR(status);
}

//--------------------------------------------------------------------------------------------


