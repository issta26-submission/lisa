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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static int trace_v2_cb(unsigned int uMask, void *pCtx, void *pArg1, void *pArg2) {
    (void)uMask;
    (void)pCtx;
    (void)pArg1;
    (void)pArg2;
    return 0;
}

int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_mutex *m = NULL;
    int rc_os = 0;
    int rc_open = 0;
    int rc_trace = 0;
    int rc_wal = 0;
    int rc_dbstat = 0;
    int cur = 0;
    int hi = 0;
    int held = 0;
    int rc_release = 0;
    const char *ver = NULL;
    const char *err = NULL;

    rc_os = sqlite3_os_init();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_trace = sqlite3_trace_v2(db, 0xFFFFFFFFu, trace_v2_cb, (void *)0x1);
    rc_wal = sqlite3_wal_checkpoint(db, "main");
    rc_dbstat = sqlite3_db_status(db, 0, &cur, &hi, 0);
    m = sqlite3_mutex_alloc(0);
    held = sqlite3_mutex_held(m);
    rc_release = sqlite3_db_release_memory(db);
    ver = sqlite3_libversion();
    err = sqlite3_errstr(rc_open);

    (void)rc_os;
    (void)rc_open;
    (void)rc_trace;
    (void)rc_wal;
    (void)rc_dbstat;
    (void)cur;
    (void)hi;
    (void)m;
    (void)held;
    (void)rc_release;
    (void)ver;
    (void)err;

    sqlite3_close_v2(db);
    sqlite3_shutdown();

    return 66;
}