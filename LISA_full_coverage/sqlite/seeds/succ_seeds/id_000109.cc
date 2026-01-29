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
//<ID> 109
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
    const void *pzTail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_collation = 0;
    int rc_is_explain = 0;
    int rc_vtab_nochange = 0;
    int rc_wal = 0;
    int col_count = 0;

    // step 2: Configure
    rc_open = sqlite3_open16((const void *)L":memory:", &db);
    rc_collation = sqlite3_create_collation_v2(db, "my_coll", 0, NULL,
        (int (*)(void *, int, const void *, int, const void *))NULL,
        (void (*)(void *))NULL);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16(db, (const void *)L"EXPLAIN SELECT 1;", -1, &stmt, &pzTail);
    rc_is_explain = sqlite3_stmt_isexplain(stmt);
    rc_step = sqlite3_step(stmt);
    col_count = sqlite3_column_count(stmt);
    rc_vtab_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_collation;
    (void)rc_is_explain;
    (void)rc_vtab_nochange;
    (void)rc_wal;
    (void)col_count;
    (void)pzTail;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}