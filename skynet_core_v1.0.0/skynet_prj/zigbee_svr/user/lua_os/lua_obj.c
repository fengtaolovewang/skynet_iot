#include "af.h"
#include "app/framework/util/af-main.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "lua_obj.h"
#include "lua_zigbeecall.h"
lua_State * OBJ_L=NULL;

EmberEventControl emberAfPluginLuaEventControl;
void emberAfPluginLuaEventHandler(void);

static void lua_vm_run(void) 
{
	lua_State *L =OBJ_L;

//	printf("lua_dev_online is ok\n!!");
	
	lua_getglobal(L, "zigbee_core");
	
	if(lua_pcall(L,0,0,0))
	{           
		printf("lua_vm_run error=%s\n",lua_tostring(L, -1));       
	}	
}

void emberAfPluginLuaEventHandler(void)
{
	 emberEventControlSetDelayMS(emberAfPluginLuaEventControl, 100);
	lua_vm_run();
}

static void lua_obj_close(void)
{  
	lua_close(OBJ_L);
}

static int lua_obj_init(char *path)
{
	lua_State *L = luaL_newstate();  
	if(L==NULL) return -1;
	
	luaL_openlibs(L);
    luaL_requiref(L, "zbcall", luaopen_zigbee_obj, 1);	
	
	if (luaL_dofile(L,path))	
	{           
		printf("hello.lua=%s\n",lua_tostring(L, -1));    
		return -1;
	}

	OBJ_L=L;	
	return 0;	//lua_obj_close();
}


void lua_vm_init(void)
{
	lua_obj_init("user/lua_os/zigbee/zigbeeclient.lua");
	
	emberEventControlSetDelayMS(emberAfPluginLuaEventControl, 100);
//	pthread_t pid ;	
//	pthread_create(&pid, NULL, lua_vm_run, NULL);
}


