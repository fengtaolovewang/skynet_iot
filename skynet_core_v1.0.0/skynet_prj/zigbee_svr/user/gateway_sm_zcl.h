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

#ifndef SILABS_CHANNEL_SM_ZCL_H
#define SILABS_CHANNEL_SM_ZCL_H
void gateway_sm_ZclMoveToColorTemp(uint8_t* Eui64,uint8_t ep,uint16_t colorTemp,uint16_t transTime);
void gateway_sm_ZclMoveToHueAndSat(uint8_t* Eui64,uint8_t ep,uint8_t hue,uint8_t sat,uint16_t transTime);
void gateway_sm_ZclControl(uint8_t* Eui64,uint8_t ep,uint16_t clusterId,uint8_t type,uint8_t command,uint8_t *data,int len);
void gateway_sm_ZclOnAndMoveToLevel(uint8_t* Eui64,uint8_t ep,uint8_t level,uint16_t transTime);
void gateway_sm_ZclOnOff(uint8_t* Eui64,uint8_t ep,uint8_t command);
void gateway_sm_IasZoneClientClearAll(void);
void gateway_sm_ZclGlobalDirection(uint8_t zclDirection);

#endif
