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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_create_insert = "CREATE TABLE t(a); INSERT INTO t VALUES(42);";
    const char *sql_select = "SELECT a FROM t;";
    void *prev_trace = NULL;
    void *trace_context = (void *)0x1;
    sqlite3_int64 mem_high_before = 0;
    sqlite3_int64 mem_high_after = 0;
    sqlite3_int64 col_val = 0;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_wal = 0;
    int rc_vtab = 0;

    // step 2: Configure / Setup
    mem_high_before = sqlite3_memory_highwater(0);
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create_insert, 0, 0, 0);
    prev_trace = sqlite3_trace(db, NULL, trace_context);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_val = sqlite3_column_int64(stmt, 0);
    rc_vtab = sqlite3_vtab_nochange((sqlite3_context *)NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);
    sqlite3_mutex_leave((sqlite3_mutex *)NULL);
    mem_high_after = sqlite3_memory_highwater(1);

    // step 4: Cleanup
    sqlite3_trace(db, (void (*)(void *, const char *))prev_trace, NULL);
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);

    (void)mem_high_before;
    (void)mem_high_after;
    (void)col_val;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_wal;
    (void)rc_vtab;
    (void)prev_trace;
    (void)trace_context;
    // API sequence test completed successfully
    return 66;
}