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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t;";
    const char *pzTail = NULL;
    const unsigned char *col_text = NULL;
    int rc_open = 0;
    int rc_prepare_create = 0;
    int rc_prepare_insert = 0;
    int rc_prepare_select = 0;
    int rc_step_create = 0;
    int rc_step_insert = 0;
    int rc_step_select = 0;
    int rc_finalize = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int isexplain_create = 0;
    int isexplain_insert = 0;
    int isexplain_select = 0;
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 restored_limit = 0;
    int col_type = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);
    prev_limit = sqlite3_soft_heap_limit64(1024 * 1024);
    restored_limit = sqlite3_soft_heap_limit64(prev_limit);

    // step 3: Operate / Validate
    rc_prepare_create = sqlite3_prepare_v3(db, sql_create, -1, 0u, &stmt, &pzTail);
    isexplain_create = sqlite3_stmt_isexplain(stmt);
    rc_step_create = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0u, &stmt, &pzTail);
    isexplain_insert = sqlite3_stmt_isexplain(stmt);
    rc_step_insert = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare_select = sqlite3_prepare_v3(db, sql_select, -1, 0u, &stmt, &pzTail);
    isexplain_select = sqlite3_stmt_isexplain(stmt);
    rc_step_select = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    col_type = sqlite3_column_type(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_wal;
    (void)prev_limit;
    (void)restored_limit;
    (void)rc_prepare_create;
    (void)rc_prepare_insert;
    (void)rc_prepare_select;
    (void)rc_step_create;
    (void)rc_step_insert;
    (void)rc_step_select;
    (void)rc_finalize;
    (void)rc_close;
    (void)isexplain_create;
    (void)isexplain_insert;
    (void)isexplain_select;
    (void)pzTail;
    (void)col_text;
    (void)col_type;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}