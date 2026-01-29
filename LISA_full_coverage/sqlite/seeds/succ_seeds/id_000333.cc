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
//<ID> 333
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
    sqlite3_mutex *mut = NULL;
    sqlite3_int64 prev_limit = 0;
    const char *dbfile = ":memory:";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_blob = 0;
    int rc_finalize = 0;

    // step 2: Setup - open an in-memory database and adjust memory limit and mutex
    rc_open = sqlite3_open(dbfile, &db);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)(64 * 1024 * 1024));
    mut = sqlite3_mutex_alloc(0);

    // step 3: Core operations - prepare a simple statement and attempt to open a blob handle
    rc_prepare = sqlite3_prepare(db, "SELECT name FROM sqlite_master WHERE type='table' LIMIT 1;", -1, &stmt, (const char **)NULL);
    rc_blob = sqlite3_blob_open(db, "main", "sqlite_master", "type", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup - finalize statement and tidy local variables
    rc_finalize = sqlite3_finalize(stmt);

    (void)rc_open;
    (void)prev_limit;
    (void)mut;
    (void)rc_prepare;
    (void)rc_blob;
    (void)rc_finalize;
    (void)dbfile;
    (void)db;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}