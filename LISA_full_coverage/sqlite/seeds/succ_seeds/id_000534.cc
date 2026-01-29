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
//<ID> 534
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
    sqlite3_mutex *mx = NULL;
    const char *sql = "SELECT 1";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_readonly = 0;
    int col_count = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    rc_init = sqlite3_initialize();

    // step 2: Setup / Configure
    rc_open = sqlite3_open16((const void *)L":memory:", &db);
    mx = sqlite3_mutex_alloc(0);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_readonly = sqlite3_stmt_readonly(stmt);
    col_count = sqlite3_column_count(stmt);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_mutex_free(mx);
    rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_readonly;
    (void)col_count;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql;
    (void)stmt;
    (void)db;
    (void)mx;

    // API sequence test completed successfully
    return 66;
}