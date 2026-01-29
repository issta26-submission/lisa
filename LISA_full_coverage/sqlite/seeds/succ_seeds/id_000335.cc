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
//<ID> 335
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
    const char *sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *pzTail = NULL;
    const char *db_name = "main";
    const char *table = "t1";
    const char *column = "data";
    sqlite3_int64 rowid = 1;
    sqlite3_int64 new_limit = (sqlite3_int64)64 * 1024 * 1024;
    sqlite3_int64 prev_limit = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_blob = 0;
    int rc_release = 0;

    // step 2: Setup - open an in-memory database, allocate a mutex, and set heap limit
    rc_open = sqlite3_open(":memory:", &db);
    mtx = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64(new_limit);

    // step 3: Core operations - prepare a statement to create a table, finalize it, then attempt to open a blob handle
    rc_prepare = sqlite3_prepare(db, sql, -1, &stmt, &pzTail);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_blob = sqlite3_blob_open(db, db_name, table, column, rowid, 0, &blob);

    // step 4: Cleanup - release database memory and tidy up
    rc_release = sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)rc_blob;
    (void)rc_release;
    (void)prev_limit;
    (void)new_limit;
    (void)pzTail;
    (void)blob;
    (void)mtx;
    (void)db;
    // API sequence test completed successfully
    return 66;
}