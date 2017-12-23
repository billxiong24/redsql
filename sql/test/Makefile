.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
TEST_FUNC=../../test_functions/test_func.c

.PHONY: clean

test: run_test_sql_api.c test_sql_api.c $(TEST_FUNC)
	$(CC) $(CFLAGS) -o $(EXEC) ../../lib/CuTest.o test_sql_api.c run_test_sql_api.c ../sql_api.c ../../row/*.c $(TEST_FUNC) `$(MYSQL_EXEC)`

clean:
	-rm -f test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
