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
//<ID> 537
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
    const char *sql = "SELECT 42";
    int rc_init = sqlite3_initialize();
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_shutdown = 0;
    int is_readonly = 0;

    // step 2: Setup (allocate mutex, open in-memory DB)
    mutex = sqlite3_mutex_alloc(0);
    rc_open = sqlite3_open16((const void *)u":memory:", &db);

    // step 3: Operate / Validate (prepare statement and check readonly)
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    is_readonly = sqlite3_stmt_readonly(stmt);

    // step 4: Cleanup (finalize, close, free mutex, shutdown)
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    sqlite3_mutex_free(mutex);
    rc_shutdown = sqlite3_shutdown();

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)is_readonly;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_shutdown;
    (void)sql;
    (void)stmt;
    (void)db;
    (void)mutex;

    return 66;
    // API sequence test completed successfully
}