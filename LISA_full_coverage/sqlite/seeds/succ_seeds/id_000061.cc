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
//<ID> 61
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
    const char16_t sql_utf16[] = u"SELECT 1 AS col;";
    const void *pzTail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int libver = 0;
    sqlite3_int64 prev_heap_limit = 0;
    sqlite3_int64 restore_heap_limit = 0;

    // step 2: Configure (query library version and set a hard heap limit based on it)
    libver = sqlite3_libversion_number();
    prev_heap_limit = sqlite3_hard_heap_limit64((sqlite3_int64)libver * 1024);

    // step 3: Operate / Validate (open an in-memory DB and prepare a UTF-16 statement)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void *)sql_utf16, -1, &stmt, &pzTail);

    // step 4: Cleanup (restore heap limit and close database)
    restore_heap_limit = sqlite3_hard_heap_limit64(prev_heap_limit);
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)libver;
    (void)prev_heap_limit;
    (void)restore_heap_limit;
    (void)stmt;
    (void)pzTail;
    // API sequence test completed successfully
    return 66;
}