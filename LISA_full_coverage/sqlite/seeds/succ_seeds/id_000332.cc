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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_int64 prev_limit = 0;
    int rc_open = 0;
    int rc_pcreate = 0;
    int rc_pinsert = 0;
    int rc_blob = 0;
    const char *sql_create = "CREATE TABLE t1(a BLOB);";
    const char *sql_insert = "INSERT INTO t1(rowid,a) VALUES(1, zeroblob(16));";

    // step 2: Setup - open in-memory database, allocate a mutex and adjust heap limit
    rc_open = sqlite3_open(":memory:", &db);
    mtx = sqlite3_mutex_alloc(0);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1024 * 1024);

    // step 3: Core operations - prepare statements and attempt to open a blob handle
    rc_pcreate = sqlite3_prepare(db, sql_create, -1, &stmt_create, NULL);
    sqlite3_finalize(stmt_create);
    rc_pinsert = sqlite3_prepare(db, sql_insert, -1, &stmt_insert, NULL);
    sqlite3_finalize(stmt_insert);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "a", (sqlite3_int64)1, 0, &blob);

    // step 4: Cleanup - restore heap limit and finalize any remaining statements
    sqlite3_soft_heap_limit64(prev_limit);
    (void)rc_open;
    (void)rc_pcreate;
    (void)rc_pinsert;
    (void)rc_blob;
    (void)db;
    (void)blob;
    (void)mtx;

    // API sequence test completed successfully
    return 66;
}