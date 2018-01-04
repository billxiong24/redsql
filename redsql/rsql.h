#ifndef RSQL_H
#define RSQL_H
#include "../types.h"

typedef struct REDSQL REDSQL;

REDSQL *redsql_init(char *sql_host, char *sql_user, char *sql_pass, char *db, char *redis_host, unsigned int redis_port);

void redsql_load_config(REDSQL *, char *file);

unsigned long redsql_store(REDSQL *redsql, char *key, ...);

RES_ROWS_ITER *redsql_retrieve(REDSQL *redsql, char *key, ...);

void redsql_close(REDSQL *);



#endif
