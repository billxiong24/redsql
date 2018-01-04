#include <stdlib.h>
#include <openssl/sha.h>
#include "rsql.h"
#include "redsql.h"
#include "../file_parser/file_parser.h"
#include "../dict/dirtybit_map/dirtybit_map.h"
#include "_priv_redsql.h"

#define HASH_LEN 16 
#define KEY_DELIM ":"

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
    size_t evicted = 0L;
    
    //execute sql query to write to database
    va_list args;
    va_start(args, key);
    v_redsql_write(redsql->rconn, NULL, 0, query, args);

    //set dirty bits for necessary keys, i.e. keys that depend on table(s) being written to
    while(tables) {
        /*printf("tables: %s\n", tables->val);*/
        NODE *keys_to_evict = redsql_fp_get_keys(redsql->parser, tables->val);
        //mark all the keys dependent on the table being written to as dirty
        while(keys_to_evict) {
            //scan the keys and mark them dirty (e.g. all keys starting with k1, k2, etc.)
            redsql_fp_set_dirtybit_scan(redsql->parser, keys_to_evict->val);
            keys_to_evict = keys_to_evict->next;
            evicted++;
        }
        tables = tables->next;
    }

    return evicted;
}

RES_ROWS_ITER *redsql_retrieve(REDSQL *redsql, char *key, ...) {

    KEY_INFO *info = redsql_fp_get_tables(redsql->parser, key);
    if(!info) {
        fprintf(stderr, "Key not found\n.");
        return NULL;
    }

    //unformatted query e.g. SELECT * FROM table WHERE id=%d
    char *query = info->query;

    va_list args, args_copy, args2;
    va_start(args, key);
    va_copy(args_copy, args);

    //the formatted query
    char *query_gen = gen_query(query, args);
    size_t len = strlen(query_gen);

    /**
     * the sha1 hash to append to the key to generate unique key- every parsed query is unique
     * if key is "k1" then generated unique key is k1:{sha1 hash}
     */

    char hash[HASH_LEN];
    char ascii[HASH_LEN * 2];
    memset(hash, 0, HASH_LEN);
    memset(ascii, 0, HASH_LEN * 2);
    SHA256(query_gen, len, hash);
    //convert to ascii
    for (int i = 0; i < HASH_LEN; i++) {
        sprintf((char*)&(ascii[i*2]), "%02x", hash[i]);
    }
    
    free(query_gen);

    //the unique hash key({key}:{sha1 hash}) 1 for key_delim and 1 for null terminator
    char hash_key[len + HASH_LEN + 1 + 1];
    memset(hash_key, 0, len + HASH_LEN + 1 + 1);

    strncpy(hash_key, key, strlen(key));
    strcat(hash_key, KEY_DELIM);
    strcat(hash_key, ascii);

    unsigned char bit = redsql_fp_get_dirtybit(redsql->parser, hash_key);
    /*bool to_cache = (bit == DBM_NULL || bit == 0);*/
    bool to_cache = (bit == 0);

    redsql_fp_set_dirtybit(redsql->parser, hash_key, 0);
    return v_redsql_read(redsql->rconn, hash_key, query, to_cache, args_copy);
}
