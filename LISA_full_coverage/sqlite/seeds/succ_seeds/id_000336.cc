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
//<ID> 336
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
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 set_limit = (sqlite3_int64)65536;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_blob = 0;
    char sql_create[] = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    char pad_buf[8];
    memset(pad_buf, 0, sizeof(pad_buf));

    // step 2: Setup - open an in-memory database and prepare a statement
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare(db, sql_create, -1, &stmt, NULL);

    // step 3: Configure & Operate - set soft heap limit, allocate a mutex, try to open a blob
    prev_limit = sqlite3_soft_heap_limit64(set_limit);
    mutex = sqlite3_mutex_alloc(0);
    rc_blob = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup - finalize prepared statement and tidy local variables
    sqlite3_finalize(stmt);
    (void)prev_limit;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_blob;
    (void)db;
    (void)blob;
    (void)mutex;
    // API sequence test completed successfully
    return 66;
}