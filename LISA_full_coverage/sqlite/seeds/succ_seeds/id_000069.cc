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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int libver = 0;
    sqlite3_int64 prevHeapLimit = 0;

    // step 2: Configure
    libver = sqlite3_libversion_number();
    prevHeapLimit = sqlite3_hard_heap_limit64((sqlite3_int64)1024 * 1024);

    // step 3: Operate / Validate
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void *)L"CREATE TABLE t(a); INSERT INTO t VALUES(1); SELECT a FROM t;", -1, &stmt, &pzTail);

    // step 4: Cleanup
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)libver;
    (void)prevHeapLimit;
    (void)stmt;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}