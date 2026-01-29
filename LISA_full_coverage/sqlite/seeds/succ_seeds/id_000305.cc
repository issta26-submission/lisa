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
//<ID> 305
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
    const void *pzTail16 = NULL;
    const wchar_t sql16[] = L"CREATE TABLE demo(id INTEGER);";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_wal = 0;
    int rc_nochange = 0;
    int rc_threadsafe = 0;
    int rc_close = 0;
    int nLog = 0;
    int nCkpt = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_threadsafe = sqlite3_threadsafe();

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0, &stmt, &pzTail16);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    rc_nochange = sqlite3_vtab_nochange(ctx);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_wal;
    (void)rc_nochange;
    (void)rc_threadsafe;
    (void)rc_close;
    (void)nLog;
    (void)nCkpt;
    (void)pzTail16;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}