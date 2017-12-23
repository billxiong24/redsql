#ifndef SQL_API_H
#define SQL_API_H
#include <mysql.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../types.h"
#include "../row/_priv_row.h"

#include "../redsql_err.h"
//TODO add line number and file


/**
 * struct to encapsulate a MYSQL struct pointer, as well an error message.
 */
typedef struct MYSQL_WRAP MYSQL_WRAP;

/**
 * Initialize function for MYSQL_WRAP
 *
 * @param mysql the MYSQL * pointer to use for queries, etc.
 * @return pointer to struct of type MYSQL_WRAP * 
 */
MYSQL_WRAP *mysql_wrap_init(MYSQL *mysql);


char *mysql_wrap_get_err(MYSQL_WRAP *);

/**
 * This function pointer is passed in to stream_read_query, and is called
 * on every row of the resultant query, like a function on a stream. 
 */
typedef void (*stream_func)(MYSQL_ROW, size_t num_rows, size_t num_cols);

/**
 * Given query format and variable argument list, generate parsed query
 *
 * @param query the formatted query to be parsed
 * @param args the variable argument list passed in from another function
 *
 * @return A parsed, executable SQL query
 */
char *gen_query(const char *query, va_list args);
/**
 * Any query to do with reading from database, i.e. SELECT, SHOW, DESCRIBE
 * returns the query result as an array of array of strings (char *)
 */
RES_ROWS_ITER *sql_read(MYSQL_WRAP  *wrap, const char *query, va_list args); 

/**
 * Any query to do with writing to database, i.e. INSERT, UPDATE, DELETE
 * returns number of affected rows
 */
int32_t sql_write(MYSQL_WRAP *wrap, const char *query, va_list args);

/**
 * Execute read query and stream results, takes in function to execute each row
 * as it is streamed through
 */
void sql_stream_read_query(MYSQL_WRAP *wrap, const char *query, stream_func func, ...);

/**
 * Destructor for MYSQL_WRAP *, frees all memory
 */
void mysql_wrap_free(MYSQL_WRAP *);

#endif
