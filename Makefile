.DEFAULT_GOAL := redsql.so
CC=gcc
CFLAGS= -Wall
EXEC=main
REDSQL_H=redsql/redsql.h 
REDSQL_C=redsql/redsql.c
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
REDIS_API=redis/redis_api.c
SQL_API=sql/sql_api.c

REDIS_DIR=redis/test
SQL_DIR=sql/test
REDSQL_DIR=redsql/test
TEST_EXEC=test

ROW_OBJ=_priv_row.o
LINKS=-levent -lhiredis

COMP=$(CC) $(CFLAGS) -fPIC -c 
OBJ=redsql.o redis_api.o sql_api.o _priv_row.o _priv_row_def.o _priv_row_redis.o _priv_row_sql.o



.PHONY: test clean

redsql.so: $(OBJ)
	$(CC) -shared -Wl,-soname,libredsql.so -o libredsql.so $(OBJ)
	rm $(OBJ)

redsql.o: $(REDSQL_C)
	$(COMP) $<

redis_api.o: $(REDIS_API)
	$(COMP) $<

sql_api.o: $(SQL_API)
	$(COMP) $<

_priv_row.o: row/_priv_row.c 
	$(COMP) $<

_priv_row_def.o: row/_priv_row_def.c
	$(COMP) $<

_priv_row_redis.o: row/_priv_row_redis.c 
	$(COMP) $<

_priv_row_sql.o: row/_priv_row_sql.c 
	$(COMP) $<

redsql: $(REDSQL_C) $(REDSQL_H) sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  main.c $(REDSQL_C) $(REDIS_API) $(SQL_API) row/*.c `$(MYSQL_EXEC)` $(LINKS)

clean:
	-rm -f *.o *.so $(EXEC) dump.rdb
	-scripts/rmswp.sh

leak:
	valgrind --leak-check=full -v ./$(EXEC)

test:
	$(MAKE) -C $(REDIS_DIR)
	$(MAKE) -C $(SQL_DIR)
	$(MAKE) -C $(REDSQL_DIR)

	$(REDIS_DIR)/$(TEST_EXEC)
	$(SQL_DIR)/$(TEST_EXEC)
	$(REDSQL_DIR)/$(TEST_EXEC)

	$(MAKE) clean -C $(REDIS_DIR)
	$(MAKE) clean -C $(SQL_DIR)
	$(MAKE) clean -C $(REDSQL_DIR)

	$(MAKE) clean
