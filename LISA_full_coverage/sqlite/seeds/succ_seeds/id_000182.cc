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
//<ID> 182
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
    const char *sql_create = "CREATE TABLE t(x INTEGER);";
    const char *sql_insert = "INSERT INTO t(x) VALUES(123);";
    const char *sql_select = "SELECT x FROM t;";
    const char *pzTail1 = NULL;
    const char *pzTail2 = NULL;
    int rc_complete_create = sqlite3_complete(sql_create);
    int rc_open = sqlite3_open(":memory:", &db);
    memset(&snapshot, 0, sizeof(snapshot));

    // step 2: Configure / Create and populate table
    int rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt, &pzTail1);
    int rc_step_create = sqlite3_step(stmt);
    int rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, &pzTail1);
    int rc_step_insert = sqlite3_step(stmt);

    // step 3: Operate / Snapshot and query
    int rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &pzTail2);
    int rc_snapshot = sqlite3_snapshot_open(db, "main", &snapshot);
    int rc_step_select = sqlite3_step(stmt);
    const void *origin_name16 = sqlite3_column_origin_name16(stmt, 0);

    // step 4: Validate / Cleanup
    int rc_complete_select = sqlite3_complete(sql_select);
    int rc_close = sqlite3_close(db);

    (void)rc_complete_create;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_prep_select;
    (void)rc_snapshot;
    (void)rc_step_select;
    (void)origin_name16;
    (void)rc_complete_select;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}