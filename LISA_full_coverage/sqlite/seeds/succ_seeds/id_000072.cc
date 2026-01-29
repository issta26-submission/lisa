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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_mutex *mutex = NULL;
    void *prev_trace_ctx = NULL;
    sqlite3_int64 hw_before = 0;
    sqlite3_int64 hw_after = 0;
    int rc_wal = 0;
    int rc_vtab = 0;

    sqlite3_initialize();

    // step 2: Configure
    hw_before = sqlite3_memory_highwater(0);
    prev_trace_ctx = sqlite3_trace(db, NULL, (void *)&hw_before);

    // step 3: Operate / Validate
    hw_after = sqlite3_memory_highwater(1);
    rc_wal = sqlite3_wal_autocheckpoint(db, (int)(hw_after & 0x7fffffff));
    rc_vtab = sqlite3_vtab_nochange(ctx);
    sqlite3_mutex_leave(mutex);

    // step 4: Cleanup
    sqlite3_trace(db, NULL, prev_trace_ctx);
    sqlite3_shutdown();

    (void)db;
    (void)ctx;
    (void)mutex;
    (void)hw_before;
    (void)hw_after;
    (void)rc_wal;
    (void)rc_vtab;
    (void)prev_trace_ctx;

    // API sequence test completed successfully
    return 66;
}