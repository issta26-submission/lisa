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
//<ID> 449
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
    const char *sql_explain = "EXPLAIN SELECT 123;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_wal = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int is_explain = 0;
    sqlite3_int64 prev_soft_limit = 0;
    sqlite3_int64 new_soft_limit = (sqlite3_int64)1024 * 1024;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_soft_limit = sqlite3_soft_heap_limit64(new_soft_limit);
    rc_wal = sqlite3_wal_autocheckpoint(db, 1000);

    // step 3: Operate
    rc_prepare = sqlite3_prepare_v3(db, sql_explain, -1, 0u, &stmt, NULL);
    is_explain = sqlite3_stmt_isexplain(stmt);

    // step 4: Cleanup / Validate
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);
    stmt = NULL;
    db = NULL;

    (void)rc_open;
    (void)prev_soft_limit;
    (void)new_soft_limit;
    (void)rc_wal;
    (void)rc_prepare;
    (void)is_explain;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql_explain;

    // API sequence test completed successfully
    return 66;
}