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
//<ID> 186
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
    const char *sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER); INSERT INTO t(id) VALUES(1); SELECT id FROM t;";
    const char *origin_name = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_complete = sqlite3_complete(sql);
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 2: Configure - obtain a snapshot for the main schema
    int rc_snapshot = sqlite3_snapshot_open(db, "main", &snapshot);

    // step 3: Operate - execute statement and inspect column origin name
    int rc_step = sqlite3_step(stmt);
    const void *origin_ptr = sqlite3_column_origin_name16(stmt, 0);
    origin_name = origin_ptr ? (const char *)origin_ptr : "NULL";

    // step 4: Validate / Cleanup - finalize statement and close database
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_complete;
    (void)rc_prepare;
    (void)rc_snapshot;
    (void)rc_step;
    (void)origin_name;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}