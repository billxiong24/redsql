.DEFAULT_GOAL := test
CC=gcc
CFLAGS=-Wall
EXEC=test
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs

.PHONY: test clean

test: run_test_sql_api.c test_sql_api.c
	$(CC) $(CFLAGS) -o $(EXEC) ../../lib/CuTest.o test_sql_api.c run_test_sql_api.c ../sql_api.c ../../row/*.c `$(MYSQL_EXEC)`  

clean:
	-rm test

leak:
	valgrind --leak-check=full -v ./$(EXEC)
