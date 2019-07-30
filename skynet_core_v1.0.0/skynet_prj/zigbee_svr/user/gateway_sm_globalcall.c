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

#include "gateway_sm_gloab.h"
#include "gateway_sm_main.h"

#include <sys/time.h>
#include <stdlib.h>

#define ZCL_BUFFER_SIZE 255

static uint8_t zclFrameControlDirection = ZCL_FRAME_CONTROL_CLIENT_TO_SERVER;
uint8_t appZclBuffer[ZCL_BUFFER_SIZE];
uint16_t appZclBufferLen;

//----------------------------------------------------------------------------------------------------

void gateway_sm_ZclGlobalRead(EmberEUI64 Eui64,uint8_t ep,uint16_t clusterId, uint16_t attributeId)
{
  uint8_t index = 0;

  setApsFrame(clusterId);
  appZclBuffer[index++] = (ZCL_GLOBAL_COMMAND| zclFrameControlDirection | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
  appZclBuffer[index++] = emberAfNextSequence();
  appZclBuffer[index++] = ZCL_READ_ATTRIBUTES_COMMAND_ID;
  appZclBuffer[index++] = LOW_BYTE(attributeId);
  appZclBuffer[index++] = HIGH_BYTE(attributeId);

  appZclBufferLen = index;
  
  gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);  
}

void gateway_sm_ZclGlobalReadFactory(EmberEUI64 Eui64,uint8_t ep)
{
  uint8_t index = 0;

  setApsFrame(0x0000);
  appZclBuffer[index++] = (ZCL_GLOBAL_COMMAND| zclFrameControlDirection | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
  appZclBuffer[index++] = emberAfNextSequence();
  appZclBuffer[index++] = ZCL_READ_ATTRIBUTES_COMMAND_ID;
  appZclBuffer[index++] = LOW_BYTE(0x0001);//app version
  appZclBuffer[index++] = HIGH_BYTE(0x0001);
  appZclBuffer[index++] = LOW_BYTE(0x0002);
  appZclBuffer[index++] = HIGH_BYTE(0x0002);//stack version
  appZclBuffer[index++] = LOW_BYTE(0x0004);
  appZclBuffer[index++] = HIGH_BYTE(0x0004);//manufacturer 
  appZclBuffer[index++] = LOW_BYTE(0x0005);
  appZclBuffer[index++] = HIGH_BYTE(0x0005);//model id
  appZclBuffer[index++] = LOW_BYTE(0x0007);
  appZclBuffer[index++] = HIGH_BYTE(0x0007);//power source

  appZclBufferLen = index;
  gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);  

}

//--------------------------------------------------------------------------------------------

bool emberAfWriteAttributesResponseCallback(EmberAfClusterId clusterId,
                                            uint8_t *buffer,
                                            uint16_t bufLen)
{
  return false;
}

void gateway_sm_ZclGlobalWrite(EmberEUI64 Eui64,uint8_t ep,uint16_t clusterId,
											uint16_t attributeId,uint8_t type,uint8_t *buffer,
                                            uint16_t bufLen)
{
  uint8_t index = 0;

  setApsFrame(clusterId);
  appZclBuffer[index++] = (ZCL_GLOBAL_COMMAND
                           | zclFrameControlDirection
                           | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
  appZclBuffer[index++] = emberAfNextSequence();
  appZclBuffer[index++] = ZCL_WRITE_ATTRIBUTES_COMMAND_ID;
  appZclBuffer[index++] = LOW_BYTE(attributeId);
  appZclBuffer[index++] = HIGH_BYTE(attributeId);
  appZclBuffer[index++] = type;
  memcpy(&appZclBuffer[index],buffer,bufLen);
  index+=bufLen;
  
  appZclBufferLen = index;

  
  gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);  
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

void gateway_sm_ZclGlobalSendMeAReport(EmberEUI64 Eui64,uint8_t ep,uint16_t clusterId,
														uint16_t attributeId,uint16_t minInterval,
														uint16_t maxInterval,uint8_t type,
														uint8_t*data)
{

  uint8_t  i;
 
  uint8_t index = 0, dataSize = 0;



  setApsFrame(clusterId);
  appZclBuffer[index++] = (ZCL_GLOBAL_COMMAND
						   | zclFrameControlDirection
						   | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER);
  appZclBuffer[index++] = emberAfNextSequence();
  appZclBuffer[index++] = ZCL_CONFIGURE_REPORTING_COMMAND_ID;
  appZclBuffer[index++] = EMBER_ZCL_REPORTING_DIRECTION_REPORTED;
  appZclBuffer[index++] = LOW_BYTE(attributeId);
  appZclBuffer[index++] = HIGH_BYTE(attributeId);
  appZclBuffer[index++] = type;
  appZclBuffer[index++] = LOW_BYTE(minInterval);
  appZclBuffer[index++] = HIGH_BYTE(minInterval);
  appZclBuffer[index++] = LOW_BYTE(maxInterval);
  appZclBuffer[index++] = HIGH_BYTE(maxInterval);

  dataSize = emberAfGetDataSize(type);

	for(i=0;i<dataSize;i++)
	{
		appZclBuffer[index+i]=data[i];
	}
    appZclBufferLen = index + dataSize;
	
	gateway_sm_ZclSend(Eui64,ep,appZclBuffer,appZclBufferLen);	
}


