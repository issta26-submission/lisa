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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *pstr = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('hello');";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure / Execute DDL and DML via prepared statements
    int rc_prep_create = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    int rc_step_create = sqlite3_step(stmt);
    int rc_finalize_create = sqlite3_finalize(stmt);
    stmt = NULL;
    int rc_prep_insert = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_finalize_insert = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate (prepare SELECT, step, read UTF-16 byte count, check SQL completeness and str error)
    int rc_prep_select = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    int rc_step_select = sqlite3_step(stmt);
    int bytes_utf16 = sqlite3_column_bytes16(stmt, 0);
    const void *text16 = sqlite3_column_text16(stmt, 0);
    pstr = sqlite3_str_new(db);
    int str_err = sqlite3_str_errcode(pstr);
    int sql_complete = sqlite3_complete(select_sql);

    // step 4: Cleanup
    int rc_finalize_select = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)bytes_utf16;
    (void)text16;
    (void)str_err;
    (void)sql_complete;
    (void)rc_finalize_select;
    (void)rc_close;
    (void)pstr;
    (void)db;
    // API sequence test completed successfully
    return 66;
}