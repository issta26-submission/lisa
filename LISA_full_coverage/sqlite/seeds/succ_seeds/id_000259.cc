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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    const void *errmsg16 = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_close = 0;

    // step 2: Setup - initialize SQLite and open an in-memory database (UTF-16)
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16((const void*)L":memory:", &db);

    // step 3: Core operations - create table, insert a row, select the row using sqlite3_prepare16 and sqlite3_step
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare16(db, (const void*)L"INSERT INTO t(x) VALUES(123);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare16(db, (const void*)L"SELECT x FROM t;", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);

    // step 4: Validate and Cleanup - retrieve UTF-16 error message (if any), log an informational message, finalize and close
    errmsg16 = sqlite3_errmsg16(db);
    sqlite3_log(rc_step, "sqlite3_prepare16/sqlite3_step sequence executed");
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_close;
    (void)pzTail;
    (void)errmsg16;

    // API sequence test completed successfully
    return 66;
}