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
//<ID> 107
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
    char *expanded_sql = NULL;
    const void *errmsg16 = NULL;
    const char16_t *sql_create = u"CREATE TABLE t(a INTEGER);";
    int rc_open16 = sqlite3_open16((const void *)u":memory:", &db);
    int rc_create_coll = 0;
    int rc_wal_autocp = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int is_explain = 0;
    int vtab_nochange_rc = 0;

    // step 2: Configure
    rc_create_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);
    rc_wal_autocp = sqlite3_wal_autocheckpoint(db, 100);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16(db, sql_create, -1, &stmt, &pzTail);
    expanded_sql = sqlite3_expanded_sql(stmt);
    rc_step = sqlite3_step(stmt);
    is_explain = sqlite3_stmt_isexplain(stmt);
    vtab_nochange_rc = sqlite3_vtab_nochange((sqlite3_context *)NULL);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(expanded_sql);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open16;
    (void)rc_create_coll;
    (void)rc_wal_autocp;
    (void)rc_prep;
    (void)rc_step;
    (void)is_explain;
    (void)vtab_nochange_rc;
    (void)errmsg16;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}