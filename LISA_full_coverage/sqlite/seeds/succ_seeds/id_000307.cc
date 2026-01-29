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
//<ID> 307
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
    const void *sql16 = (const void *)u"CREATE TABLE demo(id INTEGER);";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_ckpt = 0;
    int rc_nochange = 0;
    int rc_threadsafe = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;

    // step 2: Initialize / Configure - open in-memory DB
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_threadsafe = sqlite3_threadsafe();

    // step 3: Operate / Validate - prepare a UTF-16 statement and request WAL checkpoint
    rc_prepare = sqlite3_prepare16_v3(db, sql16, -1, 0u, &stmt, &pzTail);
    rc_ckpt = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    rc_nochange = sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_ckpt;
    (void)rc_nochange;
    (void)rc_threadsafe;
    (void)rc_close;
    (void)pzTail;
    (void)nLog;
    (void)nCkpt;
    (void)ctx;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}