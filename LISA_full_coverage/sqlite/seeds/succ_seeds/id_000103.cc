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
//<ID> 103
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
    const char *sql_explain = "EXPLAIN SELECT 1;";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    int rc_create_coll = sqlite3_create_collation_v2(db, "my_collation", 1, NULL, NULL, NULL);
    int rc_wal = sqlite3_wal_autocheckpoint(db, 1);

    // step 3: Operate / Validate
    int rc_prep = sqlite3_prepare_v2(db, sql_explain, -1, &stmt, NULL);
    int rc_isexplain = sqlite3_stmt_isexplain(stmt);
    int rc_vtab_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);

    // step 4: Cleanup
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_create_coll;
    (void)rc_wal;
    (void)rc_prep;
    (void)rc_isexplain;
    (void)rc_vtab_nochange;
    (void)rc_finalize;
    (void)rc_close;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}