.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs


.PHONY: test clean

test: run_test_redsql.c test_redsql.c
	$(CC) $(CFLAGS) -o $(EXEC) test_redsql.c run_test_redsql.c ../../sql/sql_api.c ../../redis/redis_api.c ../../row/*.c ../../lib/CuTest.o ../../redsql/redsql.c `$(MYSQL_EXEC)` -levent -lhiredis

clean:
	-rm test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
