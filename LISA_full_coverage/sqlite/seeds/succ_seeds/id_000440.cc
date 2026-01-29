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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *sql_insert = "INSERT INTO t(val) VALUES(123);";
    const char *sql_select = "SELECT val FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_wal = 0;
    int rc_isexplain = 0;
    sqlite3_int64 prev_heap = 0;
    sqlite3_int64 new_heap = (sqlite3_int64)16 * 1024 * 1024; /* 16MB */
    sqlite3_int64 fetched_val = 0;

    // step 2: Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);
    prev_heap = sqlite3_soft_heap_limit64(new_heap);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v3(db, sql_create, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v3(db, sql_insert, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v3(db, sql_select, -1, 0u, &stmt, &pzTail);
    rc_isexplain = sqlite3_stmt_isexplain(stmt);
    rc_step = sqlite3_step(stmt);
    fetched_val = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup
    sqlite3_soft_heap_limit64(prev_heap);
    sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_wal;
    (void)rc_isexplain;
    (void)prev_heap;
    (void)new_heap;
    (void)fetched_val;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}