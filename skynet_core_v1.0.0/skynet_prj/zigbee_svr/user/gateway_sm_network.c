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
#include EMBER_AF_API_NETWORK_CREATOR
#include EMBER_AF_API_NETWORK_CREATOR_SECURITY

#include "gateway_sm_main.h"
#include "gateway_sm_network.h"
#include <stdlib.h>
#include <string.h>

#define setTcRejoinsUsingWellKnownKeyAllowed(allow) \
  (void)ezspSetPolicy(EZSP_TC_REJOINS_USING_WELL_KNOWN_KEY_POLICY, (allow))

#define setTcRejoinsUsingWellKnownKeyTimeout(timeout) \
  (void)ezspSetConfigurationValue(EZSP_CONFIG_TC_REJOINS_USING_WELL_KNOWN_KEY_TIMEOUT_S, (timeout))
//--------------------------------------------------------------------------------------------

void gateway_sm_NetworkBoardPjoin(uint8_t time)
{
  emberAfPermitJoin(time, true);
}

void gateway_sm_NetworkLeave(void)
{
  EmberStatus status;
  status = emberLeaveNetwork();
  UNUSED_VAR(status);
}
//-----------------------------------------------------------------------------------------------

void gateway_sm_ZdoLeave(uint8_t* nullEui64, uint8_t removeChildren, uint8_t rejoin)
{
  EmberNodeId target;
  EmberStatus status;
  uint8_t options = 0;

  
  
  target=emberAfDeviceTableGetNodeIdFromEui64(nullEui64);
 if(target)
 {
		if (rejoin) {
		options |= EMBER_ZIGBEE_LEAVE_AND_REJOIN;
		}

  status = emberLeaveRequest(target,
                             nullEui64,
                             options,
                             EMBER_APS_OPTION_RETRY);
 	}
}

//-----------------------------------------------------------------------------------------------

void gateway_sm_OpenNetwork(void)
{
  emberAfPluginNetworkCreatorSecurityOpenNetwork();
}

void gateway_sm_CloseNetwork(void)
{
  emberAfPluginNetworkCreatorSecurityCloseNetwork();
}

//-----------------------------------------------------------------------------------------------
//1.2 nwknetwork
void gateway_sm_NetworkForm(uint8_t radioChannel,uint8_t radioTxPower,uint16_t panId)
{
  EmberStatus status;
  EmberNetworkParameters networkParams;

  MEMSET(&networkParams, 0, sizeof(EmberNetworkParameters));

   networkParams.radioChannel=radioChannel;
   networkParams.radioTxPower=radioTxPower;
   networkParams.panId=panId;

   emberAfGetFormAndJoinExtendedPanIdCallback(networkParams.extendedPanId);

  status = emberAfFormNetwork(&networkParams);
  emberAfAppFlush();
}
//-----------------------------------------------------------------------------------------------
//3.0 nwknetwork

void gateway_sm_StartNetworkWithParameters(uint8_t channel,uint8_t radioTxPower,uint16_t panId)
{

  EmberStatus status;

  status = emberAfPluginNetworkCreatorNetworkForm(true,
                                                    panId,
                                                    radioTxPower,
                                                    channel);
}

//-----------------------------------------------------------------------------------------------
int8_t gateway_sm_power=EMBER_AF_PLUGIN_NETWORK_CREATOR_RADIO_POWER;

int8_t emberAfPluginNetworkCreatorGetRadioPowerCallback(void)
{
	gateway_sm_power=19;
  printf("gateway_sm_power=%d\n",gateway_sm_power);
  return gateway_sm_power;
}

void gateway_sm_StartNetwork(uint8_t radioTxPower)
{
	gateway_sm_power=radioTxPower;
    emberAfPluginNetworkCreatorStart(true);
}

//-----------------------------------------------------------------------------------------------------------

void gateway_sm_SetJoiningKey(uint8_t* eui64,uint8_t *key,int len)
{
  EmberKeyData keyData;
  EmberStatus status;
  MEMCOPY(keyData.contents,key,len);

  status = ezspAddTransientLinkKey(eui64, &keyData);

  emberAfCorePrintln("%p: 0x%X", "Set joining link key", status);
}

void gateway_sm_ClearJoiningKeys(void)
{
  ezspClearTransientLinkKeys();
}

void gateway_sm_OpenWithKey(uint8_t* eui64,uint8_t *key,int len)
{
  EmberKeyData keyData;
  EmberStatus status;
  MEMCOPY(keyData.contents,key,len);

  status = emberAfPluginNetworkCreatorSecurityOpenNetworkWithKeyPair(eui64,
                                                                     keyData);

  emberAfCorePrintln("%p: Open network: 0x%X",
                     EMBER_AF_PLUGIN_NETWORK_CREATOR_SECURITY_PLUGIN_NAME,
                     status);
}

void gateway_sm_AllowRejoin(uint8_t allow)
{
  setTcRejoinsUsingWellKnownKeyAllowed((bool)allow);
}

void gateway_sm_SetRejoinTimeout( uint16_t timeout)
{
  setTcRejoinsUsingWellKnownKeyTimeout(timeout);
}
//-----------------------------------------------------------------------------------------------

void gateway_sm_SetPower(uint8_t power)
{
	emberSetRadioPower(power);
}

