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
//<ID> 309
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
    const void *sql16 = (const void *)L"CREATE TABLE demo(id INTEGER);";
    sqlite3_context *ctx = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_checkpoint = 0;
    int rc_close = 0;
    int threadsafe = 0;
    int nLog = 0;
    int nCkpt = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations - prepare UTF-16 statement, query threadsafety, checkpoint WAL, mark vtab nochange
    rc_prepare = sqlite3_prepare16_v3(db, sql16, -1, 0u, &stmt, &pzTail);
    threadsafe = sqlite3_threadsafe();
    rc_checkpoint = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_checkpoint;
    (void)rc_close;
    (void)threadsafe;
    (void)nLog;
    (void)nCkpt;
    (void)pzTail;
    (void)ctx;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}