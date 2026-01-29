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
//<ID> 301
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
    char const *walDbName = "main";
    char16_t sql16[] = u"CREATE TABLE demo(id INTEGER);";
    int rc_open_v2 = 0;
    int rc_prepare16 = 0;
    int rc_checkpoint = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;
    int threadsafe = 0;
    int vtab_nochange_result = 0;

    // step 2: Setup (query threadsafety and open an in-memory DB)
    threadsafe = sqlite3_threadsafe();
    rc_open_v2 = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations (prepare a UTF-16 statement, checkpoint WAL, mark vtab no-change)
    rc_prepare16 = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0, &stmt, &pzTail);
    rc_checkpoint = sqlite3_wal_checkpoint_v2(db, walDbName, 0, &nLog, &nCkpt);
    vtab_nochange_result = sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup (finalize statement and close DB)
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)threadsafe;
    (void)rc_open_v2;
    (void)rc_prepare16;
    (void)rc_checkpoint;
    (void)nLog;
    (void)nCkpt;
    (void)vtab_nochange_result;
    (void)rc_close;
    (void)pzTail;
    (void)walDbName;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}