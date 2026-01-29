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
//<ID> 512
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
    const char *sql = "SELECT sqlite_version()";
    const char *tail = NULL;
    char *expanded = NULL;
    void *mem = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_err = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    expanded = sqlite3_expanded_sql(stmt);
    mem = sqlite3_malloc64((sqlite3_uint64)64);
    mem = sqlite3_realloc(mem, 0);
    expanded = (char *)sqlite3_realloc(expanded, 0);
    rc_err = sqlite3_errcode(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_err;
    (void)rc_finalize;
    (void)rc_close;
    (void)tail;
    (void)sql;
    (void)stmt;
    (void)mem;
    (void)expanded;
    // API sequence test completed successfully
    return 66;
}