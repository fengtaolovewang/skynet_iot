#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "lua_obj.h"
#include "gateway_sm_main.h"

#include "gateway_sm_network.h"
#include "gateway_sm_zcl.h"

static int c_lua_zcl_factory(lua_State* L)
{
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	const int ep=luaL_checkinteger(L,2);

	gateway_sm_ZclGlobalReadFactory(mac,ep);
	
	return 0;
}


static int c_lua_zcl_read(lua_State* L)
{
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	const int ep=luaL_checkinteger(L,2);
	const int cluster=luaL_checkinteger(L,3);
	const int attributeId=luaL_checkinteger(L,3);

	gateway_sm_ZclGlobalRead(mac,ep,cluster&0xffff,attributeId&0xffff);
	
	return 0;
}

static int c_lua_zcl_onoff(lua_State* L)
{
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	const int ep=luaL_checkinteger(L,2);
	const int onoff=luaL_checkinteger(L,3);

	gateway_sm_ZclOnOff(mac,ep,onoff);
	
	return 0;
}

static int c_lua_zcl_control(lua_State* L)
{
	size_t len=0;
	size_t datalen=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	const int ep=luaL_checkinteger(L,2);
	const int cluster=luaL_checkinteger(L,3);
	const int type=luaL_checkinteger(L,4);
	const int cmd=luaL_checkinteger(L,5);
	unsigned char* data=(unsigned char*)lua_tolstring (L, 6,&datalen);

	gateway_sm_ZclControl(mac,ep,cluster,type,cmd,data,datalen);
	
 	return 0;

}

static int c_lua_network_setpower(lua_State* L)
{
	const int power=luaL_checkinteger(L,1);
	
	gateway_sm_SetPower(power);
	
 	return 0;

}

static int c_lua_network_status(lua_State* L)
{
	gateway_sm_publishSettings();
	return 0;
}

static int c_lua_network_del(lua_State* L)
{
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	if(len==8)
	{
		gateway_sm_ZdoLeave(mac, 0, 0);
	}
	
 	return 0;
}

static int c_lua_network_leave(lua_State* L)
{
	gateway_sm_NetworkLeave();
 	return 0;
}

static int c_lua_network_close(lua_State* L)
{
	gateway_sm_CloseNetwork();
 	return 0;
}


static int c_lua_network_open(lua_State* L)
{
	gateway_sm_OpenNetwork();
 	return 0;
}

static int c_lua_network_start(lua_State* L)
{
	const int power=luaL_checkinteger(L,1);
	
	gateway_sm_StartNetwork(power);
	
 	return 0;
}

static const luaL_Reg call[] = 
{  
	{"network_start",     c_lua_network_start},  
	{"network_open",      c_lua_network_open},  
	{"network_close",     c_lua_network_close},  
	{"network_leave",     c_lua_network_leave}, 
	{"network_del",	 	  c_lua_network_del},
	{"network_status",	  c_lua_network_status},
	{"network_setpower",  c_lua_network_setpower},
	{"zcl_control",       c_lua_zcl_control},	
	{"zcl_onoff",         c_lua_zcl_onoff},
	{"zcl_read",          c_lua_zcl_read},		
	{"zcl_factory",       c_lua_zcl_factory}, 	
	{NULL, NULL}
};
	
LUAMOD_API int luaopen_zigbee_obj (lua_State *L) 
{  
	luaL_newlib(L, call);  
	return 1;
}


