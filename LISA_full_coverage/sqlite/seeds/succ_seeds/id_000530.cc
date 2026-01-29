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
//<ID> 530
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
    sqlite3_mutex *mutex = NULL;
    const void *filename16 = (const void *)L":memory:";
    const char *sql_select = "SELECT 1;";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_stmt_ro = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    rc_init = sqlite3_initialize();

    // step 2: Setup / Configure
    rc_open = sqlite3_open16(filename16, &db);
    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    // step 3: Operate / Validate
    rc_stmt_ro = sqlite3_stmt_readonly(stmt);
    mutex = sqlite3_mutex_alloc(0);
    sqlite3_mutex_enter(mutex);
    sqlite3_mutex_leave(mutex);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_mutex_free(mutex);
    rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_stmt_ro;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql_select;
    (void)filename16;
    (void)db;
    (void)stmt;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}