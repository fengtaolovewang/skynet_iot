#ifndef SILABS_CHANNEL_SM_NETWORK_H
#define SILABS_CHANNEL_SM_NETWORK_H


void gateway_sm_SetRejoinTimeout( uint16_t timeout);
void gateway_sm_AllowRejoin(uint8_t allow);

void gateway_sm_OpenWithKey(uint8_t* eui64,uint8_t *key,int len);
void gateway_sm_ClearJoiningKeys(void);

void gateway_sm_SetJoiningKey(uint8_t* eui64,uint8_t *key,int len);

void gateway_sm_StartNetwork(uint8_t radioTxPower);
// 3.0
void gateway_sm_StartNetworkWithParameters(uint8_t channel,uint8_t radioTxPower,uint16_t panId);
void gateway_sm_CloseNetwork(void);
void gateway_sm_OpenNetwork(void);
void gateway_sm_ZdoLeave(uint8_t* nullEui64, uint8_t removeChildren, uint8_t rejoin);

//1.2
void gateway_sm_NetworkForm(uint8_t radioChannel,uint8_t radioTxPower,uint16_t panId);

void gateway_sm_NetworkLeave(void);
//ÔÊÐí×éÍø 1.2
void gateway_sm_NetworkBoardPjoin(uint8_t time);

#endif



