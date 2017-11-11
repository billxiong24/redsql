#include <mysql/my_global.h>
#include <mysql/mysql.h>

typedef struct {
    char ** string_arr;
    size_t size;
} list;

typedef struct {
    list *list;
    size_t size;
} arr_list;

typedef struct {
    MYSQL *mysql;
    char *query;
} sql_query;

typedef void (*stream_func)(MYSQL_ROW);

/**
 * Any query to do with reading from database, i.e. SELECT, SHOW, DESCRIBE
 * returns the query result as an array of array of strings (char *)
 */
arr_list *read_query(MYSQL *mysql, char *query, ...);

/**
 * Any query to do with writing to database, i.e. INSERT, UPDATE, DELETE
 * returns number of affected rows
 */
unsigned int write_query(MYSQL *mysql, char *query, ...);

/**
 * Execute read query and stream results, takes in function to execute each row
 * as it is streamed through
 */
void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...);

//TODO prepared statements
