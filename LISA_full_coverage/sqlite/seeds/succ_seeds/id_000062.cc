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
//<ID> 62
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
    const void *sql16 = (const void *)L"SELECT 123 AS val;";
    const char *stmt_sql_utf8 = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int libver = 0;
    sqlite3_int64 prev_heap_limit = 0;
    sqlite3_int64 set_heap_limit = (sqlite3_int64)10 * 1024 * 1024; /* 10MB */

    // step 2: Initialize / Configure
    libver = sqlite3_libversion_number();
    prev_heap_limit = sqlite3_hard_heap_limit64(set_heap_limit);
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16(db, sql16, -1, &stmt, NULL);
    stmt_sql_utf8 = sqlite3_sql(stmt);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    sqlite3_hard_heap_limit64(prev_heap_limit);

    (void)libver;
    (void)rc_open;
    (void)rc_prep;
    (void)stmt_sql_utf8;
    (void)set_heap_limit;
    (void)prev_heap_limit;
    (void)sql16;
    return 66;
    // API sequence test completed successfully
}