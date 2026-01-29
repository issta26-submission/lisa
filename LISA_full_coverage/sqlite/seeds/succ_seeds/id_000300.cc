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
//<ID> 300
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
    const char16_t *sql16 = u"CREATE TABLE IF NOT EXISTS t(x INTEGER);";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_wal = 0;
    int rc_vtab = 0;
    int rc_close = 0;
    int threadsafe = 0;
    int pnLog = 0;
    int pnCkpt = 0;
    sqlite3_context *ctx = NULL;

    // step 2: Setup - initialize and open an in-memory database, query threadsafety
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe = sqlite3_threadsafe();

    // step 3: Core operations - prepare a UTF-16 statement, run WAL checkpoint, call vtab nochange
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0u, &stmt, &pzTail);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &pnLog, &pnCkpt);
    rc_vtab = sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)threadsafe;
    (void)rc_prepare;
    (void)rc_wal;
    (void)pnLog;
    (void)pnCkpt;
    (void)rc_vtab;
    (void)pzTail;
    (void)ctx;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}