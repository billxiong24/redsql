
#ifndef SQL_API_H
#define SQL_API_H
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <inttypes.h>


typedef struct SQL_ROWS SQL_ROWS;
typedef void (*stream_func)(MYSQL_ROW);

/**
 * Any query to do with reading from database, i.e. SELECT, SHOW, DESCRIBE
 * returns the query result as an array of array of strings (char *)
 */
SQL_ROWS *read_query(MYSQL *mysql, char *query, ...);

/**
 * Any query to do with writing to database, i.e. INSERT, UPDATE, DELETE
 * returns number of affected rows
 */
uint32_t write_query(MYSQL *mysql, char *query, ...);

/**
 * Execute read query and stream results, takes in function to execute each row
 * as it is streamed through
 */
void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...);

/**
 * return number of rows in resultant query
 */
uint32_t get_num_rows(SQL_ROWS *rows);

/**
 * return number of fields in table of resultant query
 */
uint32_t get_num_cols(SQL_ROWS *rows);

/**
 * get a specific row in resultant query
 */
char *get_row(SQL_ROWS *rows, uint32_t row, uint32_t col);

/**
 * destructor for SQL_ROWS type structs
 */
void free_sql_rows(SQL_ROWS *rows);

//TODO prepared statements

#endif
