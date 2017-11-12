#include <stdio.h>
#include "redsql.h"
#include "sql/sql_api.h"
#include <stdarg.h>

/*int main(void) {*/
    /*MYSQL *con = mysql_init(NULL);*/
    
    /*return 0;*/
/*}*/
#define STRING_SIZE 50

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,col2 VARCHAR(40),col3 SMALLINT, col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"

static char *test(char *fmt, ...) {
    char *string = malloc(10);
    va_list arg_ptr;

    va_start(arg_ptr, fmt);
    vsnprintf(string, 10, fmt, arg_ptr);
    va_end(arg_ptr);
    return string;
}

int main(void) {




    MYSQL * mysql = mysql_init(NULL);
    if (mysql_real_connect(mysql, "localhost", "root", "Chem1313#", "chatdb", 0, NULL, 0) == NULL) {
        fprintf(stderr, "connection failed\n");
        exit(1);
    }
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[3];
    my_ulonglong  affected_rows;
    int           param_count;
    short         small_data;
    int           int_data;
    char          str_data[STRING_SIZE];
    unsigned long str_length;
    my_bool       is_null;

    /*[> Prepare an INSERT query with 3 parameters <]*/
    /*[> (the TIMESTAMP column is not named; the server <]*/
    /*[>  sets it to the current date and time) <]*/
    /*stmt = mysql_stmt_init(mysql);*/
    /*if (!stmt)*/
    /*{*/
      /*fprintf(stderr, " mysql_stmt_init(), out of memory\n");*/
      /*exit(0);*/
    /*}*/
    /*if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))*/
    /*{*/
      /*fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");*/
      /*fprintf(stderr, " %s\n", mysql_stmt_error(stmt));*/
      /*exit(0);*/
    /*}*/
    /*fprintf(stdout, " prepare, INSERT successful\n");*/

    /*[> Get the parameter count from the statement <]*/
    /*param_count= mysql_stmt_param_count(stmt);*/
    /*fprintf(stdout, " total parameters in INSERT: %d\n", param_count);*/

    /*if (param_count != 3) [> validate parameter count <]*/
    /*{*/
      /*fprintf(stderr, " invalid parameter count returned by MySQL\n");*/
      /*exit(0);*/
    /*}*/

    /*[> Bind the data for all 3 parameters <]*/

    /*memset(bind, 0, sizeof(bind));*/

    /*[> INTEGER PARAM <]*/
    /* This is a number type, so there is no need
     *    to specify buffer_length */
    /*printf("MYSQL_TYPE_STRING = %d\n", MYSQL_TYPE_STRING);*/
    /*printf("MYSQL_TYPE_LONG = %d\n", MYSQL_TYPE_LONG);*/
    /*bind[0].buffer_type= MYSQL_TYPE_LONG;*/
    /*bind[0].buffer= (char *)&int_data;*/
    /*bind[0].is_null= 0;*/
    /*bind[0].length= &str_length;*/

    /*[> STRING PARAM <]*/
    /*bind[1].buffer_type= MYSQL_TYPE_STRING;*/
    /*bind[1].buffer= (char *)str_data;*/
    /*bind[1].buffer_length= STRING_SIZE;*/
    /*bind[1].is_null= 0;*/
    /*bind[1].length= &str_length;*/

    /*[> SMALLINT PARAM <]*/
    /*bind[2].buffer_type= MYSQL_TYPE_SHORT;*/
    /*bind[2].buffer= (char *)&small_data;*/
    /*bind[2].is_null= &is_null;*/
    /*bind[2].length= &str_length;*/

    /*[> Bind the buffers <]*/
    /*if (mysql_stmt_bind_param(stmt, bind))*/
    /*{*/
      /*fprintf(stderr, " mysql_stmt_bind_param() failed\n");*/
      /*fprintf(stderr, " %s\n", mysql_stmt_error(stmt));*/
      /*exit(0);*/
    /*}*/

    /*[> Specify the data values for the first row <]*/
    /*int_data= 10;             [> integer <]*/
    /*strncpy(str_data, "MySQL", STRING_SIZE); [> string  <]*/
    /*str_length= strlen(str_data);*/

    /*[> INSERT SMALLINT data as NULL <]*/
    /*is_null= 1;*/

    /*[> Execute the INSERT statement - 1<]*/
    /*if (mysql_stmt_execute(stmt))*/
    /*{*/
      /*fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");*/
      /*fprintf(stderr, " %s\n", mysql_stmt_error(stmt));*/
      /*exit(0);*/
    /*}*/

    /*[> Get the number of affected rows <]*/
    /*affected_rows= mysql_stmt_affected_rows(stmt);*/
    /*fprintf(stdout, " total affected rows(insert 1): %lu\n",*/
                            /*(unsigned long) affected_rows);*/

    /*if (affected_rows != 1) [> validate affected rows <]*/
    /*{*/
      /*fprintf(stderr, " invalid affected rows by MySQL\n");*/
      /*exit(0);*/
    /*}*/

    /* Specify data values for second row,
     *    then re-execute the statement */
    /*int_data= 1000;*/
    /*strncpy(str_data, "The most popular Open Source database", STRING_SIZE);*/
    /*str_length= strlen(str_data);*/
    /*small_data= 1000;         [> smallint <]*/
    /*is_null= 0;               [> reset <]*/

    /*[> Execute the INSERT statement - 2<]*/
    /*if (mysql_stmt_execute(stmt))*/
    /*{*/
      /*fprintf(stderr, " mysql_stmt_execute, 2 failed\n");*/
      /*fprintf(stderr, " %s\n", mysql_stmt_error(stmt));*/
      /*exit(0);*/
    /*}*/

    /*[> Get the total rows affected <]*/
    /*affected_rows= mysql_stmt_affected_rows(stmt);*/
    /*fprintf(stdout, " total affected rows(insert 2): %lu\n",*/
                            /*(unsigned long) affected_rows);*/

    /*if (affected_rows != 1) [> validate affected rows <]*/
    /*{*/
      /*fprintf(stderr, " invalid affected rows by MySQL\n");*/
      /*exit(0);*/
    /*}*/

    /*[> Close the statement <]*/
    /*if (mysql_stmt_close(stmt))*/
    /*{*/
      /*[> mysql_stmt_close() invalidates stmt, so call          <]*/
      /*[> mysql_error(mysql) rather than mysql_stmt_error(stmt) <]*/
      /*fprintf(stderr, " failed while closing the statement\n");*/
      /*fprintf(stderr, " %s\n", mysql_error(mysql));*/
      /*exit(0);*/
    /*}*/

    /*if(mysql_query(mysql, "DESCRIBE test_table")) {*/
        /*fprintf(stderr, "error describing table");*/
        /*exit(1);*/
    /*}*/

    char *query = "SELECT id, chat_name, code, username, creator from Chat INNER JOIN MemberOf ON MemberOf.chat_id = Chat.id WHERE id = '%s'";


    SQL_ROWS *sql_rows = read_query(mysql, query,  "0043e138f3a1daf9ccfbf718fc9acd48");

    
    for (int i = 0; i < get_num_rows(sql_rows); i++) {
        for(int j = 0; j < get_num_cols(sql_rows); j++) {
            char *res = get_row(sql_rows, i, j);
            if(res) {
                puts(res);
            }
            else {
                puts("nulld");
            }
        }
    }

    /*for (int i = 0; i < get_num_rows(rows); i++) {*/
        /*MYSQL_ROW row = get_row(rows, i);*/
        /*for(int j = 0; j < get_num_cols(rows); j++) {*/
            /*if(row[j]) {*/
                /*puts(row[j]);*/
            /*}*/
            /*else {*/
                /*puts("|Null");*/
            /*}*/
        /*}*/
    /*}*/


    /*for (int i = 0; i < list->size; i++) {*/
        /*char **strs = list[i].list->string_arr;*/
        /*size_t size = list[i].list->size;*/

        /*for(int j = 0; j < size; j++) {*/
        /*}*/
    /*}*/


    /*MYSQL_RES * result = mysql_store_result(mysql);*/
    /*int num_rows = mysql_num_rows(result);*/
    /*printf("num_rows = %d\n", num_rows);*/

    /*MYSQL_ROW row;*/
    /*[>MYSQL_FIELD *FIELD;<]*/
    /*int rows = mysql_num_fields(result);*/

    /*//for each rows*/
    /*while(row = mysql_fetch_row(result)) {*/
        /*//row is an array of strings*/
        /*char **c = row;*/
        /*//for each column*/
        /*for (int i = 0; i < rows; i++) {*/
            /*if(row[i] == NULL) {*/
               /*puts("nullll");*/
               /*continue;*/
            /*}*/
            /*printf("%s \n", row[i]);*/
        /*}*/
    /*}*/
    return 0;
}
