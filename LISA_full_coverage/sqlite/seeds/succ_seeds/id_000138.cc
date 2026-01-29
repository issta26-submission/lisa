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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    const char *pzTail = NULL;
    const char *setup_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('alpha');";
    const char *select_sql = "SELECT val FROM t WHERE id=1;";
    char *expanded_sql = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);
    int rc_exec = sqlite3_exec(db, setup_sql, NULL, NULL, NULL);
    int rc_prep = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &pzTail);
    expanded_sql = sqlite3_expanded_sql(stmt);
    int rc_snap1 = sqlite3_snapshot_get(db, "main", &snap1);
    int rc_snap2 = sqlite3_snapshot_get(db, "main", &snap2);
    int cmp = sqlite3_stricmp("alpha", "ALPHA");
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)expanded_sql;
    (void)pzTail;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)cmp;
    (void)db;
    (void)stmt;
    (void)snap1;
    (void)snap2;
    return 66;
}