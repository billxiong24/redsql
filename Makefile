LIB_NAME=libredsql.so
.DEFAULT_GOAL := $(LIB_NAME)
CC=gcc
CFLAGS= -Wall
EXEC=main
REDSQL_H=redsql/redsql.h 
REDSQL_C=redsql/redsql.c
MYSQL_EXEC=/usr/bin/mysql_config --cflags --libs
REDIS_API=redis/redis_api.c
SQL_API=sql/sql_api.c

REDIS_DIR=redis/test
SQL_DIR=sql/test
REDSQL_DIR=redsql/test
TEST_EXEC=test

LINKS=-levent -lhiredis

COMP=$(CC) $(CFLAGS) -fPIC -c 
OBJ=redsql/redsql.o redis/redis_api.o sql/sql_api.o row/_priv_row.o row/_priv_row_def.o row/_priv_row_redis.o row/_priv_row_sql.o

#OBJ_COMP=$(notdir $(OBJ))
OBJDIR=out/

# modify include path so that mysql's headers can be found
export C_INCLUDE_PATH=.:/usr/include/mysql/:/usr/include


.PHONY: test clean redsql

$(LIB_NAME): $(OBJ)
	$(CC) -shared -Wl,-soname,libredsql.so -o libredsql.so $(OBJ)


redsql/redsql.o: $(REDSQL_C) $(REDSQL_H)
	$(COMP) $< -o $@

%_api.o : %_api.c %_api.h _priv_%_api.h
	$(COMP) $< -o $@

_priv_row%.o: _priv_row%.c _priv_row%.h
	$(COMP) $< -o $@

redsql: $(REDSQL_C) $(REDSQL_H) sql/* row/* redis/* types.h main.c
	$(CC) $(CFLAGS) -o $(EXEC)  main.c $(REDSQL_C) $(REDIS_API) $(SQL_API) row/*.c `$(MYSQL_EXEC)` $(LINKS)

clean:
	-rm -f *.o *.so $(EXEC) file_test
	-find . -name "dump.rdb" -type f -delete
	-find . ! -name CuTest.o -name '*.o' -type f -delete
	-scripts/rmswp.sh

leak:
	valgrind --leak-check=full -v ./$(EXEC)

test:
	$(MAKE) -C $(REDIS_DIR)
	$(MAKE) -C $(SQL_DIR)
	$(MAKE) -C $(REDSQL_DIR)

	$(REDIS_DIR)/$(TEST_EXEC)
	$(SQL_DIR)/$(TEST_EXEC)
	$(REDSQL_DIR)/$(TEST_EXEC)

	$(MAKE) clean -C $(REDIS_DIR)
	$(MAKE) clean -C $(SQL_DIR)
	$(MAKE) clean -C $(REDSQL_DIR)

	$(MAKE) clean
