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
//<ID> 187
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
    const char *create_sql = "CREATE TABLE t(id INTEGER, val TEXT);";
    const char *select_sql = "SELECT id, val FROM t;";
    const void *col_origin = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_complete = 0;
    int rc_snap = 0;
    int rc_close = 0;
    memset(&snapshot, 0, sizeof(snapshot));

    // step 2: Open database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate — validate SQL completeness, create table, prepare select, inspect column origin, open snapshot
    rc_complete = sqlite3_complete(create_sql);
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    col_origin = sqlite3_column_origin_name16(stmt, 0);
    rc_snap = sqlite3_snapshot_open(db, "main", &snapshot);

    // step 4: Validate and cleanup
    rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_complete;
    (void)col_origin;
    (void)rc_snap;
    (void)rc_close;
    (void)stmt;
    (void)snapshot;

    // API sequence test completed successfully
    return 66;
}