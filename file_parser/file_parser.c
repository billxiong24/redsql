#include "file_parser.h"
#include <stdio.h>
#include <string.h>

#include "../dict/util/str_util.h"

#define KEY_DELIM "[KEY]"

/**
 * The struct definition contains 2 maps.
 *
 * The keyquery_map contains key-pair entries such that the key is the table,
 * and the value is a list of keys which modify or are affected by writing to the table.
 *
 * The tablekey_map contains key-pair entries such that the key is the key, and the value
 * is a list of tables that key either modifies, or changes upon writing to one of the tables.
 * This map also contains a mapping from key to SQL query.
 *
 * In essence, keyquery_map and tablekey_map are inverses of each other. We use these 2 maps
 * to determine which keys in the cache are invalidated based on the execution of a certain key.
 */
struct REDSQL_FILE_PARSER {
    size_t num_keys;
    TABLEKEY_MAP *tk_map;
    KEYQUERY_MAP *kq_map;
    DBM *dbm;
};

REDSQL_FILE_PARSER *redsql_file_parser_init(size_t capacity) {
    REDSQL_FILE_PARSER *parser = malloc(sizeof(struct REDSQL_FILE_PARSER));
    parser->tk_map = tablekey_init(capacity);
    parser->kq_map = keyquery_init(capacity);
    parser->dbm = dbm_init(capacity);
    parser->num_keys = 0;

    return parser;
}

static int check_skip(char *line) {
    if(strcmp(line, "\n") == 0 || *line == '#' || *line ==';') {
        return 1;
    }

    return 0;
}
static void remove_newline(char *line, ssize_t read) {
    if(line[read - 1] == '\n') {
        line[read - 1] = '\0';
    }
}

static void store_parsed_info(DICT *kv, REDSQL_FILE_PARSER *parser) {

    //TODO move these constants somewhere
    char **arr = str_util_split(dict_get(kv, "affected_tables"), ' ');
    char *key = dict_get(kv, "key");
    char *query = dict_get(kv, "query");
    //populate dirtybit map first with 0, indicating no eviction

    /**
     * iterate through list of tables, and populate maps appropriately.
     * tablekey maps key->list of tables
     * keyquery maps table->list of keys
     */

    int ind = 0;
    while(arr[ind]) {
        tablekey_put(parser->tk_map, key, arr[ind]);
        keyquery_put(parser->kq_map, arr[ind], key);
        ind++;
    }
    tablekey_set_query(parser->tk_map, key, query);

    free(arr);
    dict_free(kv, free);
}

static void parse_key_entries(REDSQL_FILE_PARSER *parser, FILE *fp, ssize_t read) {
    char *temp = NULL;
    int count = 0;
    DICT *kv = dict_init(8);

    while(count < 3) {
        size_t temp_size= 0;
        ssize_t temp_read = getline(&temp, &temp_size, fp);
        if(check_skip(temp)) {
            free(temp);
            continue;
        }
        remove_newline(temp, temp_read);

        char key_str[read];
        char *val_str;
        int ind = 0;
        //get key
        while(temp[ind] != '=') {
            key_str[ind] = temp[ind];
            ind++;
        }
        key_str[ind] = '\0';

        //the rest of the string is the value
        val_str = str_util_clone(temp + ind + 1);

        dict_put(kv, key_str, val_str);
        count++;
        free(temp);
    }

    store_parsed_info(kv, parser);
}

static size_t count_num_keys(FILE *fp) {
    char *line = NULL;
    size_t size = 0;
    ssize_t read = 0;
    int num_keys = 0;

    while((read = getline(&line, &size, fp)) != -1) {
        remove_newline(line, read);
        if(strcmp(line, KEY_DELIM) == 0) {
            num_keys++;
        }
    }

    //after iterating file, reset pointer to beginning of file
    fseek(fp, 0, SEEK_SET);

    return num_keys;
}

REDSQL_FILE_PARSER *redsql_fp_load(char *file) {
    FILE *fp = fopen(file, "r");
    if(!fp) {
        return NULL;
    }

    size_t num_keys = count_num_keys(fp);
    /*printf("num_keys = %d\n", num_keys);*/
    REDSQL_FILE_PARSER *parser = redsql_file_parser_init(num_keys);
    char *line = NULL;
    size_t size = 0;
    ssize_t read;
    //TODO make this portable?
    while((read = getline(&line, &size, fp)) != -1) {
        if(check_skip(line)) {
            continue;
        }
        remove_newline(line, read);

        if(strcmp(line, KEY_DELIM) == 0) {
            parse_key_entries(parser, fp, read);
        }
    }
    free(line);
    fclose(fp);

    parser->num_keys = num_keys;

    return parser;
}


KEY_INFO *redsql_fp_get_tables(REDSQL_FILE_PARSER *parser, char *key) {
    return tablekey_get(parser->tk_map, key);
}

NODE *redsql_fp_get_keys(REDSQL_FILE_PARSER *parser, char *table) {
    return keyquery_get(parser->kq_map, table);
}

size_t redsql_fp_num_keys(REDSQL_FILE_PARSER *parser) {
    return parser->num_keys;
}

unsigned char redsql_fp_get_dirtybit(REDSQL_FILE_PARSER *parser, char *key) {
    return dbm_get(parser->dbm, key);
}

unsigned char redsql_fp_set_dirtybit(REDSQL_FILE_PARSER *parser, char *key, unsigned char bit) {
    return dbm_put(parser->dbm, key, bit);
}

void redsql_fp_set_dirtybit_scan(REDSQL_FILE_PARSER *parser, char *pattern) {
    dbm_set_pattern_val(parser->dbm, pattern);
}

void redsql_fp_free(REDSQL_FILE_PARSER *parser) {
    tablekey_free(parser->tk_map);
    dbm_free(parser->dbm);
    keyquery_free(parser->kq_map);
    free(parser);
}
