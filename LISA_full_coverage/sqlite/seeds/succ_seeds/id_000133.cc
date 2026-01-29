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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *expanded_sql = NULL;
    const char *sql_setup = "CREATE TABLE t(val TEXT); INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t WHERE rowid=1;";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_exec = sqlite3_exec(db, sql_setup, NULL, NULL, NULL);
    int rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    expanded_sql = sqlite3_expanded_sql(stmt);
    int rc_step = sqlite3_step(stmt);
    int col_bytes = sqlite3_column_bytes(stmt, 0);
    int rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(expanded_sql);
    int errcode = sqlite3_errcode(db);
    const char *errstr = sqlite3_errstr(errcode);
    int changes = sqlite3_changes(db);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)col_bytes;
    (void)rc_finalize;
    (void)errcode;
    (void)errstr;
    (void)changes;
    (void)rc_close;
    return 66;
}