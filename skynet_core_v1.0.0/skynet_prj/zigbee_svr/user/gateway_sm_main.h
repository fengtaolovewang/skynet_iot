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

#ifndef SILABS_CHANNEL_SM_MAIN_H
#define SILABS_CHANNEL_SM_MAIN_H

#define HOSTVER_STRING_LENGTH 14 // 13 characters + NULL (99.99.99-9999)

struct gateway_stata
{
	char ncpStackVerString[HOSTVER_STRING_LENGTH];
	uint8_t power;
	uint8_t radioChannel;
	int panId;
	uint8_t gatewayEui64[8];
};



void setApsFrame(uint16_t clusterId);
uint8_t gateway_sm_ZclSend(uint8_t* eui64,uint8_t endpoint,uint8_t *appZclBuffer,uint16_t appZclBufferLen);


void gateway_sm_publishSettings(void);

#endif
