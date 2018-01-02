#include <stdlib.h>
#include <openssl/sha.h>
#include "rsql.h"
#include "redsql.h"
#include "../file_parser/file_parser.h"
#include "../dict/dirtybit_map/dirtybit_map.h"

struct REDSQL {
    REDSQL_FILE_PARSER *parser;
    redsql_conn *rconn;
};

REDSQL *redsql_init(char *sql_host, char *sql_user, char *sql_pass, char *db, char *redis_host, unsigned int redis_port) {

    REDSQL *obj = malloc(sizeof(REDSQL));
    obj->parser = NULL;
    obj->rconn = establish_conn(sql_host, sql_user, sql_pass, db, redis_host, redis_port);

    return obj;
}

void redsql_load_config(REDSQL *redsql, char *file) {
    redsql->parser = redsql_fp_load(file);
}

unsigned long redsql_store(REDSQL *redsql, char *key, ...) {
    KEY_INFO *info = redsql_fp_get_tables(redsql->parser, key);
    char *query = info->query;
    NODE *tables = info->tables;
    size_t evicted;

    //set dirty bits for necessary keys, i.e. keys that depend on table(s) being written to
    while(tables) {
        NODE *keys_to_evict = redsql_fp_get_keys(redsql->parser, tables->val);
        //mark all the keys dependent on the table being written to as dirty
        while(keys_to_evict) {
            redsql_fp_set_dirtybit(redsql->parser, keys_to_evict->val, 1);
            keys_to_evict = keys_to_evict->next;
            evicted++;
        }
        tables = tables->next;
    }

    //TODO construct and hash query, and write to DB

    return evicted;
}

RES_ROWS_ITER *redsql_retrieve(REDSQL *redsql, char *key, ...) {

}
