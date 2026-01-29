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
//<ID> 250
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
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize and open the in-memory database
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16((const void*)L":memory:", &db);

    // step 3: Prepare a UTF-16 SQL statement and execute it
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    errmsg16 = sqlite3_errmsg16(db);
    sqlite3_log(rc_step, (const char*)errmsg16);

    // step 4: Finalize statement and close database (validate & cleanup)
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)errmsg16;

    // API sequence test completed successfully
    return 66;
}