.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test


.PHONY: test clean

test: run_test_redsql.c test_redsql.c
	$(CC) $(CFLAGS) -o $(EXEC) `../../bin/mysql_config --cflags --libs`  test_redsql.c run_test_redsql.c ../../sql/sql_api.c ../../redis/redis_api.c ../../row/*.c ../../lib/CuTest.o ../../redsql/redsql.c -levent -lhiredis

clean:
	-rm test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
