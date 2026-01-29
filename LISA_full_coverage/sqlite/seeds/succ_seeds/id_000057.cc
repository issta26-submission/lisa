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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *table_name = NULL;
    char *mstr = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int colcount = 0;

    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(a);", NULL, NULL, NULL);
    rc_exec = sqlite3_exec(db, "INSERT INTO t(a) VALUES(42);", NULL, NULL, NULL);
    rc_prep = sqlite3_prepare_v2(db, "SELECT a FROM t;", -1, &stmt, NULL);
    colcount = sqlite3_column_count(stmt);
    table_name = sqlite3_column_table_name(stmt, 0);
    mstr = sqlite3_mprintf("table: %s", table_name);
    sqlite3_free(mstr);
    sqlite3_finalize(stmt);

    (void)db;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)colcount;
    (void)table_name;

    return 66;
}