#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_str *builder = NULL;
    char *sql_fragment1 = (char *)"CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);";
    char *sql_fragment2 = (char *)"INSERT INTO t(value) VALUES('a');";
    char *sql_select = (char *)"SELECT count(*) FROM t;";
    const char *tail = NULL;
    char *final_sql = NULL;
    void *realloc_buf = NULL;
    void *realloc_buf2 = NULL;
    int rc_threadsafe = sqlite3_threadsafe();
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete_flag = 0;
    int row_count = 0;

    // step 2: Configure / Build SQL and allocate buffers
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, sql_fragment1, (int)strlen(sql_fragment1));
    final_sql = sqlite3_str_finish(builder);
    complete_flag = sqlite3_complete(final_sql);
    realloc_buf = sqlite3_realloc(NULL, 64);
    realloc_buf2 = sqlite3_realloc(realloc_buf, 128);

    // step 3: Operate / Execute statements and validate results
    rc_prep = sqlite3_prepare_v3(db, final_sql, -1, 0, &stmt_create, &tail);
    rc_step = sqlite3_step(stmt_create);
    rc_finalize = sqlite3_finalize(stmt_create);
    rc_prep = sqlite3_prepare_v3(db, sql_fragment2, -1, 0, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    row_count = sqlite3_column_int(stmt_select, 0);
    rc_finalize = sqlite3_finalize(stmt_select);
    sqlite3_result_null((sqlite3_context *)NULL);

    // step 4: Cleanup / Free resources and close
    sqlite3_realloc(realloc_buf2, 0);
    sqlite3_realloc(final_sql, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_threadsafe;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)complete_flag;
    (void)row_count;
    (void)tail;
    (void)builder;
    (void)sql_fragment1;
    (void)sql_fragment2;
    (void)sql_select;
    (void)realloc_buf;
    (void)realloc_buf2;
    (void)final_sql;
    (void)db;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;

    // API sequence test completed successfully
    return 66;
}