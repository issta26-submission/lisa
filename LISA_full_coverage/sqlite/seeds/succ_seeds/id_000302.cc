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
//<ID> 302
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
    const char16_t *sql16 = u"CREATE TABLE demo(id INTEGER);";
    const void *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare16 = 0;
    int rc_chkpt = 0;
    int rc_vtab_nochange = 0;
    int rc_threadsafe = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;

    // step 2: Initialize (open an in-memory database)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate and Validate (prepare UTF-16 SQL, checkpoint WAL, check vtab and threadsafety)
    rc_prepare16 = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0, &stmt, &pzTail);
    rc_chkpt = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    rc_vtab_nochange = sqlite3_vtab_nochange(ctx);
    rc_threadsafe = sqlite3_threadsafe();

    // step 4: Cleanup (finalize statement and close DB)
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare16;
    (void)rc_chkpt;
    (void)rc_vtab_nochange;
    (void)rc_threadsafe;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)nLog;
    (void)nCkpt;
    (void)ctx;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}