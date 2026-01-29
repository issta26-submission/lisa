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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE t(x);";
    const char *tail = NULL;
    const char *errmsg = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_shutdown = 0;
    int stmt_busy = 0;
    sqlite3_int64 changes = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    errmsg = sqlite3_errmsg(db);

    // step 3: Operate / Validate
    rc_exec = sqlite3_exec(db, sql, NULL, NULL, NULL);
    rc_prepare = sqlite3_prepare16(db, (const void *)sql, -1, &stmt, (const void **)&tail);
    stmt_busy = sqlite3_stmt_busy(stmt);
    sqlite3_stmt_scanstatus_reset(stmt);
    changes = sqlite3_changes64(db);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    rc_shutdown = sqlite3_shutdown();

    (void)errmsg;
    (void)tail;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)rc_shutdown;
    (void)stmt_busy;
    (void)changes;
    (void)sql;

    // API sequence test completed successfully (preserve original return value)
    return 66;
}