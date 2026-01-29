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
//<ID> 34
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
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const void *origin16 = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_snapshot1 = 0;
    int rc_snapshot2 = 0;
    int cmp = 0;
    int sys_errno = 0;
    sqlite3_int64 mem_used = 0;

    // step 2: Setup - open in-memory database and create a table
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)u"CREATE TABLE t(x INTEGER);", -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate - insert a row, select it, inspect origin name, snapshots, and stats
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)u"INSERT INTO t(x) VALUES(42);", -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)u"SELECT x AS alias_x FROM t;", -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    origin16 = sqlite3_column_origin_name16(stmt, 0);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_snapshot1 = sqlite3_snapshot_open(db, "main", &snap1);
    rc_snapshot2 = sqlite3_snapshot_open(db, "main", &snap2);
    cmp = sqlite3_snapshot_cmp(&snap1, &snap2);
    sys_errno = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();

    // step 4: Cleanup - close database and silence unused warnings
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_snapshot1;
    (void)rc_snapshot2;
    (void)cmp;
    (void)origin16;
    (void)sys_errno;
    (void)mem_used;

    // API sequence test completed successfully
    return 66;
}