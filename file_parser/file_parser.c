#include "file_parser.h"
#include <stdio.h>
#include <string.h>

#include "../dict/keyquery_map/keyquery_map.h"
#include "../dict/tablekey_map/tablekey_map.h"
#include "../dict/util/str_util.h"

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
    TABLEKEY_MAP *tk_map;
    KEYQUERY_MAP *kq_map;
};

REDSQL_FILE_PARSER *redsql_file_parser_init(size_t capacity) {
    REDSQL_FILE_PARSER *parser = malloc(sizeof(struct REDSQL_FILE_PARSER));
    parser->tk_map = tablekey_init(capacity);
    parser->kq_map = keyquery_init(capacity);

    return parser;
}

int check_skip(char *line) {
    if(strcmp(line, "\n") == 0 || *line == '#' || *line ==';') {
        return 1;
    }

    return 0;
}
int remove_newline(char *line, ssize_t read) {
    if(line[read - 1] == '\n') {
        line[read - 1] = '\0';
    }
}

void store_parsed_info(DICT *kv, REDSQL_FILE_PARSER *parser) {

    //TODO move these constants somewhere
    char **arr = str_util_split(dict_get(kv, "affected_tables"), ' ');
    char *key = dict_get(kv, "key");
    char *query = dict_get(kv, "query");

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

void parse_key_entries(REDSQL_FILE_PARSER *parser, FILE *fp, ssize_t read) {
    char *temp = NULL;
    ssize_t temp_size = 0;
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

REDSQL_FILE_PARSER *redsql_file_parser_load(char *file) {
    REDSQL_FILE_PARSER *parser = redsql_file_parser_init(32);
    FILE *fp = fopen(file, "r");
    if(!fp) {
        return NULL;
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t read;

    //TODO make this portable?
    while((read = getline(&line, &size, fp)) != -1) {
        if(check_skip(line)) {
            continue;
        }
        remove_newline(line, read);

        if(strcmp(line, "[KEY]") == 0) {
            parse_key_entries(parser, fp, read);
        }
    }
    free(line);
    fclose(fp);

    return parser;
}
