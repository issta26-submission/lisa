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
//<ID> 66
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
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_close = 0;
    int libver = 0;
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 restored_limit = 0;
    sqlite3_int64 new_limit = (sqlite3_int64)(1024LL * 1024LL);

    // step 2: Initialize / Configure
    rc_init = sqlite3_initialize();
    libver = sqlite3_libversion_number();
    prev_limit = sqlite3_hard_heap_limit64(new_limit);

    // step 3: Operate (open DB and prepare a UTF-16 statement)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void*)u"SELECT 1;", -1, &stmt, NULL);

    // step 4: Validate / Cleanup
    restored_limit = sqlite3_hard_heap_limit64(prev_limit);
    rc_close = sqlite3_close_v2(db);
    (void)rc_init;
    (void)libver;
    (void)prev_limit;
    (void)new_limit;
    (void)restored_limit;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_close;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}