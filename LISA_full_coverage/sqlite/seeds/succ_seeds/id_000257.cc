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
//<ID> 257
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
    const void *sql16 = (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);";
    const void *errmsg16 = NULL;
    int rc_init = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;

    // step 2: Initialize / Configure
    rc_init = sqlite3_initialize();
    sqlite3_log(rc_init, "sqlite3_initialize returned");

    // step 3: Operate - prepare and step the (possibly NULL) statement, capture error message
    rc_prep = sqlite3_prepare16(db, sql16, -1, &stmt, &pzTail);
    sqlite3_log(rc_prep, "sqlite3_prepare16 returned");
    errmsg16 = sqlite3_errmsg16(db);
    (void)errmsg16;
    rc_step = sqlite3_step(stmt);
    sqlite3_log(rc_step, "sqlite3_step returned");

    // step 4: Validate / Cleanup
    rc_fin = sqlite3_finalize(stmt);
    sqlite3_log(rc_fin, "sqlite3_finalize returned");

    // API sequence test completed successfully
    (void)db;
    (void)pzTail;
    return 66;
}