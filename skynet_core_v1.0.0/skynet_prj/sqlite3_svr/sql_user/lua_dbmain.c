#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "sqlite3db.h"
#include "lua_dbcall.h"

lua_State * OBJ_L=NULL;
sqlite3 * DB=NULL;
	
#define DB_PTAH "./s.db"
#define DB_LUA_PTAH "db/dbclient.lua"

static void lua_vm_run(void) 
{
	lua_State *L =OBJ_L;

	lua_getglobal(L, "db_core");
	
	if(lua_pcall(L,0,0,0))
	{           
		printf("lua_vm_run error=%s\n",lua_tostring(L, -1));       
	}	
}

static void lua_db_close(void)
{  
	lua_close(OBJ_L);
	Sql_db_close(DB);
}

static int lua_db_init(char *path)
{
	lua_State *L = luaL_newstate();  
	if(L==NULL) return -1;
	
	luaL_openlibs(L);
    luaL_requiref(L, "db", luaopen_db_obj, 1);	
	
	if (luaL_dofile(L,path))	
	{           
		printf("hello.lua=%s\n",lua_tostring(L, -1));    
		return -1;
	}

	OBJ_L=L;	
	return 0;	//lua_obj_close();
}

static void _lua_vm_init(void)
{
	lua_db_init(DB_LUA_PTAH);
}

void main(void)
{
	DB=sql_db_open(DB_PTAH);
	if(DB==NULL)
	return ;
	
	_lua_vm_init();
	while(1)
	{
		usleep(100*1000);
		lua_vm_run();
	}
	lua_db_close();
}

