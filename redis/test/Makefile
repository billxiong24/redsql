.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test

.PHONY: test clean

test: run_test_redis_api.c test_redis_api.c
	$(CC) $(CFLAGS) -o $(EXEC) ../../lib/CuTest.o test_redis_api.c run_test_redis_api.c ../../sql/sql_api.c ../redis_api.c ../../row/*.c -lhiredis `../../bin/mysql_config --cflags --libs`  

clean:
	-rm test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
