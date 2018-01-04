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

DBM_DIR=dict/dirtybit_map/dirtybit_map
KQ_DIR=dict/keyquery_map/keyquery_map
TK_DIR=dict/tablekey_map/tablekey_map
NODE_DIR=dict/node/node
UTIL_DIR=dict/util/str_util

DICT=dict/dict.c dict/dict.h dict/_priv_dict.h
DBM=$(DBM_DIR).c $(DBM_DIR).h
KEYQUERY=$(KQ_DIR).c $(KQ_DIR).h 
TABLEKEY=$(TK_DIR).c $(TK_DIR).h
NODE=$(NODE_DIR).c $(NODE_DIR).h
UTIL=$(UTIL_DIR).c $(UTIL_DIR).h


LINKS=-levent -L. -lredsql -lhiredis -lcrypto -lssl

COMP=$(CC) $(CFLAGS) -fPIC -c 
OBJ=redsql/redsql.o redis/redis_api.o sql/sql_api.o row/_priv_row.o row/_priv_row_def.o row/_priv_row_redis.o row/_priv_row_sql.o redsql/rsql.o dict.o dbm.o keyquery.o tablekey.o node.o util.o


# modify include path so that mysql's headers can be found
export C_INCLUDE_PATH=.:/usr/include/mysql/:/usr/include


.PHONY: test clean redsql

$(LIB_NAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$(LIB_NAME) -o $(LIB_NAME) $(OBJ)


redsql/rsql.o: redsql/rsql.c redsql/rsql.h
	$(COMP) $< -o $@

redsql/redsql.o: $(REDSQL_C) $(REDSQL_H)
	$(COMP) $< -o $@

%_api.o : %_api.c %_api.h _priv_%_api.h
	$(COMP) $< -o $@

_priv_row%.o: _priv_row%.c _priv_row%.h
	$(COMP) $< -o $@

dict.o: $(DICT)
	$(COMP) $< -o $@
dbm.o: $(DBM)
	$(COMP) $< -o $@
keyquery.o: $(KEYQUERY)
	$(COMP) $< -o $@
tablekey.o: $(TABLEKEY)
	$(COMP) $< -o $@
node.o: $(NODE)
	$(COMP) $< -o $@
util.o: $(UTIL)
	$(COMP) $< -o $@

redsql: $(LIB_NAME)
	$(CC) $(CFLAGS) -o $(EXEC)  main.c file_parser/file_parser.c $(LINKS) `$(MYSQL_EXEC)`

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
