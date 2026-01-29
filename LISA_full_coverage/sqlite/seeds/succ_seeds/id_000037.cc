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
//<ID> 37
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
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const void *origin_name = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int rc_snap1 = 0;
    int rc_snap2 = 0;
    int cmp_res = 0;
    int sys_err = 0;
    sqlite3_int64 mem_used = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations - create table, insert a row, select the row and obtain column origin name
    rc_prep_create = sqlite3_prepare16(db, (const void*)L"CREATE TABLE t(a INTEGER);", -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);

    rc_prep_insert = sqlite3_prepare16(db, (const void*)L"INSERT INTO t(a) VALUES(42);", -1, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);

    rc_prep_select = sqlite3_prepare16(db, (const void*)L"SELECT a FROM t;", -1, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    origin_name = sqlite3_column_origin_name16(stmt_select, 0);
    rc_finalize_select = sqlite3_finalize(stmt_select);

    // step 4: Validation and Cleanup - open snapshots, compare them, query system error and memory used, then close DB
    rc_snap1 = sqlite3_snapshot_open(db, "main", &snap1);
    rc_snap2 = sqlite3_snapshot_open(db, "main", &snap2);
    cmp_res = sqlite3_snapshot_cmp(&snap1, &snap2);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_finalize_select;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)cmp_res;
    (void)origin_name;
    (void)sys_err;
    (void)mem_used;

    // API sequence test completed successfully
    return 66;
}