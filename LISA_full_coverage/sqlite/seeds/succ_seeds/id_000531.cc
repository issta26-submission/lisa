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
//<ID> 531
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
    sqlite3_mutex *mutex = NULL;
    const char *sql = "SELECT 1;";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int readonly_flag = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Setup
    rc_init = sqlite3_initialize();
    mutex = sqlite3_mutex_alloc(0);
    rc_open = sqlite3_open16((const void *)u":memory:", &db);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    readonly_flag = sqlite3_stmt_readonly(stmt);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_mutex_free(mutex);
    rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)readonly_flag;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql;
    (void)stmt;
    (void)db;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}