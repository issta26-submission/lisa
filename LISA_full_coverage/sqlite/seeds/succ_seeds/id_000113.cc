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
//<ID> 113
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
    sqlite3_str *str_obj = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('hello');";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fini_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_fini_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int col_bytes16 = 0;
    int str_err = 0;
    int sql_complete = 0;
    void *tmp_free = NULL;

    // step 2: Configure / Execute statements
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v2(db, create_sql, -1, &stmt_create, &pzTail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fini_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, &pzTail);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fini_insert = sqlite3_finalize(stmt_insert);
    rc_prep_select = sqlite3_prepare_v2(db, select_sql, -1, &stmt_select, &pzTail);

    // step 3: Operate / Validate
    rc_step_select = sqlite3_step(stmt_select);
    col_bytes16 = sqlite3_column_bytes16(stmt_select, 0);
    str_obj = sqlite3_str_new(db);
    str_err = sqlite3_str_errcode(str_obj);
    sql_complete = sqlite3_complete(select_sql);

    // step 4: Cleanup
    rc_fini_insert = rc_fini_insert; /* keep variable used */
    tmp_free = sqlite3_realloc(str_obj, 0);
    (void)tmp_free;
    rc_fini_create = rc_fini_create;
    sqlite3_finalize(stmt_select);
    sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)col_bytes16;
    (void)str_err;
    (void)sql_complete;
    (void)pzTail;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)db;

    // API sequence test completed successfully
    return 66;
}