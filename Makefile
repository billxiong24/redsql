.DEFAULT_GOAL := redsql
CC=gcc
CFLAGS=-Wall
EXEC=redsql

redsql: redsql.c redsql.h sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  lib/* main.c redsql.c redis/redis_api.c sql/sql_api.c row/_priv_row.c `./bin/mysql_config --cflags --libs` -levent -lhiredis

clean:
	-rm *.o $(EXEC) 

leak:
	valgrind --leak-check=full -v ./$(EXEC)
