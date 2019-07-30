/***************************************************************************//**
 * @file
 * @brief Definitions for the Gateway
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

#ifndef SILABS_CHANNEL_SM_GLOAB_H
#define SILABS_CHANNEL_SM_GLOAB_H

// String/other helpers
static __inline void eui64ToString(EmberEUI64 eui, char* euiString)
{
  sprintf(euiString, "0x%02X%02X%02X%02X%02X%02X%02X%02X",
          eui[7],
          eui[6],
          eui[5],
          eui[4],
          eui[3],
          eui[2],
          eui[1],
          eui[0]);
}

static __inline void nodeIdToString(EmberNodeId nodeId, char* nodeIdString)
{
  sprintf(nodeIdString, "0x%04X", nodeId);
}

static __inline void printAttributeBuffer(uint16_t clusterId, uint8_t* buffer,uint16_t bufLen)
{
  uint16_t bufferIndex;

  emberAfAppPrintln(" Cluster, Attribute: %04X, %02X%02X"
                    " Success Code: %02X"
                    " Data Type: %02X\n"
                    " Hex Buffer: ",
                    clusterId,
                    buffer[0],
                    buffer[1],
                    buffer[2],
                    buffer[3]);

  // Print buffer data as a hex string, starting at the data start byte
  for (bufferIndex = 4;
       bufferIndex < bufLen;
       bufferIndex++) {
    emberAfAppPrint("%02X", buffer[bufferIndex]);
  }
  emberAfAppPrintln("");
}


#endif
