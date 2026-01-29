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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_snapshot snapA;
    sqlite3_snapshot snapB;
    const void *origin_name = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_snap1 = 0;
    int rc_snap2 = 0;
    int cmp = 0;
    int sys_err = 0;
    sqlite3_int64 mem_used = 0;

    memset(&snapA, 0, sizeof(snapA));
    memset(&snapB, 0, sizeof(snapB));

    // step 2: Open database and prepare a simple SELECT statement (UTF-16)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare16(db, (const void*)L"SELECT 1 AS one;", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    // step 3: Operate / Validate - inspect column origin, query system errno and memory, use snapshots and compare
    origin_name = sqlite3_column_origin_name16(stmt, 0);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();
    rc_snap1 = sqlite3_snapshot_open(db, "main", &snapA);
    rc_snap2 = sqlite3_snapshot_open(db, "main", &snapB);
    cmp = sqlite3_snapshot_cmp(&snapA, &snapB);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    (void)origin_name;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)cmp;
    (void)sys_err;
    (void)mem_used;

    // API sequence test completed successfully
    return 66;
}