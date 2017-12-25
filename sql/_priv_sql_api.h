/**
 * If consumer of this library, do not include this file!
 * This file is for internal use in conjunction with sql_api.h- Refer to sql_api.h for public API
 */
#ifndef _PRIV_SQL_API_H
#define _PRIV_SQL_API_H

struct MYSQL_WRAP{
    MYSQL *mysql;
    char *err;
};

#endif
