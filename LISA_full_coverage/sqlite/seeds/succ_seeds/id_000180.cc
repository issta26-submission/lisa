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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *select_sql = "SELECT 1 AS col;";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_complete = sqlite3_complete(select_sql);

    // step 2: Configure & Operate
    int rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    int rc_step = sqlite3_step(stmt);

    // step 3: Validate
    const void *origin_name = sqlite3_column_origin_name16(stmt, 0);
    sqlite3_snapshot snapshot;
    int rc_snapshot = sqlite3_snapshot_open(db, "main", &snapshot);

    // step 4: Cleanup
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_complete;
    (void)rc_prepare;
    (void)rc_step;
    (void)origin_name;
    (void)snapshot;
    (void)rc_snapshot;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}