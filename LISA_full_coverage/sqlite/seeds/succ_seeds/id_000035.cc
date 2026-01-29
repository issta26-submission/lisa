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
//<ID> 35
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
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const void *origin_name = NULL;
    sqlite3_int64 mem_used = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_snap1 = 0;
    int rc_snap2 = 0;
    int cmp = 0;
    int sys_err = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize (open an in-memory database)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate and Validate (prepare/step a statement, inspect column origin, take snapshots, compare, query system/memory info)
    rc_prep = sqlite3_prepare16_v3(db, (const void *)u"SELECT 1 AS col;", -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    origin_name = sqlite3_column_origin_name16(stmt, 0);
    rc_snap1 = sqlite3_snapshot_open(db, "main", &snap1);
    rc_snap2 = sqlite3_snapshot_open(db, "main", &snap2);
    cmp = sqlite3_snapshot_cmp(&snap1, &snap2);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)origin_name;
    (void)mem_used;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)cmp;
    (void)sys_err;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}