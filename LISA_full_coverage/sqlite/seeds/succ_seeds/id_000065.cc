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
//<ID> 65
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
    const char16_t *sql16 = u"CREATE TABLE t(a); INSERT INTO t VALUES(1); SELECT a FROM t;";
    const void *pzTail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_close = 0;
    int libver = 0;
    sqlite3_int64 heap_old = 0;
    sqlite3_int64 heap_new = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    libver = sqlite3_libversion_number();
    heap_old = sqlite3_hard_heap_limit64(0);
    heap_new = sqlite3_hard_heap_limit64((sqlite3_int64)libver * 1024);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16(db, (const void *)sql16, -1, &stmt, &pzTail);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    (void)stmt;
    (void)pzTail;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_close;
    (void)libver;
    (void)heap_old;
    (void)heap_new;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}