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
//<ID> 256
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

    // step 2: Setup - initialize library and open an in-memory database
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16((const void*)L":memory:", &db);

    // step 3: Core operations - prepare a UTF-16 statement, execute it, retrieve UTF-16 error text and log
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    errmsg16 = sqlite3_errmsg16(db);
    sqlite3_log(rc_step, "sqlite3_step executed");

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)errmsg16;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}