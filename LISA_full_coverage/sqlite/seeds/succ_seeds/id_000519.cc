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
//<ID> 519
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
    const char *sql = "SELECT 123";
    char *expanded = NULL;
    void *buffer = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int errcode_before_close = 0;
    size_t len = 0;

    // step 2: Initialize / Prepare
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Use APIs (data flows between calls)
    expanded = sqlite3_expanded_sql(stmt);
    len = strlen(expanded);
    buffer = sqlite3_malloc64((sqlite3_uint64)(len + 1));
    memcpy(buffer, expanded, len + 1);
    errcode_before_close = sqlite3_errcode(db);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_realloc(expanded, 0);
    sqlite3_realloc(buffer, 0);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)len;
    (void)errcode_before_close;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}