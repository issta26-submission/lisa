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
//<ID> 258
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
    const void *errmsg = NULL;
    int rc_init = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;
    int rc_close = 0;

    // step 2: Initialize library
    rc_init = sqlite3_initialize();

    // step 3: Configure and operate - prepare a UTF-16 statement and execute a step
    rc_prep = sqlite3_prepare16(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    errmsg = sqlite3_errmsg16(db);
    sqlite3_log(rc_prep, "sqlite3_prepare16 returned");
    sqlite3_log(rc_step, (const char*)errmsg);

    // step 4: Validate and cleanup
    rc_fin = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_fin;
    (void)rc_close;
    (void)pzTail;
    (void)errmsg;
    (void)db;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}