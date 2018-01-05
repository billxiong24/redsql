#include <stdio.h>
#include "redsql/redsql.h"
#include "redsql/rsql.h"

#define STRING_SIZE 50

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,col2 VARCHAR(40),col3 SMALLINT, col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"

int main(void) {
    REDSQL *redsql = redsql_init("localhost", "root", "", "employees", "localhost", 6379);
    redsql_load_config(redsql, "example.cfg");

    puts("finished loading config");

    /*redsql_retrieve(redsql, "get_department", "d007");*/
    /*redsql_store(redsql, "new_department", "d011", "department name");*/
    /*redsql_retrieve(redsql, "get_department", "d007");*/
    /*redsql_store(redsql, "del_department", "d011", "department name");*/
    /*redsql_retrieve(redsql, "get_department", "d008");*/
    /*redsql_retrieve(redsql, "get_department", "d007");*/
    /*redsql_retrieve(redsql, "get_department", "d007");*/
    /*redsql_retrieve(redsql, "get_department", "d008");*/
    /*redsql_retrieve(redsql, "get_department", "d008");*/
    /*redsql_store(redsql, "employee_salary", 80000, 3);*/
    /*redsql_retrieve(redsql, "get_department", "d008");*/
    /*redsql_store(redsql, "get_manager", "11023");*/
    /*redsql_retrieve(redsql, "get_department", "d008");*/
    /*redsql_retrieve(redsql, "title_gender", "F", "Senior Engineer");*/
    /*redsql_store(redsql, "employee_salary", 80000, 3);*/
    /*redsql_retrieve(redsql, "title_gender", "M", "Senior Engineer");*/

    /*redsql_retrieve(redsql, "title_gender", "F", "Senior Engineer");*/
    /*redsql_retrieve(redsql, "title_gender", "M", "Senior Engineer");*/
    /*RES_ROWS_ITER *iter = redsql_retrieve(redsql, "get_department", "d008");*/

    /*while(res_row_iter_has_next(iter)) {*/
        /*char **next = res_row_iter_next(iter);*/
        /*puts(next[0]);*/
    /*}*/
    redsql_close(redsql);
    return 0;
}
