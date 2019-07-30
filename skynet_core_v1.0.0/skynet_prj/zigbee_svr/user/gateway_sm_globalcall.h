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

#ifndef SILABS_CHANNEL_SM_GLOBALCALL_H
#define SILABS_CHANNEL_SM_GLOBALCALL_H

void gateway_sm_ZclGlobalSendMeAReport(EmberEUI64 Eui64,uint8_t ep,uint16_t clusterId,
														uint16_t attributeId,uint16_t minInterval,
														uint16_t maxInterval,uint8_t type,
														uint8_t*data);

void gateway_sm_ZclGlobalWrite(EmberEUI64 Eui64,uint8_t ep,uint16_t clusterId,
											uint16_t attributeId,uint8_t type,uint8_t *buffer,
                                            uint16_t bufLen);

void gateway_sm_ZclGlobalReadFactory(EmberEUI64 eui64,uint8_t endpoint);
void gateway_sm_ZclGlobalRead(EmberEUI64 eui64,uint8_t endpoint,uint16_t clusterId, uint16_t attributeId);

#endif
