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
#include EMBER_AF_API_COMMAND_RELAY
#include EMBER_AF_API_LINKED_LIST

#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "stack/include/trust-center.h"

#include <sys/time.h>
#include <stdlib.h>

#if 0
{ .commandString = COMMAND_STRING_OTA_RELOAD,
  .functionPtr = &emAfPluginGatewayRelayMqttOtaStorageCommonReload },
{ .commandString = COMMAND_STRING_OTA_POLICY_QUERY,
  .functionPtr = &emAfPluginGatewayRelayMqttOtaServerPolicyQuery },
{ .commandString = COMMAND_STRING_OTA_NOTIFY,
  .functionPtr = &emAfPluginGatewayRelayMqttOtaServerNotify },
#endif

static void publishMqttOtaEvent(char* messageType,
                                EmberEUI64 eui64,
                                uint8_t* status,
                                uint32_t* blocksSent,
                                uint8_t* blockSize,
                                uint16_t* manufacturerId,
                                uint16_t* imageTypeId,
                                uint32_t* firmwareVersion)
{
  char euiString[EUI64_STRING_LENGTH] = { 0 };
  char manufacturerIdString[TWO_BYTE_HEX_STRING_SIZE] = { 0 };
  char imageTypeIdString[TWO_BYTE_HEX_STRING_SIZE] = { 0 };
  char firmwareVersionString[FOUR_BYTE_HEX_STRING_SIZE] = { 0 };
  cJSON* otaJson;
  char* statusString;

  eui64ToString(eui64, euiString);

  otaJson = cJSON_CreateObject();
  cJSON_AddStringToObject(otaJson, "messageType", messageType);
  cJSON_AddStringToObject(otaJson, "eui64", euiString);
  if (status) {
    statusString = createOneByteHexString(*status);
    cJSON_AddStringToObject(otaJson, "status", statusString);
    free(statusString);
  }
  if (blocksSent && blockSize) {
    cJSON_AddIntegerToObject(otaJson,
                             "bytesSent",
                             (*blocksSent) * (*blockSize));
  }

  if (manufacturerId) {
    sprintf(manufacturerIdString, "0x%04X", *manufacturerId);
    cJSON_AddStringToObject(otaJson, "manufacturerId", manufacturerIdString);
  }
  if (imageTypeId) {
    sprintf(imageTypeIdString, "0x%04X", *imageTypeId);
    cJSON_AddStringToObject(otaJson, "imageTypeId", imageTypeIdString);
  }
  if (firmwareVersion) {
    sprintf(firmwareVersionString, "0x%08X", *firmwareVersion);
    cJSON_AddStringToObject(otaJson, "firmwareVersion", firmwareVersionString);
  }
  publishMqttTopic("otaevent", otaJson);
}

//plugin specific callbacks
void emberAfPluginOtaServerUpdateCompleteCallback(uint16_t manufacturerId,
                                                  uint16_t imageTypeId,
                                                  uint32_t firmwareVersion,
                                                  EmberNodeId nodeId,
                                                  uint8_t status)
{
  char* messageType = (status == EMBER_ZCL_STATUS_SUCCESS) ? "otaFinished"
                      : "otaFailed";

  EmberEUI64 nodeEui64;
  emberAfDeviceTableGetEui64FromNodeId(nodeId, nodeEui64);

  publishMqttOtaEvent(messageType,
                      nodeEui64,
                      &status,
                      NULL, // blockSent is unused
                      NULL, // actualLength is unused
                      &manufacturerId,
                      &imageTypeId,
                      &firmwareVersion);

  // Note that this global block sent count only supports 1 OTA
  otaBlockSent = 0;
}

void emberAfPluginOtaServerBlockSentCallback(uint8_t actualLength,
                                             uint16_t manufacturerId,
                                             uint16_t imageTypeId,
                                             uint32_t firmwareVersion)
{
  // Use a throttle value here to control the amount of updates being published
  if (otaBlockSent % BLOCK_SENT_THROTTLE_VALUE == 0) {
    EmberNodeId nodeId = emberAfCurrentCommand()->source;
    EmberEUI64 nodeEui64;
    emberAfDeviceTableGetEui64FromNodeId(nodeId, nodeEui64);
    publishMqttOtaEvent("otaBlockSent",
                        nodeEui64,
                        NULL, // status is unused
                        &otaBlockSent,
                        &actualLength,
                        &manufacturerId,
                        &imageTypeId,
                        &firmwareVersion);
  }

  otaBlockSent++;
}

void emberAfPluginOtaServerUpdateStartedCallback(uint16_t manufacturerId,
                                                 uint16_t imageTypeId,
                                                 uint32_t firmwareVersion,
                                                 uint8_t maxDataSize,
                                                 uint32_t offset)
{
  // Note that this global block sent count only supports 1 OTA
  otaBlockSent = 0;
  EmberNodeId nodeId = emberAfCurrentCommand()->source;
  EmberEUI64 nodeEui64;
  emberAfDeviceTableGetEui64FromNodeId(nodeId, nodeEui64);
  publishMqttOtaEvent("otaStarted",
                      nodeEui64,
                      NULL, // status is unused
                      NULL, // blockSent is unused
                      NULL, // actualLength is unused
                      &manufacturerId,
                      &imageTypeId,
                      &firmwareVersion);
}
												 
//-------------------------------------------------------------------------------------------
//================================CALL

void emAfPluginGatewayRelayMqttOtaServerNotify(const uint8_t * argString)
{
		EmberAfOtaImageId id;
		EmberNodeId dest;
		uint8_t endpoint;
		uint8_t payloadType;
		uint8_t jitter;

		sscanf(argString,
		"%hx %hhx %hhx %hhx %hx %hx %x",
		&dest,
		&endpoint,
		&payloadType,
		&jitter,
		&id.manufacturerId,
		&id.imageTypeId,
		&id.firmwareVersion);
		emberAfOtaServerSendImageNotifyCallback(dest,
									   endpoint,
									   payloadType,
									   jitter,
									   &id);
}

void emAfPluginGatewayRelayMqttOtaServerPolicyQuery(const uint8_t * argString)
{
	uint8_t value;

	sscanf(argString, "%hhx", &value);
	emAfOtaServerSetQueryPolicy(value);
}

void emAfPluginGatewayRelayMqttOtaStorageCommonReload(void)
{
#if defined (EMBER_AF_PLUGIN_OTA_STORAGE_POSIX_FILESYSTEM)
	emAfOtaStorageClose();
#endif

	emberAfOtaStorageInitCallback();
}

//================================CALL


