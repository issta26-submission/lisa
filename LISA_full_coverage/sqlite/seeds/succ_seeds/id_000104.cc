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
//<ID> 104
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
    const void *filename = (const void *)u":memory:";
    const void *create_sql = (const void *)u"CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_stmt_isexplain = 0;
    int rc_coll = 0;
    int rc_wal = 0;
    int rc_vtab_nochange = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_prep = sqlite3_prepare16(db, create_sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_stmt_isexplain = sqlite3_stmt_isexplain(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_coll = sqlite3_create_collation_v2(db, "MYCOL", 1, NULL, NULL, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);
    rc_vtab_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_stmt_isexplain;
    (void)rc_coll;
    (void)rc_wal;
    (void)rc_vtab_nochange;
    (void)rc_close;
    (void)stmt;
    (void)create_sql;
    (void)filename;
    // API sequence test completed successfully
    return 66;
}