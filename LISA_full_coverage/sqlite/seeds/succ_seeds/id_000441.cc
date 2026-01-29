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
//<ID> 441
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY);";
    const char *sql_explain = "EXPLAIN QUERY PLAN SELECT id FROM t;";
    int rc_open = 0;
    int rc_prep1 = 0;
    int rc_step1 = 0;
    int rc_finalize1 = 0;
    int rc_prep2 = 0;
    int rc_step2 = 0;
    int rc_finalize2 = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int is_explain = 0;
    sqlite3_int64 prev_soft_limit = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_soft_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1000000);
    rc_wal = sqlite3_wal_autocheckpoint(db, 1);

    // step 3: Operate - create table, then prepare and run an EXPLAIN statement
    rc_prep1 = sqlite3_prepare_v3(db, sql_create, -1, 0u, &stmt, NULL);
    rc_step1 = sqlite3_step(stmt);
    rc_finalize1 = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prep2 = sqlite3_prepare_v3(db, sql_explain, -1, 0u, &stmt, NULL);
    is_explain = sqlite3_stmt_isexplain(stmt);
    rc_step2 = sqlite3_step(stmt);
    rc_finalize2 = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup / Validate / Finalize
    sqlite3_soft_heap_limit64(prev_soft_limit);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep1;
    (void)rc_step1;
    (void)rc_finalize1;
    (void)rc_prep2;
    (void)rc_step2;
    (void)rc_finalize2;
    (void)rc_wal;
    (void)rc_close;
    (void)is_explain;
    (void)sql_create;
    (void)sql_explain;
    (void)prev_soft_limit;

    // API sequence test completed successfully
    return 66;
}