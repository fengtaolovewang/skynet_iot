#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "lua_obj.h"
#include "lua_zigbeepush.h"

extern lua_State * OBJ_L;

//--------------------------------------

void lua_network_state(uint8_t*mac,uint8_t power,int pid,uint8_t channel,char *version)
{	
	lua_State *L=OBJ_L;
	printf("lua_network_state is ok!!\n");
	lua_getglobal(L, "zigbee_networkstate");
	lua_pushlstring(L, (char*)mac, 8);	
	lua_pushinteger (L,power);
	lua_pushinteger(L, pid);
	lua_pushinteger(L, channel);
	lua_pushstring(L,version);  //version	
	
	if(lua_pcall(L,5,0,0))
	{           
		printf("lua_network_state error=%s\n",lua_tostring(L, -1));       
	}
}

void lua_zcl_report(uint8_t*mac,uint8_t ep,int cluster,uint8_t*data,int len )
{
	lua_State *L=OBJ_L;
	printf("lua_zcl_report is ok!!\n");
	
	lua_getglobal(L, "zigbee_zclreport");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,ep);
	lua_pushinteger (L,cluster);		
	lua_pushlstring(L, (char *)data,len);
				
	if(lua_pcall(L,4,0,0))	
	{           
		printf("lua_zcl_report error=%s\n",lua_tostring(L, -1));       
	}
}

void lua_rssi_report(uint8_t*mac,uint8_t rssi,uint8_t lqi)
{
	lua_State *L=OBJ_L;
	printf("lua_rssi_report is ok!!\n");
	
	lua_getglobal(L, "zigbee_rssireport");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,rssi);
	lua_pushinteger (L,lqi);		
				
	if(lua_pcall(L,3,0,0))	
	{           
		printf("lua_rssi_report error=%s\n",lua_tostring(L, -1));       
	}
}

void lua_aps_sent(uint8_t*mac,uint8_t status,int cluster,uint8_t cmd)
{
	lua_State *L=OBJ_L;
	printf("lua_aps_sent is ok!!\n");
	
	lua_getglobal(L, "zigbee_apssent");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,status);
	lua_pushinteger (L,cluster);		
	lua_pushinteger (L,cmd);		
				
	if(lua_pcall(L,4,0,0))	
	{           
		printf("lua_aps_sent error=%s\n",lua_tostring(L, -1));       
	}
}


void lua_zcl_cmd(uint8_t*mac,uint8_t ep,uint8_t type,int cluster,uint8_t cmd,uint8_t*data,int len )
{
	lua_State *L=OBJ_L;
	printf("lua_zcl_cmd is ok!!\n");
	
	lua_getglobal(L, "zigbee_zclcmd");
	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,ep);
	lua_pushinteger (L,type);
	
	lua_pushinteger (L,cluster);		
	lua_pushinteger (L,cmd);		
	lua_pushlstring(L, (char *)data,len);
				
	if(lua_pcall(L,6,0,0))	
	{           
		printf("lua_zcl_report error=%s\n",lua_tostring(L, -1));       
	}
}

void lua_zcl_read(uint8_t*mac,uint8_t ep,int cluster,uint8_t*data,int len )
{
	lua_State *L=OBJ_L;
	printf("lua_zcl_read is ok!!\n");
	
	lua_getglobal(L, "zigbee_zclread");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,ep);
	lua_pushinteger (L,cluster);		
	lua_pushlstring(L, (char *)data,len);
				
	if(lua_pcall(L,4,0,0))	
	{           
		printf("lua_zcl_report error=%s\n",lua_tostring(L, -1));       
	}
	
}
void lua_dev_left(uint8_t *mac)
{
	lua_State *L=OBJ_L;
	printf("zigbee_devleft is ok!!\n");
	
	lua_getglobal(L, "zigbee_devleft");
	lua_pushlstring(L, (char *)mac, 8);
		
	if(lua_pcall(L,1,0,0))
	{           
		printf("lua_dev_left error=%s\n",lua_tostring(L, -1));       
	}	
}

void lua_dev_online(uint8_t*mac,uint8_t ep,int devid,uint8_t* incluster,int inlen,uint8_t*outcluster,int outlen )
{	
	lua_State *L=OBJ_L;
	printf("lua_dev_online is ok!! \n");

	lua_getglobal(L, "zigbee_devonline");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,ep);
	lua_pushinteger (L,devid);		
	lua_pushlstring(L, (char *)incluster, inlen);
	lua_pushlstring(L, (char *)outcluster, outlen);
		
	if(lua_pcall(L,5,0,0))
	{           
		printf("lua_dev_online error=%s\n",lua_tostring(L, -1));       
	}		
}


void lua_dev_ep(uint8_t*mac,uint8_t cont,uint8_t* list)
{	
	lua_State *L=OBJ_L;
	printf("lua_dev_ep is ok!! \n");

	lua_getglobal(L, "zigbee_eplist");

	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,cont);	
	lua_pushlstring(L, (char *)list, cont);

	if(lua_pcall(L,3,0,0))
	{           
		printf("lua_dev_online error=%s\n",lua_tostring(L, -1));       
	}		
}


void lua_dev_change(uint8_t *mac,uint8_t status)
{
	lua_State *L=OBJ_L;
	printf("lua_dev_change is ok!!\n");
	
	lua_getglobal(L, "zigbee_devchange");
	lua_pushlstring(L, (char *)mac, 8);
	lua_pushinteger (L,status);
		
	if(lua_pcall(L,1,0,0))
	{           
		printf("lua_dev_left error=%s\n",lua_tostring(L, -1));       
	}

}


