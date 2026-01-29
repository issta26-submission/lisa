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
//<ID> 30
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
    const void *origin_name16 = NULL;
    int rc_open = 0;
    int rc_p1 = 0;
    int rc_step1 = 0;
    int rc_f1 = 0;
    int rc_p2 = 0;
    int rc_f2 = 0;
    int rc_close = 0;
    int sys_err = 0;
    int snapcmp = 0;
    sqlite3_int64 mem_used = 0;

    // step 2: Setup - open database and create a table
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_p1 = sqlite3_prepare16(db, (const void *)u"CREATE TABLE t(id INTEGER);", -1, &stmt, NULL);
    rc_step1 = sqlite3_step(stmt);
    rc_f1 = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate - prepare a SELECT, inspect column origin, query system errno and memory, use snapshots
    rc_p2 = sqlite3_prepare16(db, (const void *)u"SELECT id FROM t;", -1, &stmt, NULL);
    origin_name16 = sqlite3_column_origin_name16(stmt, 0);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();
    memset(&snap1, 0, sizeof(snap1));
    memset(&snap2, 0, sizeof(snap2));
    snapcmp = sqlite3_snapshot_cmp(&snap1, &snap2);

    // step 4: Cleanup - finalize statement and close database
    rc_f2 = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)origin_name16;
    (void)rc_open;
    (void)rc_p1;
    (void)rc_step1;
    (void)rc_f1;
    (void)rc_p2;
    (void)rc_f2;
    (void)rc_close;
    (void)sys_err;
    (void)mem_used;
    (void)snapcmp;
    (void)snap1;
    (void)snap2;

    // API sequence test completed successfully
    return 66;
}