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
//<ID> 446
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
    const char *sql_select = "SELECT val FROM t;";
    int rc_open = 0;
    int rc_prepare_create = 0;
    int rc_prepare_insert = 0;
    int rc_prepare_select = 0;
    int rc_step_create = 0;
    int rc_step_insert = 0;
    int rc_step_select = 0;
    int rc_isexplain_create = 0;
    int rc_isexplain_insert = 0;
    int rc_isexplain_select = 0;
    int rc_wal = 0;
    const unsigned char *col_text = NULL;
    sqlite3_int64 last_rowid = 0;
    sqlite3_int64 prev_heap_limit = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_heap_limit = sqlite3_soft_heap_limit64((sqlite3_int64)(8 * 1024 * 1024)); // set 8MB soft heap limit

    // step 3: Operate - create table, insert a row, select it, and configure WAL autocheckpoint
    rc_prepare_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_isexplain_create = sqlite3_stmt_isexplain(stmt_create);
    rc_step_create = sqlite3_step ? sqlite3_step(stmt_create) : 0;
    if (stmt_create) sqlite3_finalize(stmt_create);
    stmt_create = NULL;

    rc_prepare_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_isexplain_insert = sqlite3_stmt_isexplain(stmt_insert);
    rc_step_insert = sqlite3_step ? sqlite3_step(stmt_insert) : 0;
    last_rowid = sqlite3_last_insert_rowid(db);
    if (stmt_insert) sqlite3_finalize(stmt_insert);
    stmt_insert = NULL;

    rc_prepare_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    rc_isexplain_select = sqlite3_stmt_isexplain(stmt_select);
    rc_step_select = sqlite3_step ? sqlite3_step(stmt_select) : 0;
    col_text = sqlite3_column_text ? sqlite3_column_text(stmt_select, 0) : NULL;
    if (stmt_select) sqlite3_finalize(stmt_select);
    stmt_select = NULL;

    rc_wal = sqlite3_wal_autocheckpoint(db, 10);

    // step 4: Cleanup / Validate / Finalize
    sqlite3_close_v2 ? sqlite3_close_v2(db) : 0;
    db = NULL;

    (void)rc_open;
    (void)prev_heap_limit;
    (void)rc_prepare_create;
    (void)rc_prepare_insert;
    (void)rc_prepare_select;
    (void)rc_step_create;
    (void)rc_step_insert;
    (void)rc_step_select;
    (void)rc_isexplain_create;
    (void)rc_isexplain_insert;
    (void)rc_isexplain_select;
    (void)rc_wal;
    (void)col_text;
    (void)last_rowid;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}