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
//<ID> 252
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
    int rc_prep1 = 0;
    int rc_step1 = 0;
    int rc_prep2 = 0;
    int rc_step2 = 0;
    int rc_close = 0;

    // step 2: Initialize library and open an in-memory database
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations — create a table and insert a row using UTF-16 SQL via sqlite3_prepare16 and sqlite3_step
    rc_prep1 = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);
    rc_step1 = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep2 = sqlite3_prepare16(db, (const void*)L"INSERT INTO t(x) VALUES(42);", -1, &stmt, &pzTail);
    rc_step2 = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validation and cleanup — retrieve UTF-16 error message, log a status, and close the database
    errmsg16 = sqlite3_errmsg16(db);
    sqlite3_log(rc_step2, "SQLite UTF-16 prepare/step operations completed");
    rc_close = sqlite3_close(db);
    (void)errmsg16;
    (void)rc_init;
    (void)rc_open;
    (void)rc_prep1;
    (void)rc_step1;
    (void)rc_prep2;
    (void)rc_step2;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}