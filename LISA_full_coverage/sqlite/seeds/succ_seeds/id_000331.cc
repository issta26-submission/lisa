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
//<ID> 331
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
    const char *sql = "CREATE TABLE test_blob(id INTEGER PRIMARY KEY, data BLOB);";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_blob_open = 0;
    int rc_finalize = 0;
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 new_limit = (sqlite3_int64)(1024 * 1024);

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - prepare a statement, allocate a mutex, set soft heap limit, attempt to open a blob
    rc_prepare = sqlite3_prepare(db, sql, -1, &stmt, &pzTail);
    mtx = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64(new_limit);
    rc_blob_open = sqlite3_blob_open(db, "main", "test_blob", "data", 1, 0, &blob);

    // step 4: Cleanup - finalize statement and tidy up
    rc_finalize = sqlite3_finalize(stmt);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_blob_open;
    (void)rc_finalize;
    (void)prev_limit;
    (void)new_limit;
    (void)db;
    (void)blob;
    (void)mtx;
    // API sequence test completed successfully
    return 66;
}