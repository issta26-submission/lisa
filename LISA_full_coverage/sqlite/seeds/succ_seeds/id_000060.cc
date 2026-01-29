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
//<ID> 60
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
    const void *sql16 = (const void *)L"";
    const void *pzTail = NULL;
    int rc_init = 0;
    int libver = 0;
    sqlite3_int64 prev_limit = 0;
    sqlite3_int64 set_limit = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_close = 0;

    // step 2: Initialize and configure
    rc_init = sqlite3_initialize();
    libver = sqlite3_libversion_number();
    set_limit = (sqlite3_int64)libver * 1024;
    prev_limit = sqlite3_hard_heap_limit64(set_limit);

    // step 3: Operate - open an in-memory database and prepare a UTF-16 statement (empty SQL yields no stmt)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, sql16, -1, &stmt, &pzTail);

    // step 4: Validate and cleanup - restore heap limit and close the database
    (void)sqlite3_hard_heap_limit64(prev_limit);
    rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)libver;
    (void)set_limit;
    (void)prev_limit;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_close;
    (void)stmt;
    (void)pzTail;
    // API sequence test completed successfully
    return 66;
}