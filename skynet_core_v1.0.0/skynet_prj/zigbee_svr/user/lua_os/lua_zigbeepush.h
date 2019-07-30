#ifndef _LUA_ZIGBEE_PUSH_H
#define _LUA_ZIGBEE_PUSH_H
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

void lua_dev_online(uint8_t*mac,uint8_t ep,int devid,uint8_t* incluster,int inlen,uint8_t*outcluster,int outlen );
void lua_dev_left(uint8_t *mac);
void lua_zcl_report(uint8_t*mac,uint8_t ep,int cluster,uint8_t*data,int len );
void lua_zcl_read(uint8_t*mac,uint8_t ep,int cluster,uint8_t*data,int len );
void lua_network_state(uint8_t*mac,uint8_t power,int pid,uint8_t channel,char *version);
void lua_dev_change(uint8_t *mac,uint8_t status);
void lua_zcl_cmd(uint8_t*mac,uint8_t ep,uint8_t type,int cluster,uint8_t cmd,uint8_t*data,int len );
void lua_rssi_report(uint8_t*mac,uint8_t rssi,uint8_t lqi);
void lua_aps_sent(uint8_t*mac,uint8_t status,int cluster,uint8_t cmd);
void lua_dev_ep(uint8_t*mac,uint8_t cont,uint8_t* list);

#endif
