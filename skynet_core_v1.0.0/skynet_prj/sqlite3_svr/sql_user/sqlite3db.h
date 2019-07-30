#ifndef __SQLITE3DB_H
#define __SQLITE3DB_H

#include "sqlite3.h"

// sql :: "delete from %s where %s='%s' and %s='%s'"

void Sql_db_del(sqlite3 *db,char *sql);
// sql :: "select * from %s"
// sql :: "select * from %s where zongID!='ff'"

sqlite3 * sql_db_open(char *path);

// -------------tablename       k   v   k    v--------------where
// sql :"UPDATE   %s       SET  %s='%s',%s='%s' WHERE %s='%s' and %s='%s' "

int Sql_db_update(sqlite3 *db,char *sql);
#if 0
void _cb(sqlite3_stmt*	stmt3)
{
		//scence id
		str = (char *)sqlite3_column_text(stmt3,0);
		strcpy(pScenceID, str);
		//ENABLE id
		str = (char *)sqlite3_column_text(stmt3,1);
		if(0!=strcmp(str,"01"))
		continue;
		//pstr
		str = (char *)sqlite3_column_text(stmt3,2);
		strcpy(pstr, str);
		printf("file=%s,line=%d,index:%d\npScenceID:%s\npstr:%s\n",__FILE__,__LINE__,cnt,pScenceID,pstr);
		ScenceCondTion(pScenceID,pstr,cnt);
		cnt++;
}
#endif

// sql :: "select * from %s"
// sql :: "select * from %s where zongID!='ff'"
void Sql_db_get(sqlite3 *db,char *sql,int num,void (*cb)(sqlite3_stmt*	stmt3,int num));

void Sql_db_close(sqlite3 *db);

// "create table zigbeedevtable(macaddr,nwkaddr,capabilities,devstatus,online,groups,company);"
void Sql_db_creattable(sqlite3 *db,char *sql);
// -------------tablename           number
// sql :"insert  into  %s    values(?,?,?,?)"
#if 0
void _incb(sqlite3_stmt*	stmt3)
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

int Sql_db_insert(sqlite3 *db,char *tname,unsigned tnum ,void (*cb)(sqlite3_stmt*	stmt3,int num));

#endif
