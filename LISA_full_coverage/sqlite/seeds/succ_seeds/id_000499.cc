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
//<ID> 499
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
    const char *sql = "SELECT 1";
    void *mem = NULL;
    const char *colname = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int colcount = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Prepare
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Use APIs (data flows between calls)
    colcount = sqlite3_column_count(stmt);
    colname = sqlite3_column_name(stmt, 0);
    mem = sqlite3_malloc(32);
    mem = sqlite3_realloc(mem, 64);
    sqlite3_set_last_insert_rowid(db, (sqlite3_int64)42);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)colcount;
    (void)colname;
    (void)mem;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}