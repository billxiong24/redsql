.DEFAULT_GOAL := redsql
CC=gcc
CFLAGS= -Wall
EXEC=main
REDSQL_H=redsql/redsql.h
REDSQL_C=redsql/redsql.c
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
REDIS_API=redis/redis_api.c
SQL_API=sql/sql_api.c
.PHONY: redsql test clean

redsql: $(REDSQL_C) $(REDSQL_H) sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  lib/* main.c $(REDSQL_C) $(REDIS_API) $(SQL_API) row/*.c `$(MYSQL_EXEC)` -levent -lhiredis

clean:
	-rm *.o $(EXEC)

leak:
	valgrind --leak-check=full -v ./$(EXEC)
