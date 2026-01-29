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
//<ID> 539
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
    sqlite3_mutex *mtx = NULL;
    const char *sql = "SELECT 1;";
    const void *filename16 = (const void *)L":memory:";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_readonly = 0;
    int col_count = 0;
    int rc_mutex_alloc = 0;
    int rc_close = 0;
    int rc_finalize = 0;
    int rc_shutdown = 0;

    // step 2: Initialize / Setup
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16(filename16, &db);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    rc_readonly = sqlite3_stmt_readonly(stmt);
    col_count = sqlite3_column_count(stmt);
    mtx = sqlite3_mutex_alloc(0);
    sqlite3_mutex_enter(mtx);
    sqlite3_mutex_leave(mtx);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_mutex_free(mtx);
    rc_close = sqlite3_close_v2(db);
    rc_shutdown = sqlite3_shutdown();

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_readonly;
    (void)col_count;
    (void)rc_mutex_alloc;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_shutdown;
    (void)sql;
    (void)filename16;

    // API sequence test completed successfully
    return 66;
}