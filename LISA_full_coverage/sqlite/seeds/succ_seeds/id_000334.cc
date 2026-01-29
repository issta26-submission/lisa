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
//<ID> 334
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
    sqlite3_mutex *mutex = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_blob = 0;
    int rc_finalize = 0;
    sqlite3_int64 prev_limit = 0;
    const char *dbfile = ":memory:";
    const char *sql = "SELECT 1";
    char tail_buffer[16];
    memset(tail_buffer, 0, sizeof(tail_buffer));

    // step 2: Setup
    rc_open = sqlite3_open(dbfile, &db);
    mutex = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1048576);

    // step 3: Core operations
    rc_prepare = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    rc_blob = sqlite3_blob_open(db, "main", "demo", "data", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_blob;
    (void)rc_finalize;
    (void)prev_limit;
    (void)mutex;
    (void)blob;
    (void)tail_buffer;
    (void)dbfile;
    // API sequence test completed successfully
    return 66;
}