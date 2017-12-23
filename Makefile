.DEFAULT_GOAL := redsql
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


.PHONY: test clean

redsql: $(REDSQL_C) $(REDSQL_H) sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  lib/* main.c $(REDSQL_C) $(REDIS_API) $(SQL_API) row/*.c `$(MYSQL_EXEC)` -levent -lhiredis

clean:
	-rm -f *.o $(EXEC) dump.rdb

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
