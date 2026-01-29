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
//<ID> 253
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
    const char *errmsg_as_char = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;
    int rc_close = 0;

    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16((const void*)L":memory:", &db);

    // step 2: Configure - prepare a UTF-16 statement
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);

    // step 3: Operate and validate - execute the statement, fetch any UTF-16 error message, and log it
    rc_step = sqlite3_step(stmt);
    errmsg16 = sqlite3_errmsg16(db);
    errmsg_as_char = (const char *)errmsg16;
    sqlite3_log(rc_step, errmsg_as_char);

    // step 4: Cleanup - finalize statement and close database
    rc_fin = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_fin;
    (void)rc_close;
    (void)pzTail;
    (void)errmsg16;
    (void)errmsg_as_char;

    // API sequence test completed successfully
    return 66;
}