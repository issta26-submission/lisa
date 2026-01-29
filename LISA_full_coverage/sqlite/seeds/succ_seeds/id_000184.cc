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
//<ID> 184
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
    sqlite3_snapshot snap;
    const char *sql_create = "CREATE TABLE t(id INTEGER);";
    const char *sql_insert = "INSERT INTO t(id) VALUES(42);";
    const char *sql_select = "SELECT id FROM t;";
    const char *pzTail = NULL;
    const void *col_origin = NULL;
    memset(&snap, 0, sizeof(snap));
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_complete_create = sqlite3_complete(sql_create);
    int rc_complete_insert = sqlite3_complete(sql_insert);
    int rc_complete_select = sqlite3_complete(sql_select);

    // step 2: Operate - create table and insert a row via prepared statements
    int rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, &pzTail);
    int rc_step = sqlite3_step(stmt);
    int rc_prepare_ins = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, &pzTail);
    int rc_step_ins = sqlite3_step(stmt);

    // step 3: Operate - select and inspect column origin (metadata)
    int rc_prepare_sel = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &pzTail);
    int rc_step_sel = sqlite3_step(stmt);
    col_origin = sqlite3_column_origin_name16(stmt, 0);

    // step 4: Validate / Snapshot / Cleanup
    int rc_snap = sqlite3_snapshot_open(db, "main", &snap);
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_complete_create;
    (void)rc_complete_insert;
    (void)rc_complete_select;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_prepare_ins;
    (void)rc_step_ins;
    (void)rc_prepare_sel;
    (void)rc_step_sel;
    (void)col_origin;
    (void)rc_snap;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}