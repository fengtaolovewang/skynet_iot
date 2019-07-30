#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "sqlite3db.h"
#include "lua_dbcall.h"

extern lua_State * OBJ_L;
extern sqlite3 * DB;

#if 0
void _incb(sqlite3_stmt*	stmt3,int num)
{
	int i;
	i=0;
	while(ZigbeeTable[TableNum][i]!=NULL)
	{
		GetSql_Zigbee_dat(indata, ZigbeeTable[TableNum][i], data[i]);
		sqlite3_bind_text(stmt3,i+1,data[i],strlen(data[i]),NULL);
		i++;
	}
}
#endif

static void lua_db_insert(sqlite3_stmt* stmt3,int num)
{
	lua_State *L=OBJ_L;
	printf("lua_db_insert is ok!!\n");
	
	lua_getglobal(L, "db_insert");
	lua_pushinteger (L,num);
	
	int i;
	if(lua_pcall(L,1,num,0))
	{           
		printf("lua_db_insert error=%s\n",lua_tostring(L, -1));       
	}
	
	for(i=0;i<num;i++)
	{
		// GetSql_Zigbee_dat(indata, ZigbeeTable[TableNum][i], data[i]);
		
		switch (lua_type(L,num-i))
		{
			case LUA_TSTRING:
			{
				char *d=lua_tostring(L, num-i);
				sqlite3_bind_text(stmt3,i+1,d,strlen(d),NULL);
				break;
			}
			case LUA_TNUMBER:
			{
				int d=lua_tointeger (L, num-i);
				sqlite3_bind_int(stmt3,i+1,d);
				break;
			}
			default:
				sqlite3_bind_null(stmt3,i+1);
			break;
		}
		
	}	

}
static int c_lua_db_insert(lua_State* L)
{
	char* tname=(char*)lua_tostring (L, 1);
	int tnum=luaL_checkinteger(L,2);
	
	Sql_db_insert(DB,tname,tnum, lua_db_insert);

	return 0;
}

// sql :: "delete from %s where %s='%s' and %s='%s'"
static int c_lua_db_del(lua_State* L)
{
	char* sql=(char*)lua_tostring (L, 1);

	Sql_db_del(DB,sql);
	
	return 0;
}

// sql :: "select * from %s"
// sql :: "select * from %s where zongID!='ff'"

static void lua_db_get(sqlite3_stmt*	stmt3,int num)
{
	lua_State *L=OBJ_L;
	
	char *str=NULL;
	int i;
	printf("lua_db_get is ok!!\n");
	
	lua_getglobal(L, "db_get");
		
	for(i=0;i<num;i++)
	{
		switch(sqlite3_column_type(stmt3,i))
		{
			case SQLITE_INTEGER: 
			{			
				int iVal = sqlite3_column_int(stmt3,i);
				lua_pushinteger(L,iVal);
				break;
			}

			case SQLITE_TEXT: 
			{			
				str = (char *)sqlite3_column_text(stmt3,i);
				lua_pushstring(L,str);
				break;
			}

			default:
				lua_pushnil(L);
			break;	
		}

	}
	
	if(lua_pcall(L,num,0,0))
	{           
		printf("lua_db_get error=%s\n",lua_tostring(L, -1));       
	}
}

static int c_lua_db_get(lua_State* L)
{
	char* sql=(char*)lua_tostring (L, 1);
	int num=luaL_checkinteger(L,2);
	
	Sql_db_get(DB,sql,num,lua_db_get);
	return 0;
}

// sql :"UPDATE   %s       SET  %s='%s',%s='%s' WHERE %s='%s' and %s='%s' "
static int c_lua_db_updata(lua_State* L)
{
	char* sql=(char*)lua_tostring (L, 1);


	Sql_db_update(DB,sql);
	
 	return 0;

}

// "create table zigbeedevtable(macaddr,nwkaddr,capabilities,devstatus,online,groups,company);"
static int c_lua_db_creat(lua_State* L)
{
   char* sql=(char*)lua_tostring (L, 1);
	
	Sql_db_creattable(DB,sql);
 	return 0;
}


static const luaL_Reg call[] = 
{  
	{"insert",     c_lua_db_insert},  
	{"del",        c_lua_db_del},  
	{"get",        c_lua_db_get},  
	{"updata",     c_lua_db_updata}, 
	{"creat",	  c_lua_db_creat},	
	{NULL, NULL}
};
	
LUAMOD_API int luaopen_db_obj (lua_State *L) 
{  
	luaL_newlib(L, call);  
	return 1;
}


