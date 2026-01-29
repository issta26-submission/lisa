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
//<ID> 64
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
    const void *pzTail = NULL;
    const void *sql16 = (const void *)L"CREATE TABLE t(a); INSERT INTO t VALUES(42); SELECT a FROM t;";
    int rc_open = 0;
    int rc_prep = 0;
    int libver = 0;
    sqlite3_int64 prev_limit = 0;

    // step 2: Setup - query library version and set a hard heap limit
    libver = sqlite3_libversion_number();
    prev_limit = sqlite3_hard_heap_limit64((sqlite3_int64) (1024 * 1024)); // set ~1MB hard heap limit

    // step 3: Core operations - open an in-memory database and prepare a UTF-16 SQL statement
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, sql16, -1, &stmt, &pzTail);

    // step 4: Cleanup - restore previous heap limit and close the database handle
    sqlite3_hard_heap_limit64(prev_limit);
    sqlite3_close_v2(db);

    (void)stmt;
    (void)pzTail;
    (void)rc_open;
    (void)rc_prep;
    (void)libver;
    (void)prev_limit;
    (void)sql16;
    // API sequence test completed successfully
    return 66;
}