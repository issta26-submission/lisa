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
//<ID> 338
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
    int tmpbuf[4];
    const char *dbfile = ":memory:";
    const char sql[] = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";

    memset(tmpbuf, 0, sizeof(tmpbuf));

    // step 2: Setup - open database, adjust heap limit and allocate a mutex
    rc_open = sqlite3_open(dbfile, &db);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1024 * 1024); // set 1MB soft limit, capture previous
    mutex = sqlite3_mutex_alloc(0);

    // step 3: Core operations - prepare a statement, attempt to open a blob, then finalize the statement
    rc_prepare = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", (sqlite3_int64)1, 0, &blob);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup - restore heap limit and release some DB memory
    sqlite3_soft_heap_limit64(prev_limit);
    sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_blob;
    (void)rc_finalize;
    (void)mutex;
    (void)blob;
    (void)tmpbuf;
    (void)dbfile;
    (void)prev_limit;
    // API sequence test completed successfully
    return 66;
}