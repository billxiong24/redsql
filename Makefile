.DEFAULT_GOAL := redsql
CC=gcc
CFLAGS=-Wall
EXEC=main
REDSQL_C=redsql/redsql.c

.PHONY: redsql test clean

redsql: $(REDSQL_C) redsql/redsql.h sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  lib/* main.c $(REDSQL_C) redis/redis_api.c sql/sql_api.c row/*.c `./bin/mysql_config --cflags --libs` -levent -lhiredis

clean:
	-rm *.o $(EXEC)

leak:
	valgrind --leak-check=full -v ./$(EXEC)
