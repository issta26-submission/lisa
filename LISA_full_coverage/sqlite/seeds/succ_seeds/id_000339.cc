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
//<ID> 339
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
    const char *dbfile = ":memory:";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_close = 0;

    // step 2: Setup
    rc_open = sqlite3_open(dbfile, &db);
    mtx = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)(4 * 1024 * 1024));

    // step 3: Core operations / Validate
    rc_prepare = sqlite3_prepare(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);", -1, &stmt, NULL);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_blob_open = sqlite3_blob_open(db, "main", "test", "data", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup
    rc_close = sqlite3_close(db);

    (void)prev_limit;
    (void)mtx;
    (void)blob;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_close;
    (void)dbfile;
    // API sequence test completed successfully
    return 66;
}