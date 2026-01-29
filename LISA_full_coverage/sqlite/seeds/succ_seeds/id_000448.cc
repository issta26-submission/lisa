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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT id, val FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_wal = 0;
    int is_explain = 0;
    sqlite3_int64 old_soft_limit = 0;
    sqlite3_int64 new_soft_limit = (sqlite3_int64)1024 * 1024; /* 1MB */

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);
    old_soft_limit = sqlite3_soft_heap_limit64(new_soft_limit);

    // step 3: Operate - create table, insert a row, prepare select and inspect statement
    rc_prepare = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_finalize = sqlite3_finalize(stmt_create);
    rc_prepare = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);
    rc_prepare = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    is_explain = sqlite3_stmt_isexplain(stmt_select);
    rc_step = sqlite3_step(stmt_select);
    rc_finalize = sqlite3_finalize(stmt_select);

    // step 4: Cleanup / Validate / Finalize
    sqlite3_close_v2(db);
    db = NULL;
    stmt_create = NULL;
    stmt_insert = NULL;
    stmt_select = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_wal;
    (void)is_explain;
    (void)old_soft_limit;
    (void)new_soft_limit;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}