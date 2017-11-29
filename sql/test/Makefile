.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test

.PHONY: redsql test clean

test: run_test_sql_api.c test_sql_api.c
	$(CC) $(CFLAGS) -o $(EXEC) ../../lib/CuTest.o `../../bin/mysql_config --cflags --libs`  test_sql_api.c run_test_sql_api.c ../sql_api.c ../../row/*.c

clean:
	-rm test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
