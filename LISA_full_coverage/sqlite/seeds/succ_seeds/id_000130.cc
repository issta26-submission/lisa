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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t WHERE id = 1;";
    int rc_open = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    int rc_exec = sqlite3_exec(db, sql_setup, NULL, NULL, &errmsg);
    sqlite3_free(errmsg);
    int rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    const char *colname = sqlite3_column_name(stmt, 0);
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    int rc_snap1 = sqlite3_snapshot_get(db, "main", &snap1);
    int rc_snap2 = sqlite3_snapshot_get(db, "main", &snap2);
    int snap_cmp = (snap1 == snap2);
    sqlite3_snapshot_free(snap1);
    sqlite3_snapshot_free(snap2);
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)colname;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)snap_cmp;
    (void)rc_finalize;
    (void)rc_close;
    return 66;
}