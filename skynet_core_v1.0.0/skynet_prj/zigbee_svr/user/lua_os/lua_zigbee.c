#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "lua_obj.h"

//--------------------------------------

int lua_coap_send(lua_State* L)
{
	coap_token tk;

	coap_pdu_t* s = (coap_pdu_t*)luaL_checkudata(L, 1, "lua_coap");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	
	const int fd=luaL_checkinteger(L,2);	
	size_t LEN=0;
	const char* token = lua_tolstring(L, 3,&LEN);
	if(LEN)
	{
		memcpy(tk.s,(uint8*)token,LEN);
		tk.length=LEN;
	}
	const int code=luaL_checkinteger(L,4);
	
	struct lua_zigbee_objcb *ref=NULL;	
	if(lua_isfunction(L, 5)) 
	{//不是一个函数
		ref=Db_Malloc(sizeof(*ref));
		if(ref)
		{
			bzero(ref,sizeof(*ref));
			
			lua_pushvalue(L, 5);
			ref->cb_ref=lua_setluadata(L);
			ref->L=L;
			if(!lua_isnil(L,6))
			{
				lua_pushvalue(L, 6);
				ref->ref_arg=lua_setluadata(L);
			}
		}
		printf("------------send CB FUN is not NULL!!!!\n");
		
		def_coap_msg_event_send_con(fd,&tk,code&0xff,s,lua_coap_ack,ref);
	}
	else
	def_coap_msg_event_send_con(fd,&tk,code&0xff,s,NULL,NULL);
	
	return 0;
}

int lua_coap_post(lua_State* L)
{
	coap_token tk;

	coap_pdu_t* s = (coap_pdu_t*)luaL_checkudata(L, 1, "lua_coap");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	
	const int fd=luaL_checkinteger(L,2);
	size_t LEN=0;
	const char* token = lua_tolstring(L, 3,&LEN);
	if(LEN)
	{
		memcpy(tk.s,(uint8*)token,LEN);
		tk.length=LEN;
	}
	const int code=luaL_checkinteger(L,4);
	
	struct lua_zigbee_objcb *ref=NULL;	
	if(lua_isfunction(L, 5)) 
	{//不是一个函数
		ref=Db_Malloc(sizeof(*ref));
		if(ref)
		{
			bzero(ref,sizeof(*ref));
			
			lua_pushvalue(L, 5);
			ref->cb_ref=lua_setluadata(L);
			ref->L=L;
			if(!lua_isnil(L,6))
			{
				lua_pushvalue(L, 6);
				ref->ref_arg=lua_setluadata(L);
			}
		}
		printf("------------post CB FUN is not NULL!!!!\n");		
		def_coap_msg_event_send_non(fd,&tk,code&0xff,s,lua_coap_ack,ref);		
	}	
	else
	def_coap_msg_event_send_non(fd,&tk,code&0xff,s,NULL,NULL);
	
	return 0;
}


int lua_coap_replay(lua_State* L)
{
	coap_pdu_t* s = (coap_pdu_t*)luaL_checkudata(L, 1, "lua_coap");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	
	const int fd=luaL_checkinteger(L,2);
	const int code=luaL_checkinteger(L,3);

	def_coap_msg_event_replay(fd,code&0xff,s);
	return 0;
}

int lua_coap_replay_error(lua_State* L)
{
	coap_pdu_t* s = (coap_pdu_t*)luaL_checkudata(L, 1, "lua_coap");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	
	const int fd=luaL_checkinteger(L,2);
	const int code=luaL_checkinteger(L,3);

	def_coap_msg_event_replay_reset(fd,code&0xff,s);
	return 0;
}
int lua_coap_replay_asynbc(lua_State* L)
{
	coap_pdu_t* s = (coap_pdu_t*)luaL_checkudata(L, 1, "lua_coap");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	
	const int fd=luaL_checkinteger(L,2);
	def_coap_msg_event_replay_asynbc(fd,s);
	return 0;
}


int lua_create_new_zigbee_obj(lua_State* L)
{

struct lua_zigbee_objcb *ref=NULL;
struct lua_zigbee_objcb *no_ref=NULL;
struct lua_zigbee_objcb *zigbee_ref=NULL;

	if(lua_isfunction(L, 1))
	{//coap_recive
		ref=Db_Malloc(sizeof(*ref));
		if(ref)
		{
			lua_pushvalue(L, 1);
			ref->cb_ref=lua_setluadata(L);
			ref->L=L;
			ref->ref_arg=0;
		}
	}
	if(lua_isfunction(L, 2))
	{//dev_notify
		no_ref=Db_Malloc(sizeof(*no_ref));
		if(no_ref)
		{
			lua_pushvalue(L, 2);
			no_ref->cb_ref=lua_setluadata(L);
			no_ref->L=L;
			no_ref->ref_arg=0;
		}
	}
	if(lua_isfunction(L, 3))
	{//zigbee_recive
		zigbee_ref=Db_Malloc(sizeof(*zigbee_ref));
		if(zigbee_ref)
		{
			lua_pushvalue(L, 3);
			zigbee_ref->cb_ref=lua_setluadata(L);
			zigbee_ref->L=L;
			zigbee_ref->ref_arg=0;
		}
	}
	const char *name=luaL_checkstring(L,4);
	const int id=luaL_checkinteger(L,5);
	const int time=luaL_checkinteger(L,6);
	const int fifo_flag=luaL_checkinteger(L,7);

	printf("time=%d\n",time);

	def_zigbee_devobj_open(name,id, time,fifo_flag,ref,no_ref,zigbee_ref);

	printf("zigbee obj is ok \n");
		
    return 0;
}

int lua_zigbee_coap_online(lua_State* L)
{
	int parent=0;
	int child=0;
	int id=0;
	
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);

	def_zigbee_devmanage_get_info(mac, NULL,&id, &child, &parent, NULL);
	int DEV_ID= id | child<<16;

	zigbee_net_dev_online(mac,  parent, DEV_ID);

	return 0;
}

int lua_zigbee_coap_offline(lua_State* L)
{
	size_t len=0;
	unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	
	zigbee_net_dev_offline(mac);

	return 0;
}

int lua_zigbee_coap_report(lua_State* L)
{
	size_t len=0;
	 unsigned char* mac=(unsigned char*)lua_tolstring (L, 1,&len);
	 unsigned char* data=(unsigned char*)lua_tolstring (L, 2,&len);
	const int rssi=luaL_checkinteger(L,3);

	zigbee_net_dev_coap_report_info(mac,len,data,rssi&0xff);

	return 0;
}

int lua_zigbee_obj_uart_send_msg(lua_State* L)
{
	uint8 * dev=(uint8 *)lua_tostring(L, 1);	
	const int type=luaL_checkinteger(L,2);
	void* msg = lua_touserdata(L, 3);
	const size_t sz=luaL_checkinteger(L,4);
	
	if(type==0x00 && dev==NULL)
	return 0;
	
	int handle=def_zigbee_devmanage_get_handle(dev);
	if(lua_isfunction(L, 5))
	{
		lua_zigbee_msg_token * ref=Db_Malloc(sizeof(*ref));
		bzero(ref,sizeof(*ref));
		
		lua_pushvalue(L, 5);
		ref->cb_fun.cb_ref=lua_setluadata(L);
		ref->cb_fun.L=L;
		if(!lua_isnil(L,6))
		{
			lua_pushvalue(L, 6);
			ref->cb_fun.ref_arg=lua_setluadata(L); 
		}
		
		ref->token=queue_msg_event_context_newsession_handle(handle);

		ref->send_cont=lua_tointeger(L, 7);
		ref->timeout=lua_tointeger(L, 8);
		
		if(ref->send_cont==0)
		{
			ref->send_cont=3;
			ref->timeout=4000;
		}		
		memcpy(ref->mac,dev,8);
		
		lua_zigbee_obj_push(ref);
		def_uartsend_dever_send_handle(handle,dev,type,msg,sz, ref->token,ref->send_cont,ref->timeout);	
	}
	else
		def_uartsend_dever_send_handle(handle,dev,type,msg,sz, 0,0,0);
		
	return 0;
}

int lua_zigbee_obj_uart_add_fifo(lua_State* L)
{
	uint8 * dev=(uint8 *)lua_tostring(L, 1);	
	const int type=luaL_checkinteger(L,2);
	void* msg = lua_touserdata(L, 3);
	const size_t sz=luaL_checkinteger(L,4);
	int handle=def_zigbee_devmanage_get_handle(dev);
	
	lua_zigbee_msg_token *ref=NULL;
	if(lua_isfunction(L, 5))
	{
		ref=Db_Malloc(sizeof(*ref));
		bzero(ref,sizeof(*ref));
		
		lua_pushvalue(L, 5);
		ref->cb_fun.cb_ref=lua_setluadata(L);
		ref->cb_fun.L=L;

		if(!lua_isnil(L,6))
		{
			lua_pushvalue(L, 6);
			ref->cb_fun.ref_arg=lua_setluadata(L); 
		}
		
		ref->send_cont=lua_tointeger(L, 7);
		ref->timeout=lua_tointeger(L, 8);
		
		if(ref->send_cont==0)
		{
			ref->send_cont=3;
			ref->timeout=4000;
		}
		memcpy(ref->mac,dev,8);
		
		ref->token=queue_msg_event_context_newsession_handle(handle);
		lua_zigbee_obj_push(ref);
		lua_pushinteger(L, def_zigbee_devmanage_dev_push_fifo(dev,type,msg,sz,ref->token,ref->send_cont,ref->timeout));		
	}
	else
		lua_pushinteger(L, def_zigbee_devmanage_dev_push_fifo(dev,type,msg,sz,0,0,0));	

	return 1;
}


int lua_zigbee_obj_uart_send_fifo(lua_State* L)
{
	uint8 * dev=(uint8 *)lua_tostring(L, 1);	
	def_zigbee_devmanage_dev_send_fifo(dev);
	return 0;
}

int lua_zigbee_obj_uart_get_fifo_num(lua_State* L)
{
	uint8 * dev=(uint8 *)lua_tostring(L, 1);
	
	lua_pushinteger(L, def_zigbee_devmanage_dev_get_fifo_num(dev));	

	return 1;
}

static void lua_obj_time_event(void *arg)
{
	printf("lua_obj_time_event\n");
	if(arg)
	{
		struct lua_zigbee_objcb *f=arg;
		
		lua_getluadata(f->L,f->cb_ref);
		
		if(f->ref_arg)
		{
			lua_getluadata(f->L,f->ref_arg);	
			if(lua_pcall(f->L,1,0,0))	
			{           
				printf("lua_obj_time_event error=%s\n",lua_tostring(f->L, -1));       
			}
			
			lua_unregluadata(f->L, f->ref_arg);
		}
		else
		{
			if(lua_pcall(f->L,0,0,0))	
			{           
				printf("lua_obj_time_event error=%s\n",lua_tostring(f->L, -1));       
			}
		}
		lua_unregluadata(f->L, f->cb_ref);
	
		DbMemFree(f, sizeof(struct lua_zigbee_objcb));
	}
}

static int lua_creat_obj_time_fun(lua_State* L)
{
	struct lua_zigbee_objcb *time_ref=NULL;

	if(lua_isfunction(L, 1))
	{
		time_ref=Db_Malloc(sizeof(struct lua_zigbee_objcb));
		if(time_ref)
		{
			bzero(time_ref,sizeof(*time_ref));
			
			time_ref->L=L;
			lua_pushvalue(L, 1);
			time_ref->cb_ref=lua_setluadata(L);
			if(!lua_isnil(L,2))
			{
				lua_pushvalue(L, 2);
				time_ref->ref_arg=lua_setluadata(L);
			}
			
			log_info("time_ref->cb_ref =%d :: time_ref->ref_arg=%d\n",time_ref->cb_ref,time_ref->ref_arg);
		}
	}
	const int time_ms=luaL_checkinteger(L,3);
	
	def_time_msg_push(time_ms, lua_obj_time_event, time_ref);
	
	return 0;
}

static int lua_creat_obj_get_time_fun(lua_State* L)
{
	uint64_t t=def_time_get();

	lua_pushinteger(L,t>>32 & 0xffffffff);//push 高位
	lua_pushinteger(L,t & 0xffffffff);//push 地位

	return 2;
}

static int lua_zigbee_obj_setid_fun(lua_State* L)
{
	uint8 * dev=(uint8 *)lua_tostring(L, 1);
	const int id=luaL_checkinteger(L,2);
	def_zigbee_devmanage_set_child(dev, id);
	
	zigbee_DevStoreInfo DEV;
	memset(&DEV,0,sizeof(DEV));
	if(!Block_Store_GetZigbeeDev(dev,&DEV) && DEV.child_id!=id)
	{

		DEV.child_id=id;
		
		Block_Store_AddZigbeeDev(dev, &DEV,1);
	}	
	return 0;
}

static int lua_zigbee_obj_del_fun(lua_State* L)
{
	uint8 * mac=(uint8 *)lua_tostring(L, 1);
	zigbee_net_zigbee_dev_leave(def_zigbee_devmanage_get_nwk(mac), mac);		
	return 0;
}

static int lua_zigbee_obj_get_filesize_fun(lua_State* L)
{
	char * file=lua_tostring(L, 1);
	FILE *fp = fopen(file,"rb");
	if(fp==NULL)
		lua_pushinteger(L,0);
	else
	{
		fseek(fp,0L,SEEK_END);
		lua_pushinteger(L,ftell(fp));
		fclose(fp);
	}
 	return 1;
}

static int lua_zigbee_obj_lode_file_fun(lua_State* L)
{
	char * file=lua_tostring(L, 1);
	FILE *fp = fopen(file,"rb");
	if(fp==NULL)
	{
		lua_pushinteger(L,0);
		lua_pushnil(L);	
	}
	else
	{
		fseek(fp,0L,SEEK_END);
		int len=ftell(fp);
		if(len)
		{
			fseek(fp,0L,SEEK_SET);
			
			uint8 *data=Db_Malloc(len);
			bzero(data,len);
			fread(data,len,1,fp);
			
			lua_pushinteger(L,len);
			lua_pushlightuserdata(L,data);
		}
		fclose(fp);
	}
 	return 2;
}

static int lua_zigbee_obj_unlode_file_fun(lua_State* L)
{
	void *data=lua_touserdata(L, 1);
	const int len=luaL_checkinteger(L,2);
	
	if(data && len)
	{
		DbMemFree(data, len);
	}
 	return 0;
}
static int lua_zigbee_obj_get_file_fun(lua_State* L)
{
	void *data=lua_touserdata(L, 1);
	const int len=luaL_checkinteger(L,2);
	const int index=luaL_checkinteger(L,3);
	const int size=luaL_checkinteger(L,4);
	
	if(data && len)
	{
		if(index<len)
		{			
			if(index+size>len)
			{
				lua_pushlstring(L, (char *)data+index, len-index);	
			}
			else
			{
				lua_pushlstring(L, (char *)data+index, size);	
			}
			return 1;
		}
	}

	lua_pushnil(L);
	
 	return 1;
}

static const luaL_Reg call[] = 
{  
	{"network_start",     lua_create_new_zigbee_obj},  
	{"network_open",     lua_zigbee_coap_online},  
	{"network_close",     lua_zigbee_coap_offline},  
	{"network_leave",     lua_zigbee_coap_report}, 
	{"network_del",	 lua_coap_send},
	{"network_status",	 lua_coap_post},
	{"network_setpower", lua_coap_replay},
	{"zcl_control", lua_coap_replay_error},	
	{"zcl_onoff", lua_coap_replay_asynbc},		
	{NULL, NULL}
};
	
LUAMOD_API int luaopen_zigbee_obj (lua_State *L) 
{  
	luaL_newlib(L, call);  
	return 1;
}




