.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
TEST_FUNC=../../test_functions/test_func.c

.PHONY: test clean

test: run_test_redis_api.c test_redis_api.c $(TEST_FUNC)
	$(CC) $(CFLAGS) -o $(EXEC) ../../lib/CuTest.o test_redis_api.c run_test_redis_api.c ../../sql/sql_api.c ../redis_api.c ../../row/*.c $(TEST_FUNC) -lhiredis `$(MYSQL_EXEC)`

clean:
	-rm -f test

leak:
	valgrind --leak-check=full -v ./$(EXEC)

