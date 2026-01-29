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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations/Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char16_t sql16[] = u"SELECT 1 AS col;";
    const void *pzTail = NULL;
    int rc_open = -1;
    int rc_prep = -1;
    int version = 0;
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 set_limit = (sqlite3_int64)1024 * 1024;

    // step 2: Configure
    prev_limit = sqlite3_hard_heap_limit64(set_limit);
    version = sqlite3_libversion_number();

    // step 3: Operate/Validate
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void *)sql16, -1, &stmt, &pzTail);
    const char *stmt_sql = sqlite3_sql(stmt);
    const char *errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    sqlite3_close_v2(db);
    sqlite3_hard_heap_limit64(prev_limit);
    (void)stmt;
    (void)rc_open;
    (void)rc_prep;
    (void)stmt_sql;
    (void)errmsg;
    (void)version;
    (void)prev_limit;
    (void)set_limit;
    // API sequence test completed successfully
    return 66;
}