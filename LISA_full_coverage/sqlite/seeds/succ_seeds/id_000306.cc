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
//<ID> 306
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
    sqlite3_context *ctx = NULL;
    const void *pzTail = NULL;
    const char16_t *sql16 = u"CREATE TABLE fuzz(a INTEGER);";
    int rc_open = 0;
    int rc_prepare16 = 0;
    int rc_vtab_nochange = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;
    int threadsafe = 0;

    // step 2: Initialize - open an in-memory database and query thread-safety
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe = sqlite3_threadsafe();

    // step 3: Operate and Validate - prepare a UTF-16 statement, perform WAL checkpoint, and call vtab nochange
    rc_prepare16 = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0u, &stmt, &pzTail);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    rc_vtab_nochange = sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)threadsafe;
    (void)rc_prepare16;
    (void)rc_wal;
    (void)rc_vtab_nochange;
    (void)nLog;
    (void)nCkpt;
    (void)pzTail;
    (void)rc_close;
    (void)ctx;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}