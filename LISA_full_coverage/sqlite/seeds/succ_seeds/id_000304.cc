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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_context *ctx = NULL;
    const void *pzTail = NULL;
    const void *sql16 = (const void*)u"CREATE TABLE demo(id INTEGER);";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_wal = 0;
    int rc_nochange = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int pnLog = 0;
    int pnCkpt = 0;
    int ts = 0;

    // Step 2: Configure & Prepare
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare16_v3(db, sql16, -1, 0u, &stmt, &pzTail);

    // Step 3: Operate & Validate
    ts = sqlite3_threadsafe();
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &pnLog, &pnCkpt);
    rc_nochange = sqlite3_vtab_nochange(ctx);

    // Step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)ts;
    (void)rc_wal;
    (void)pnLog;
    (void)pnCkpt;
    (void)rc_nochange;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)sql16;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}