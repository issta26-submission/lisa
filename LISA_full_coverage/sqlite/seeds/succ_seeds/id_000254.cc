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
//<ID> 254
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
    const void *pzTail = NULL;
    const void *errmsg16 = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Configure - open an in-memory database and prepare a UTF-16 SQL statement
    rc_open = sqlite3_open16((const void*)L":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);

    // step 3: Operate / Validate - execute the prepared statement and obtain any UTF-16 error message
    rc_step = sqlite3_step(stmt);
    errmsg16 = sqlite3_errmsg16(db);
    sqlite3_log(rc_step, "sqlite3_step executed");

    // step 4: Cleanup - finalize statement and close database
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)pzTail;
    (void)errmsg16;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;

    return 66;
    // API sequence test completed successfully
}