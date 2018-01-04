#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include "../dict/keyquery_map/keyquery_map.h"
#include "../dict/tablekey_map/tablekey_map.h"
#include "../dict/dirtybit_map/dirtybit_map.h"

typedef struct REDSQL_FILE_PARSER REDSQL_FILE_PARSER; 

REDSQL_FILE_PARSER *redsql_fp_load(char *file);

KEY_INFO *redsql_fp_get_tables(REDSQL_FILE_PARSER *parser, char *table);
NODE *redsql_fp_get_keys(REDSQL_FILE_PARSER *parser, char *key);
unsigned char redsql_fp_get_dirtybit(REDSQL_FILE_PARSER *, char *key);

unsigned char redsql_fp_set_dirtybit(REDSQL_FILE_PARSER *, char *, unsigned char bit);

void redsql_fp_set_dirtybit_scan(REDSQL_FILE_PARSER *parser, char *pattern);

size_t redsql_fp_num_keys(REDSQL_FILE_PARSER *parser);

void redsql_fp_free(REDSQL_FILE_PARSER *);

#endif
