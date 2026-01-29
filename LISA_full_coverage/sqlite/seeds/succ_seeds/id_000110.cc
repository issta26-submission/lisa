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
//<ID> 110
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
    sqlite3_str *str_builder = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure / Execute statements
    int rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    int rc_step_create = sqlite3_step(stmt_create);
    int rc_finalize_create = sqlite3_finalize(stmt_create);
    int rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    int rc_step_insert = sqlite3_step(stmt_insert);
    int rc_finalize_insert = sqlite3_finalize(stmt_insert);

    // step 3: Operate / Validate
    int sql_complete = sqlite3_complete(sql_select);
    int rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, NULL);
    int rc_step_select = sqlite3_step(stmt_select);
    int bytes_utf16 = sqlite3_column_bytes16(stmt_select, 0);
    str_builder = sqlite3_str_new(db);
    int str_err = sqlite3_str_errcode(str_builder);
    int combined_metric = rc_open + rc_prep_create + rc_step_create + rc_finalize_create + rc_prep_insert + rc_step_insert + rc_finalize_insert + sql_complete + rc_prep_select + rc_step_select + bytes_utf16 + str_err;
    (void)combined_metric;

    // step 4: Cleanup
    int rc_finalize_select = sqlite3_finalize(stmt_select);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)sql_complete;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)bytes_utf16;
    (void)str_builder;
    (void)str_err;
    (void)rc_finalize_select;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}