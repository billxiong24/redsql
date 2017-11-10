.DEFAULT_GOAL := redsql
CC=gcc
CFLAGS=-Wall

redsql: redsql.c redsql.h sql/sql_api.c sql/sql_api.h
	$(CC) $(CFLAGS) -o redsql lib/libmysqlclient.so redsql.c sql/sql_api.c `./bin/mysql_config --cflags --libs`

clean:
	-rm *.o redsql
