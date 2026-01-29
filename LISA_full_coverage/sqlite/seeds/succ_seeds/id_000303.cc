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
//<ID> 303
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
    int rc_init = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;
    int threadsafe = 0;
    int rc_vtab_nochange = 0;

    // step 2: Setup (initialize and open an in-memory database)
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate and Validate (prepare a UTF-16 statement, checkpoint WAL, query threadsafety, vtab nochange)
    rc_prepare = sqlite3_prepare16_v3(db, L"CREATE TABLE demo(id INTEGER);", -1, 0u, &stmt, &pzTail);
    rc_finalize = sqlite3_finalize(stmt);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    threadsafe = sqlite3_threadsafe();
    rc_vtab_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);

    // step 4: Cleanup (close database and coerce unused values)
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)rc_wal;
    (void)rc_close;
    (void)nLog;
    (void)nCkpt;
    (void)threadsafe;
    (void)rc_vtab_nochange;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}