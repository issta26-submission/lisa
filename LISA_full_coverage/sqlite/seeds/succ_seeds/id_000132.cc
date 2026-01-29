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
//<ID> 132
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
    sqlite3_str *strbuf = NULL;
    char *errmsg = NULL;
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const char *setup_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure - create table using sqlite3_exec and initialize str buffer
    int rc_exec = sqlite3_exec(db, setup_sql, 0, 0, &errmsg);
    strbuf = sqlite3_str_new(db);
    int str_errcode = sqlite3_str_errcode(strbuf);

    // step 3: Operate / Validate - obtain snapshots, prepare and step a statement, read a column
    int rc_snap1 = sqlite3_snapshot_open(db, "main", &snap1);
    int rc_snap2 = sqlite3_snapshot_open(db, "main", &snap2);
    int rc_prep = sqlite3_prepare(db, select_sql, -1, &stmt, 0);
    int rc_step = sqlite3_step(stmt);
    const unsigned char *col0 = sqlite3_column_text(stmt, 0);
    (void)col0;

    // step 4: Cleanup
    int rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_str_reset(strbuf);
    sqlite3_free(strbuf);
    strbuf = NULL;
    sqlite3_free(errmsg);
    errmsg = NULL;
    int rc_close = sqlite3_close(db);
    db = NULL;

    int combined_check = rc_open + rc_exec + str_errcode + rc_snap1 + rc_snap2 + rc_prep + rc_step + rc_finalize + rc_close;
    (void)combined_check;

    return 66;
}