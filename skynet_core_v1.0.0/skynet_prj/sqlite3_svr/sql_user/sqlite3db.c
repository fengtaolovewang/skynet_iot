#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sqlite3.h"
#include "sqlite3db.h"
// -------------tablename           number
// sql :"insert  into  %s    values(?,?,?,?)"
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

int Sql_db_insert(sqlite3 *db,char *tname,unsigned tnum ,void (*cb)(sqlite3_stmt*	stmt3,int num))
{
	char		sql[2048]={0};
	sqlite3_stmt*	stmt3 = NULL;
	int stutas=-1;
	int len=0;
	int datlen=0;
    int i;
	
	len=sprintf(sql+datlen,"insert into %s values(",tname);
	datlen+=len;
	for(i=0;i<tnum-1;i++)
	{
		len=sprintf(sql+datlen,"?,");
		datlen+=len;
	}
	sprintf(sql+datlen,"?)");
	
	//	datlen+=len;
	printf("file=%s,line=%d,sql=%s\n",__FILE__,__LINE__,sql);
	
	if (sqlite3_prepare_v2(db,sql,strlen(sql),&stmt3,NULL) != SQLITE_OK)
	{
		fprintf(stderr, "Can't prepare insert database: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		if(cb)
		{
			cb(stmt3,tnum);
			
			if ( sqlite3_step(stmt3) != SQLITE_DONE)
			{
				fprintf(stderr, "Can't sqlite3_step database: %s\n", sqlite3_errmsg(db));
				stutas=-1;
			}
			else
			{
				printf("file=%s,line=%d,insert OK\n",__FILE__,__LINE__);
				stutas=0;
			}	
		}
	}

	if(stmt3!=NULL)
	{
		sqlite3_finalize(stmt3);
	}

	return stutas;
}


// -------------tablename       k   v   k    v--------------where
// sql :"UPDATE   %s       SET  %s='%s',%s='%s' WHERE %s='%s' and %s='%s' "

int Sql_db_update(sqlite3 *db,char *sql)
{
	sqlite3_stmt*	stmt3 = NULL;
	int status=-1;

	if (sqlite3_prepare_v2(db,sql,strlen(sql),&stmt3,NULL) != SQLITE_OK)
	{
		fprintf(stderr, "Can't prepare database: %s\n", sqlite3_errmsg(db));
		status=-1;
	}
	else if ( sqlite3_step(stmt3) != SQLITE_DONE)
	{
		fprintf(stderr, "UpdateSql failed: %s\n", sqlite3_errmsg(db));
		status=-1;
	}
	else
	{
		printf("file=%s,line=%d,UpdateSql success\n",__FILE__,__LINE__);
		status=0;
	}

	if(stmt3!=NULL)
	{
		sqlite3_finalize(stmt3);
	}
	return status;
}


// sql :: "delete from %s where %s='%s' and %s='%s'"
void Sql_db_del(sqlite3 *db,char *sql)
{
	char*zErrMsg = NULL;

	printf("Sql_db_del sql: %s\n", sql);

	if (sqlite3_exec(db,sql,NULL,NULL,&zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	if(zErrMsg!=NULL)
	{
		 sqlite3_free(zErrMsg);
	}
}

// sql :: "select * from %s"
// sql :: "select * from %s where zongID!='ff'"

void Sql_db_get(sqlite3 *db,char *sql,int num,void (*cb)(sqlite3_stmt*	stmt3,int num))
{
	sqlite3_stmt*	stmt3 = NULL;
		
	if (sqlite3_prepare_v2(db,sql,strlen(sql),&stmt3,NULL) != SQLITE_OK)
	{
		fprintf(stderr, "Can't prepare database: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		while(sqlite3_step(stmt3) == SQLITE_ROW)
		{
			if(cb)
			{
				cb(stmt3,num);
			}
		}
	}
    if(stmt3!=NULL)
    {
        sqlite3_finalize(stmt3);
    }
}

// /bin/zigbeetbl.db
sqlite3 * sql_db_open(char *path)
{
		sqlite3 *db=NULL;
		int rc;
		
		sqlite3_config(SQLITE_CONFIG_SINGLETHREAD); // SQLITE_OPEN_NOMUTEX

		sqlite3_initialize();

		rc = sqlite3_open(path, &db);//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	    if( rc )
		{
			 fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			 return NULL;
		}
		return db;
}

void Sql_db_close(sqlite3 *db)
{
	sqlite3_close(db);
}

// "create table zigbeedevtable(macaddr,nwkaddr,capabilities,devstatus,online,groups,company);"
void Sql_db_creattable(sqlite3 *db,char *sql)
{
	int rc;
	
		rc = sqlite3_exec(db,sql,NULL,NULL,NULL);
		if(rc != SQLITE_OK)
		{
			  fprintf(stderr, "Can't creat %s : %s\n", sql,sqlite3_errmsg(db));
		}
}

