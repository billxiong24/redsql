.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
TEST_FUNC=../../test_functions/test_func.c


.PHONY: test clean

test: run_test_redsql.c test_redsql.c $(TEST_FUNC)
	$(CC) $(CFLAGS) -o $(EXEC) test_redsql.c run_test_redsql.c ../../sql/sql_api.c ../../redis/redis_api.c ../../row/*.c ../../lib/CuTest.o ../../redsql/redsql.c $(TEST_FUNC) `$(MYSQL_EXEC)` -levent -lhiredis

clean:
	-rm -f test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
