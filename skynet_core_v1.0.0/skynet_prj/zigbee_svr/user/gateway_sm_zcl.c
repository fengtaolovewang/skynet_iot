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

#include "gateway_sm_main.h"
#include "lua_zigbeepush.h"

#include <sys/time.h>
#include <stdlib.h>

#define ZCL_BUFFER_SIZE 255

static uint8_t zclFrameControlDirection = ZCL_FRAME_CONTROL_CLIENT_TO_SERVER;
uint8_t appZclBuffer[ZCL_BUFFER_SIZE];
uint16_t appZclBufferLen;

								  
//bookkeeping callbacks
bool gateway_sm_PreCommandReceivedCallback( EmberAfClusterCommand* cmd)
{
EmberEUI64 eui64;
EmberNodeId nodeId;
uint8_t sourceEndpoint;
EmberAfClusterId clusterId = cmd->apsFrame->clusterId;
uint8_t zclCmd = cmd->commandId;
uint8_t *message = cmd->buffer+cmd->payloadStartIndex;
uint16_t msgLen = cmd->bufLen;
uint8_t type=cmd->clusterSpecific;

sourceEndpoint = emberAfCurrentCommand()->apsFrame->sourceEndpoint;
nodeId = emberAfCurrentCommand()->source;
emberAfDeviceTableGetEui64FromNodeId(nodeId, eui64);

	if(clusterId==ZCL_OTA_BOOTLOAD_CLUSTER_ID || (clusterId==ZCL_IAS_ZONE_CLUSTER_ID && zclCmd==ZCL_ZONE_ENROLL_REQUEST_COMMAND_ID))
	{
		return false;
	}
	else
	{
		lua_zcl_cmd(eui64,sourceEndpoint,type,clusterId,zclCmd,message,msgLen);
		return true;
	}

/*
publishMqttZclCommand(cmd->commandId,
					  cmd->clusterSpecific,
					  cmd->apsFrame->clusterId,
					  cmd->mfgSpecific,
					  cmd->mfgCode,
					  cmd->buffer,
					  cmd->bufLen,
					  cmd->payloadStartIndex);*/
}
//zcl cmd
bool emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd)
{
	return gateway_sm_PreCommandReceivedCallback(cmd);
}

								  

  void gateway_sm_ZclGlobalDirection(uint8_t zclDirection)
  {
  	zclFrameControlDirection = ((zclDirection == ZCL_DIRECTION_CLIENT_TO_SERVER)? ZCL_FRAME_CONTROL_CLIENT_TO_SERVER: ZCL_FRAME_CONTROL_SERVER_TO_CLIENT);
  }
 
  //-----------------------------------------------------------------------------------------------------------
  
  void gateway_sm_IasZoneClientClearAll(void)
  {
	emAfClearServers();
  }
  
  //--------------------------------------------------------------------------------------------
  
  void gateway_sm_ZclOnOff(uint8_t* Eui64,uint8_t ep,uint8_t command)
  {
	uint8_t index = 0;
  
	setApsFrame(ZCL_ON_OFF_CLUSTER_ID);
	appZclBuffer[index++] = (ZCL_CLUSTER_SPECIFIC_COMMAND| ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
	appZclBuffer[index++] = emberAfNextSequence();
	appZclBuffer[index++] = command;
  
	appZclBufferLen = index;
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
  }
  
  void gateway_sm_ZclOnAndMoveToLevel(uint8_t* Eui64,uint8_t ep,uint8_t level,uint16_t transTime)
  {

	uint8_t index = 0;
    
	setApsFrame(ZCL_LEVEL_CONTROL_CLUSTER_ID);
	appZclBuffer[index++] = (ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
	appZclBuffer[index++] = emberAfNextSequence();
	appZclBuffer[index++] = ZCL_MOVE_TO_LEVEL_WITH_ON_OFF_COMMAND_ID;
	appZclBuffer[index++] = level;
	appZclBuffer[index++] = LOW_BYTE(transTime);
	appZclBuffer[index++] = HIGH_BYTE(transTime);
  
	appZclBufferLen = index;
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
  }
  
  void gateway_sm_ZclMoveToColorTemp(uint8_t* Eui64,uint8_t ep,uint16_t colorTemp,uint16_t transTime)
  {

	uint8_t index = 0;
    
	setApsFrame(ZCL_COLOR_CONTROL_CLUSTER_ID);
	appZclBuffer[index++] = (ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
	appZclBuffer[index++] = emberAfNextSequence();
	appZclBuffer[index++] = ZCL_MOVE_TO_COLOR_TEMPERATURE_COMMAND_ID;
	appZclBuffer[index++] = LOW_BYTE(colorTemp);
	appZclBuffer[index++] = HIGH_BYTE(colorTemp);
	appZclBuffer[index++] = LOW_BYTE(transTime);
	appZclBuffer[index++] = HIGH_BYTE(transTime);
  
	appZclBufferLen = index;
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
  }
  
  void gateway_sm_ZclMoveToHueAndSat(uint8_t* Eui64,uint8_t ep,uint8_t hue,uint8_t sat,uint16_t transTime)
  {

	uint8_t index = 0;
    
	setApsFrame(ZCL_COLOR_CONTROL_CLUSTER_ID);
	appZclBuffer[index++] = (ZCL_CLUSTER_SPECIFIC_COMMAND| ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
	appZclBuffer[index++] = emberAfNextSequence();
	appZclBuffer[index++] = ZCL_MOVE_TO_HUE_AND_SATURATION_COMMAND_ID;
	appZclBuffer[index++] = hue;
	appZclBuffer[index++] = sat;
	appZclBuffer[index++] = LOW_BYTE(transTime);
	appZclBuffer[index++] = HIGH_BYTE(transTime);
  
	appZclBufferLen = index;
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
  }
  //--------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------
  
  void gateway_sm_ZclControl(uint8_t* Eui64,uint8_t ep,uint16_t clusterId,uint8_t type,uint8_t command,uint8_t *data,int len)
  {
	uint8_t index = 0;
	setApsFrame(clusterId);
	appZclBuffer[index++] = (type| ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
	appZclBuffer[index++] = emberAfNextSequence();
	appZclBuffer[index++] = command;
  	memcpy(&appZclBuffer[index],data,len);
	index+=len;
	appZclBufferLen = index;
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
  }

#if 0
  void gateway_sm_ZclGroup(EmberEUI64 Eui64,uint8_t ep,uint8_t cmd,uint16_t groupId)
  {
	const uint8_t * stringPtr;
	uint8_t groupName[MAX_COMMAND_LEN];
	uint8_t nameLength, dataSize, i, count;
	uint8_t index = 0;
  
	MEMSET(groupName, 0, MAX_COMMAND_LEN);
	setApsFrame(ZCL_GROUPS_CLUSTER_ID);
	appZclBuffer[index++] = (ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER); //01
	appZclBuffer[index++] = emberAfNextSequence(); //04
  
	switch ( argString[2] ) {
	  case 'd': //ad'd'
		stringPtr = argString + 4;
		sscanf(stringPtr, "%hx %s", &groupId, groupName);
		nameLength = strlen(groupName);
		appZclBuffer[index++] = ZCL_ADD_GROUP_COMMAND_ID; //00
		appZclBuffer[index++] = LOW_BYTE(groupId);
		appZclBuffer[index++] = HIGH_BYTE(groupId);
		appZclBuffer[index++] = nameLength;
  
		for (i = 0; i < nameLength; i++) {
		  appZclBuffer[index++] = groupName[i];
		}
  
		break;
	  case 'e': //vi'e'w
		stringPtr = argString + 5;
		sscanf(stringPtr, "%hx", &groupId);
		appZclBuffer[index++] = ZCL_VIEW_GROUP_COMMAND_ID; //01
		appZclBuffer[index++] = LOW_BYTE(groupId);
		appZclBuffer[index++] = HIGH_BYTE(groupId);
		break;
	  case 't': //ge't'
		stringPtr = argString + 4;
		sscanf(stringPtr, "%hhx %*s[\0]", &count);
		stringPtr = strtok(stringPtr, " ");
		appZclBuffer[index++] = ZCL_GET_GROUP_MEMBERSHIP_COMMAND_ID; //02
		appZclBuffer[index++] = count;
		for (i = 0; i < count; i++) {
		  stringPtr = strtok(NULL, " ");
		  sscanf(stringPtr, "%hx %*s[\0]", &groupId);
		  appZclBuffer[index++] = LOW_BYTE(groupId);
		  appZclBuffer[index++] = HIGH_BYTE(groupId);
		}
		break;
	  case 'm': //re'm'ove
		stringPtr = argString + 7;
		sscanf(stringPtr, "%hx", &groupId);
		appZclBuffer[index++] = ZCL_REMOVE_GROUP_COMMAND_ID; //03
		appZclBuffer[index++] = LOW_BYTE(groupId);
		appZclBuffer[index++] = HIGH_BYTE(groupId);
		break;
	  case 'a': //rm'a'll
		stringPtr = argString + 6;
		appZclBuffer[index++] = ZCL_REMOVE_ALL_GROUPS_COMMAND_ID; //04
		break;
	  case '-': //ad'-'if-id
		stringPtr = argString + 4;
		sscanf(stringPtr, "%hx %s", &groupId, groupName);
		nameLength = strlen(groupName);
		appZclBuffer[index++] = ZCL_ADD_GROUP_IF_IDENTIFYING_COMMAND_ID; //05
		appZclBuffer[index++] = LOW_BYTE(groupId);
		appZclBuffer[index++] = HIGH_BYTE(groupId);
		appZclBuffer[index++] = nameLength;
		for (i = 0; i < nameLength; i++) {
		  appZclBuffer[index++] = groupName[i];
		}
		break;
	  default:
		break;
	}
	appZclBufferLen = index;
	zclCmdIsBuilt = true;
  }
#endif


