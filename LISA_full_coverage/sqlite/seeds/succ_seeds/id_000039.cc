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
//<ID> 39
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
    struct sqlite3_snapshot snap1;
    struct sqlite3_snapshot snap2;
    const void *col_origin = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int cmp_result = 0;
    int sys_err = 0;
    sqlite3_int64 mem_used = 0;

    // step 2: Setup - initialize snapshots and open an in-memory database
    memset(&snap1, 0, sizeof(snap1));
    memset(&snap2, 0, sizeof(snap2));
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations - prepare a UTF-16 SQL, step it, query column origin, compare snapshots, query system errno and memory used
    rc_prep = sqlite3_prepare16(db, (const void*)u"SELECT 1 AS col;", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_origin = sqlite3_column_origin_name16(stmt, 0);
    cmp_result = sqlite3_snapshot_cmp(&snap1, &snap2);
    sys_err = sqlite3_system_errno(db);
    mem_used = sqlite3_memory_used();

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)col_origin;
    (void)cmp_result;
    (void)sys_err;
    (void)mem_used;

    // API sequence test completed successfully
    return 66;
}