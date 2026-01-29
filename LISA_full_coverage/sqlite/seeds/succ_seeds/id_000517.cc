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
//<ID> 517
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
    const char *sql = "SELECT 1 AS one";
    char *expanded = NULL;
    void *buffer = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int errcode = 0;
    sqlite3_uint64 alloc_sz = (sqlite3_uint64)128;

    // step 2: Initialize / Prepare
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    expanded = sqlite3_expanded_sql(stmt);
    buffer = sqlite3_malloc64(alloc_sz);
    memset(buffer, 0, (size_t)alloc_sz);
    errcode = sqlite3_errcode(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    buffer = sqlite3_realloc(buffer, 0);
    sqlite3_close_v2(db);

    (void)expanded;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_finalize;
    (void)errcode;
    (void)sql;
    (void)stmt;
    (void)buffer;
    (void)db;

    // API sequence test completed successfully
    return 66;
}