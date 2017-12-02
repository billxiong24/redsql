#ifndef REDSQL_ERR_H
#define REDSQL_ERR_H

/**
 * Macros for error codes are defined here.
 * These error codes are used by all implementation files for
 * returning upon reaching an unrecoverable error
 */

#define ERR_NULL_STR_STR "Null pointer exception. Key or query was null.\n"
#define ERR_NULL_CONN_STR "Null pointer exception in connecting to service. \n"
#define ERR_NULL_ARGS_STR "Null pointer exception in passing in va_list \n"
#define ERR_NULL_WRAP_STR "Null pointer exception in structure \n"

#define ERR_NULL_STR -1
#define ERR_NULL_CONN -2
#define ERR_NULL_ARGS -3
#define ERR_NULL_WRAP -4

#endif
