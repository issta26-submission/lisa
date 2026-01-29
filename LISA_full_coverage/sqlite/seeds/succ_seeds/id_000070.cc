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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 123;";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int col0 = 0;
    sqlite3_int64 mw_before = 0;
    sqlite3_int64 mw_after = 0;
    void *trace_ctx = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_mutex *mutex = NULL;

    // step 2: Setup
    sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, &pzTail);

    // step 3: Core operations / Validate
    rc_step = sqlite3_step(stmt);
    col0 = sqlite3_column_int(stmt, 0);
    sqlite3_set_last_insert_rowid(db, (sqlite3_int64)col0);
    mw_before = sqlite3_memory_highwater(0);
    trace_ctx = sqlite3_trace(db, NULL, NULL);
    sqlite3_wal_autocheckpoint(db, 100);
    sqlite3_vtab_nochange(ctx);
    sqlite3_mutex_leave(mutex);
    mw_after = sqlite3_memory_highwater(1);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    sqlite3_shutdown();
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)col0;
    (void)mw_before;
    (void)mw_after;
    (void)trace_ctx;
    (void)pzTail;
    (void)ctx;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}