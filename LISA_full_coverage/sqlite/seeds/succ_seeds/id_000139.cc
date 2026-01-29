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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open16((const void *)":memory:", &db);
    char **results = NULL;
    int nrow = 0;
    int ncol = 0;
    char *errmsg = NULL;

    // step 2: Configure - create table and insert using sqlite3_get_table
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    int rc_get = sqlite3_get_table(db, sql_setup, &results, &nrow, &ncol, &errmsg);
    sqlite3_int64 lastid = sqlite3_last_insert_rowid(db);

    // step 3: Operate / Validate - prepare/select, get expanded SQL, snapshots and compare, get error code
    const char *sql_select = "SELECT val FROM t LIMIT 1;";
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    int rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0u, &stmt, &pzTail);
    char *expanded = sqlite3_expanded_sql(stmt);
    const char *orig_sql = sqlite3_sql(stmt);
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    int rc_snap1 = sqlite3_snapshot_get(db, "main", &snap1);
    int rc_snap2 = sqlite3_snapshot_get(db, "main", &snap2);
    long snap_cmp = (long)(snap1) - (long)(snap2);
    int errcode = sqlite3_errcode(db);

    // step 4: Cleanup
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);

    // silence unused variable warnings
    (void)rc_open;
    (void)results;
    (void)nrow;
    (void)ncol;
    (void)errmsg;
    (void)rc_get;
    (void)lastid;
    (void)rc_prep;
    (void)expanded;
    (void)orig_sql;
    (void)rc_snap1;
    (void)rc_snap2;
    (void)snap_cmp;
    (void)errcode;
    (void)rc_finalize;
    (void)rc_close;

    return 66;
}