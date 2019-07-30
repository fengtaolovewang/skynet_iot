This directory contains a snapshot of the SQLite trunk source code
as of 2015-11-30 19:15 UTC, concatenated into an "amalgamation".

See https://www.sqlite.org/draft/howtocompile.html for instructions
on how to compile these sources.

See https://www.sqlite.org/amalgamation.html for additional
information about the amalgamated source code format.

编译时选择线程模式
    可以通过定义SQLITE_THREADSAFE宏来指定线程模式。如果没有指定，默认为串行模式。定义宏SQLITE_THREADSAFE=1指定使用串行模式;=0使用单线程模式；＝2使用多线程模式。

    sqlite3_threadsafe()函数的返回值可以确定编译时指定的线程模式。如果指定了单线程模式，函数返回false。如果指定了串行或者多线程模式，函数返回true。
	由于sqlite3_threadsafe()函数要早于多线程模式以及启动时和运行时的模式选择，所以它既不能区别多线程模式和串行模式也不能区别启动时和运行时的模式。

    最后一句可通过sqlite3_threadsafe函数的实现来理解SQLITE_API int sqlite3_threadsafe(void){ return SQLITE_THREADSAFE; }如果编译时指定了单线程模式，
	那么临界互斥逻辑在构造时就被省略，因此也就无法在启动时或运行时指定串行模式或多线程模式。
	

启动时选择线程模式

    假如在编译时没有指定单线程模式，就可以在应用程序初始化时使用sqlite3_config()函数修改线程模式。参数SQLITE_CONFIG_SINGLETHREAD可指定为
单线程模式，SQLITE_CONFIG_MULTITHREAD指定为多线程模式，SQLITE_CONFIG_SERIALIZED指定为串行模式

运行时选择线程模式

    如果没有在编译时和启动时指定为单线程模式，那么每个数据库连接在创建时可单独的被指定为多线程模式或者串行模式，
	但是不能指定为单线程模式。如果在编译时或启动时指定为单线程模式，就无法在创建连接时指定多线程或者串行模式。

    创建连接时用sqlite3_open_v2()函数的第三个参数来指定线程模式。
	SQLITE_OPEN_NOMUTEX标识创建多线程模式的连接；
	SQLITE_OPEN_FULLMUTEX标识创建串行模式的连接。
	如果没有指定标识，或者使用sqlite3_open()或sqlite3_open16()函数来创建数据库连接，
	那么在编译时或启动时指定的线程模式将作为默认的线程模式使用。

/*
** Open a new database handle.
*/
SQLITE_API int SQLITE_STDCALL sqlite3_open(const char *zFilename, sqlite3 **ppDb)
{
  return openDatabase(zFilename, ppDb,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
}

关键的参数：flags
SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
SQLITE_OPEN_FULLMUTEX：设置数据库连接运行在串行模式。
SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
