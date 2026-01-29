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
//<ID> 108
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
    int rc_open = sqlite3_open16((const void *)L":memory:", &db);
    int rc_prep = sqlite3_prepare16(db, (const void *)L"EXPLAIN SELECT 1;", -1, &stmt, NULL);

    // step 2: Configure
    int rc_coll = sqlite3_create_collation_v2(db, "example_coll", 1, NULL, NULL, NULL);
    int rc_wal = sqlite3_wal_autocheckpoint(db, 100);

    // step 3: Operate / Validate
    int is_explain = sqlite3_stmt_isexplain(stmt);
    int rc_step = sqlite3_step(stmt);
    double col0 = sqlite3_column_double(stmt, 0);
    int rc_vtab_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);

    // step 4: Cleanup
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)rc_coll;
    (void)rc_wal;
    (void)is_explain;
    (void)rc_step;
    (void)col0;
    (void)rc_vtab_nochange;
    (void)rc_finalize;
    (void)rc_close;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}