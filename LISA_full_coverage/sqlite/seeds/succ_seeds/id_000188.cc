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
//<ID> 188
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
    sqlite3_snapshot snapshot;
    const char *sql = "SELECT 1 AS val;";
    const char *pzTail = NULL;
    const void *origin_name = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_snap = 0;
    int rc_close = 0;
    int rc_complete = 0;
    memset(&snapshot, 0, sizeof(snapshot));

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - validate SQL, prepare and step the statement, inspect column origin, open snapshot
    rc_complete = sqlite3_complete(sql);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    origin_name = sqlite3_column_origin_name16(stmt, 0);
    rc_snap = sqlite3_snapshot_open(db, "main", &snapshot);

    // step 4: Validate / Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_snap;
    (void)rc_close;
    (void)rc_complete;
    (void)pzTail;
    (void)origin_name;
    (void)snapshot;

    // API sequence test completed successfully
    return 66;
}