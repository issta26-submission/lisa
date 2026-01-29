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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_mutex *mx = NULL;
    void *trace_prev = NULL;
    const char *sql_create = "CREATE TABLE t(a); INSERT INTO t VALUES(1);";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_wal = 0;
    sqlite3_int64 mem_before = 0;
    sqlite3_int64 mem_after = 0;

    // step 2: Setup / configure
    sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);
    trace_prev = sqlite3_trace(db, NULL, NULL);
    mem_before = sqlite3_memory_highwater(0);

    // step 3: Operate / validate
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 1);
    mem_after = sqlite3_memory_highwater(0);
    (void)sqlite3_vtab_nochange(ctx);
    sqlite3_mutex_leave(mx);

    // step 4: Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    (void)trace_prev;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_wal;
    (void)mem_before;
    (void)mem_after;
    // API sequence test completed successfully
    return 66;
}