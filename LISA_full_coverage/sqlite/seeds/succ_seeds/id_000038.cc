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
//<ID> 38
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
    struct sqlite3_snapshot snap1;
    struct sqlite3_snapshot snap2;
    const void *origin_name16 = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_snap1 = 0;
    int rc_snap2 = 0;
    int cmp = 0;
    int sysErr = 0;
    sqlite3_int64 memUsed = 0;

    // step 2: Setup (initialize database and query global state)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    memUsed = sqlite3_memory_used();
    sysErr = sqlite3_system_errno(db);

    // step 3: Core operations (create table, insert, select, inspect column origin, take snapshots and compare)
    rc_prepare = sqlite3_prepare16(db, u"CREATE TABLE t(a);", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare16(db, u"INSERT INTO t(a) VALUES(42);", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare16(db, u"SELECT a FROM t;", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    origin_name16 = sqlite3_column_origin_name16(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_snap1 = sqlite3_snapshot_open(db, "main", &snap1);
    rc_snap2 = sqlite3_snapshot_open(db, "main", &snap2);
    cmp = sqlite3_snapshot_cmp(&snap1, &snap2);

    // step 4: Cleanup
    sqlite3_close_v2(db);

    // API sequence test completed successfully
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)cmp;
    (void)origin_name16;
    (void)sysErr;
    (void)memUsed;
    return 66;
}