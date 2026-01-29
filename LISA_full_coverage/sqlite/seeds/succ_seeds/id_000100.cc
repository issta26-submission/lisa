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
//<ID> 100
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
    const void *errmsg16 = NULL;
    int rc_open = 0;
    int rc_coll = 0;
    int rc_wal = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;
    int rc_close = 0;
    int is_explain = 0;
    int vtab_nochange_rc = 0;

    // step 2: Configure
    rc_open = sqlite3_open16(L":memory:", &db);
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16(db, L"SELECT 1;", -1, &stmt, NULL);
    is_explain = sqlite3_stmt_isexplain(stmt);
    rc_step = sqlite3_step(stmt);
    vtab_nochange_rc = sqlite3_vtab_nochange((sqlite3_context *)NULL);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup
    rc_fin = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_coll;
    (void)rc_wal;
    (void)rc_prep;
    (void)is_explain;
    (void)rc_step;
    (void)vtab_nochange_rc;
    (void)errmsg16;
    (void)rc_fin;
    (void)rc_close;
    (void)db;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}