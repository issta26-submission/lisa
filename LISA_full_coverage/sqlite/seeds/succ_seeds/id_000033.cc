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
//<ID> 33
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
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const void *origin_name16 = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_snap1 = 0;
    int rc_snap2 = 0;
    int snap_cmp = 0;
    int sys_err = 0;
    sqlite3_int64 mem_used = 0;
    char16_t sql16[] = u"SELECT 1;";

    // ensure snapshot storage is zeroed
    memset(&snap1, 0, sizeof(snap1));
    memset(&snap2, 0, sizeof(snap2));

    // step 2: Open database and prepare a UTF-16 statement
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare16(db, (const void *)sql16, -1, &stmt, NULL);

    // step 3: Operate - step the statement, query column origin, take snapshots, compare, query system and memory
    rc_step = sqlite3_step(stmt);
    origin_name16 = sqlite3_column_origin_name16(stmt, 0);
    rc_snap1 = sqlite3_snapshot_open(db, "main", &snap1);
    rc_snap2 = sqlite3_snapshot_open(db, "main", &snap2);
    snap_cmp = sqlite3_snapshot_cmp(&snap1, &snap2);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();

    // step 4: Cleanup - finalize statement, close database, silence unused warnings
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    (void)origin_name16;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)snap_cmp;
    (void)sys_err;
    (void)mem_used;

    // API sequence test completed successfully
    return 66;
}