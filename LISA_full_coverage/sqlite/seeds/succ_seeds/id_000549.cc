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
//<ID> 549
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t1(txt) VALUES('hello');";
    const char *sql_select = "SELECT id, txt FROM t1;";
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    char *finished = NULL;
    int str_len = 0;
    sqlite3_int64 changes64 = 0;
    const char *tbl_name = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fin_create = 0;
    int rc_enable_shared = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_fin_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_fin_select = 0;
    int rc_close = 0;
    int col0_int = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open16((const void *)L":memory:", &db);
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fin_create = sqlite3_finalize(stmt_create);
    rc_enable_shared = sqlite3_enable_shared_cache(1);
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fin_insert = sqlite3_finalize(stmt_insert);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);

    // create a sqlite3_str and operate on it (instead of realloc-like calls)
    builder = sqlite3_str_new(db);
    str_val = sqlite3_str_value(builder);
    str_len = sqlite3_str_length(builder);
    sqlite3_str_reset(builder);

    // step 3: Operate / Validate
    rc_step_select = sqlite3_step(stmt_select);
    tbl_name = sqlite3_column_database_name(stmt_select, 0);
    changes64 = sqlite3_total_changes64(db);
    col0_int = sqlite3_column_int(stmt_select, 0);
    finished = sqlite3_str_finish(builder);
    sqlite3_free(finished);

    // step 4: Cleanup
    rc_fin_select = sqlite3_finalize(stmt_select);
    rc_close = sqlite3_close(db);

    (void)str_val;
    (void)str_len;
    (void)changes64;
    (void)tbl_name;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_fin_create;
    (void)rc_enable_shared;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_fin_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_fin_select;
    (void)rc_close;
    (void)col0_int;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}