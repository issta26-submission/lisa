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
//<ID> 337
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
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_int64 prev_limit = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_blob = 0;
    const char *tail = NULL;
    const char *select_sql = "SELECT 1;";

    // step 2: Setup - open an in-memory database, allocate a mutex, and adjust heap limit
    rc_open = sqlite3_open(":memory:", &db);
    mtx = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)(1024 * 1024));

    // step 3: Core operations - prepare a simple statement and attempt to open a blob handle
    rc_prep = sqlite3_prepare(db, select_sql, -1, &stmt, &tail);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup - finalize prepared statement and restore previous heap limit
    sqlite3_finalize(stmt);
    sqlite3_soft_heap_limit64(prev_limit);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_blob;
    (void)db;
    (void)blob;
    (void)mtx;
    (void)tail;
    (void)select_sql;
    (void)prev_limit;

    // API sequence test completed successfully
    return 66;
}