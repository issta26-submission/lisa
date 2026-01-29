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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_int64 extracted = 0;
    const char *errstr = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);
    rc_prepare = sqlite3_prepare(db, "CREATE TABLE IF NOT EXISTS t(x);", -1, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_prepare = sqlite3_prepare(db, "INSERT INTO t(x) VALUES(42);", -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_prepare = sqlite3_prepare(db, "SELECT x FROM t LIMIT 1;", -1, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    extracted = sqlite3_column_int64(stmt_select, 0);
    errstr = sqlite3_errstr(rc_step);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)db;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)extracted;
    (void)errstr;

    return 66;
}